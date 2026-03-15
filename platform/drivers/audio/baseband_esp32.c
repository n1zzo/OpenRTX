#include <zephyr/kernel.h>
#include <zephyr/drivers/i2s.h>
#include <zephyr/device.h>

#define I2S_TX_NODE DT_NODELABEL(i2s0)
#define BUFFER_SIZE 960
#define BUFFER_BYTES (BUFFER_SIZE * sizeof(int16_t))

static const struct device *i2s_dev = DEVICE_DT_GET(I2S_TX_NODE);
static bool baseband_running = false;
static int16_t app_buffer[BUFFER_SIZE];
static uint32_t write_count = 0;

K_MEM_SLAB_DEFINE(i2s_tx_slab, BUFFER_BYTES, 16, 4);

int baseband_init(void)
{
    if (!device_is_ready(i2s_dev)) return -1;

    struct i2s_config config = {
        .word_size = 16,
        .channels = 2,  // Stereo required by ESP32-S3 I2S
        .format = I2S_FMT_DATA_FORMAT_I2S,
        .options = I2S_OPT_BIT_CLK_MASTER | I2S_OPT_FRAME_CLK_MASTER,
        .frame_clk_freq = 48000,
        .mem_slab = &i2s_tx_slab,
        .block_size = BUFFER_BYTES,
        .timeout = 1000,
    };

    int ret = i2s_configure(i2s_dev, I2S_DIR_TX, &config);
    printk("I2S config: %d\n", ret);
    write_count = 0;
    return ret;
}

int baseband_start(void)
{
    if (baseband_running) return 0;
    
    // Must queue at least one buffer before START
    void *initial_buf;
    int ret = k_mem_slab_alloc(&i2s_tx_slab, &initial_buf, K_MSEC(100));
    if (ret < 0) {
        printk("Failed to alloc initial buffer: %d\n", ret);
        return ret;
    }
    
    // Fill with stereo test pattern (duplicate each sample for L+R)
    int16_t *buf = (int16_t *)initial_buf;
    for (int i = 0; i < BUFFER_SIZE / 2; i++) {
        int16_t sample = (i & 1) ? 3000 : -3000;  // Alternating pattern
        buf[i * 2 + 0] = sample;  // Left channel
        buf[i * 2 + 1] = sample;  // Right channel
    }
    
    // Queue the buffer
    ret = i2s_write(i2s_dev, initial_buf, BUFFER_BYTES);
    if (ret < 0) {
        printk("Failed to queue initial buffer: %d\n", ret);
        k_mem_slab_free(&i2s_tx_slab, initial_buf);
        return ret;
    }
    
    printk("Queued initial buffer with alternating +3000/-3000 pattern\n");
    
    // Now START will succeed because there is data queued
    ret = i2s_trigger(i2s_dev, I2S_DIR_TX, I2S_TRIGGER_START);
    if (ret == 0) {
        baseband_running = true;
        printk("I2S started (writes: %u)\n", write_count);
    } else {
        printk("I2S start failed: %d\n", ret);
    }
    return ret;
}

int baseband_stop(void)
{
    if (!baseband_running) return 0;
    int ret = i2s_trigger(i2s_dev, I2S_DIR_TX, I2S_TRIGGER_STOP);
    if (ret == 0) {
        baseband_running = false;
        printk("I2S stopped (total writes: %u)\n", write_count);
    }
    return ret;
}

int baseband_write(const int16_t *samples, size_t count)
{
    if (!baseband_running) return -1;
    
    // Allocate buffer from slab
    void *tx_buf;
    int ret = k_mem_slab_alloc(&i2s_tx_slab, &tx_buf, K_MSEC(100));
    if (ret < 0) {
        return ret;
    }
    
    // For stereo I2S: duplicate each mono sample for L+R channels
    // Input: count mono samples -> Output: count stereo frames (2*count samples)
    int16_t *stereo_buf = (int16_t *)tx_buf;
    size_t frames = (count < (BUFFER_SIZE / 2)) ? count : (BUFFER_SIZE / 2);
    
    for (size_t i = 0; i < frames; i++) {
        stereo_buf[i * 2 + 0] = samples[i];  // Left channel
        stereo_buf[i * 2 + 1] = samples[i];  // Right channel (same as left)
    }
    
    // Zero-fill remainder if needed
    if (frames < (BUFFER_SIZE / 2)) {
        memset(&stereo_buf[frames * 2], 0, (BUFFER_SIZE - frames * 2) * 2);
    }
    
    // Queue for transmission
    ret = i2s_write(i2s_dev, tx_buf, BUFFER_BYTES);
    if (ret < 0) {
        k_mem_slab_free(&i2s_tx_slab, tx_buf);
        return ret;
    }
    
    write_count++;
    if (write_count <= 5 || write_count % 10 == 0) {
        printk("  Write #%u queued (%zu samples)\n", write_count, count);
    }
    
    return count;  // Return number of mono samples written
}

int16_t *baseband_get_buffer(void) { return app_buffer; }
size_t baseband_get_buffer_size(void) { return BUFFER_SIZE; }

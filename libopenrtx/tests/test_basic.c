/**
 * @file test_basic.c
 * @brief Basic libopenrtx test
 */

#include <stdio.h>
#include <assert.h>
#include "libopenrtx.h"
#include "libopenrtx_audio.h"

int main(int argc, char* argv[])
{
    printf("=== libopenrtx Basic Test ===\n\n");
    
    // Test initialization
    printf("Testing libopenrtx_init()... ");
    int ret = libopenrtx_init();
    assert(ret == LIBOPENRTX_OK);
    printf("OK\n");
    
    // Test frequency
    printf("Testing libopenrtx_set_frequency()... ");
    ret = libopenrtx_set_frequency(145500000);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_frequency() == 145500000);
    printf("OK (%.3f MHz)\n", libopenrtx_get_frequency() / 1000000.0);
    
    // Test mode
    printf("Testing libopenrtx_set_mode()... ");
    ret = libopenrtx_set_mode(MODE_FM);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_mode() == MODE_FM);
    
    ret = libopenrtx_set_mode(MODE_M17);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_mode() == MODE_M17);
    printf("OK (mode=%d)\n", libopenrtx_get_mode());
    
    // Test PTT
    printf("Testing libopenrtx_set_ptt()... ");
    ret = libopenrtx_set_ptt(true);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_ptt() == true);
    
    ret = libopenrtx_set_ptt(false);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_ptt() == false);
    printf("OK\n");
    
    // Test volume/gain
    printf("Testing libopenrtx_set_volume()... ");
    ret = libopenrtx_set_volume(0.75);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_volume() == 0.75);
    printf("OK (volume=%.2f)\n", libopenrtx_get_volume());
    
    printf("Testing libopenrtx_set_mic_gain()... ");
    ret = libopenrtx_set_mic_gain(0.5);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_mic_gain() == 0.5);
    printf("OK (gain=%.2f)\n", libopenrtx_get_mic_gain());
    
    // Test CTCSS
    printf("Testing libopenrtx_set_ctcss()... ");
    ret = libopenrtx_set_ctcss(100);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_ctcss() == 100);
    
    ret = libopenrtx_set_ctcss(0);
    assert(ret == LIBOPENRTX_OK);
    assert(libopenrtx_get_ctcss() == 0);
    printf("OK\n");
    
    // Test state monitoring
    printf("Testing state functions...\n");
    printf("  RSSI: %.1f dBm\n", libopenrtx_get_rssi());
    printf("  Battery: %.2f V (%d%%)\n", libopenrtx_get_battery(), 
           libopenrtx_get_battery_percent());
    printf("  Squelch: %s\n", libopenrtx_get_squelch_open() ? "OPEN" : "CLOSED");
    
    // Test channels
    printf("Testing channel functions...\n");
    printf("  Channel count: %d\n", libopenrtx_channel_count());
    
    libopenrtx_channel_t channel;
    channel.name[0] = '\0';  // Initialize for safety
    ret = libopenrtx_channel_get(0, &channel);
    assert(ret == LIBOPENRTX_OK);
    printf("  Channel 0: %s @ %.3f MHz\n", channel.name, 
           channel.rx_frequency / 1000000.0);
    
    // Test audio codec functions
    printf("Testing audio functions...\n");
    printf("  PCM frame size: %d samples\n", libopenrtx_get_pcm_frame_size());
    printf("  FM encoded frame size: %d bytes\n", 
           libopenrtx_get_encoded_frame_size(MODE_FM));
    printf("  M17 encoded frame size: %d bytes\n", 
           libopenrtx_get_encoded_frame_size(MODE_M17));
    
    // Test encoding/decoding
    int16_t pcm_in[160] = {0};
    uint8_t encoded[128];
    size_t encoded_len = 0;
    int16_t pcm_out[160];
    size_t pcm_len = 0;
    
    printf("Testing encode/decode...\n");
    ret = libopenrtx_encode(pcm_in, encoded, sizeof(encoded), &encoded_len, MODE_FM);
    printf("  Encoded %d samples -> %zu bytes\n", 160, encoded_len);
    
    ret = libopenrtx_decode(encoded, encoded_len, pcm_out, sizeof(pcm_out), &pcm_len, MODE_FM);
    printf("  Decoded %zu bytes -> %zu samples\n", encoded_len, pcm_len);
    
    // Cleanup
    printf("\nTesting libopenrtx_terminate()... ");
    libopenrtx_terminate();
    printf("OK\n");
    
    printf("\n=== All tests passed! ===\n");
    return 0;
}
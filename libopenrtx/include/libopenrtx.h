/**
 * @file libopenrtx.h
 * @brief OpenRTX Core Library API
 * 
 * This library provides a clean C API for radio control and audio
 * processing, suitable for use by GTK applications via FFI.
 */

#ifndef LIBOPENRTX_H
#define LIBOPENRTX_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Version
 * @{
 */
#define LIBOPENRTX_VERSION_MAJOR 0
#define LIBOPENRTX_VERSION_MINOR 1
#define LIBOPENRTX_VERSION_PATCH 0
/** @} */

/**
 * @name Return codes
 * @{
 */
#define LIBOPENRTX_OK          0
#define LIBOPENRTX_ERROR      -1
#define LIBOPENRTX_EINIT      -2
#define LIBOPENRTX_EPARAM     -3
#define LIBOPENRTX_ENOENT     -4
#define LIBOPENRTX_EBUSY      -5
#define LIBOPENRTX_ETIMEOUT   -6
/** @} */

/**
 * @name Operating modes
 * @{
 */
#define MODE_FM     0
#define MODE_NFM    1
#define MODE_AM     2
#define MODE_USB    3
#define MODE_LSB    4
#define MODE_M17    5
#define MODE_DMR    6
/** @} */

/**
 * @name Lifecycle
 * @{
 */

/**
 * Initialize the radio subsystem
 * @return LIBOPENRTX_OK on success, error code otherwise
 */
int libopenrtx_init(void);

/**
 * Shutdown the radio subsystem
 */
void libopenrtx_terminate(void);

/**
 * Check if initialized
 * @return true if initialized
 */
bool libopenrtx_is_initialized(void);

/** @} */

/**
 * @name Radio Control
 * @{
 */

/**
 * Set the operating frequency
 * @param khz Frequency in kHz (e.g., 145500000 for 145.5 MHz)
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_frequency(uint32_t khz);

/**
 * Get the current frequency
 * @return Frequency in kHz
 */
uint32_t libopenrtx_get_frequency(void);

/**
 * Set the operating mode
 * @param mode One of MODE_FM, MODE_NFM, MODE_AM, MODE_USB, MODE_LSB, MODE_M17, MODE_DMR
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_mode(int mode);

/**
 * Get the current mode
 * @return Current mode
 */
int libopenrtx_get_mode(void);

/**
 * Get the mode as a string
 * @param buffer Buffer to write mode string
 * @param size Size of buffer
 */
void libopenrtx_get_mode_string(char* buffer, size_t size);

/**
 * Set Push-To-Talk state
 * @param enabled true to enable TX, false for RX
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_ptt(bool enabled);

/**
 * Get PTT state
 * @return true if transmitting
 */
bool libopenrtx_get_ptt(void);

/**
 * Enable receiver
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_enable_rx(void);

/**
 * Disable receiver
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_disable_rx(void);

/** @} */

/**
 * @name Audio Control
 * @{
 */

/**
 * Set speaker volume
 * @param level Volume level 0.0-1.0
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_volume(float level);

/**
 * Get speaker volume
 * @return Current volume level
 */
float libopenrtx_get_volume(void);

/**
 * Set microphone gain
 * @param gain Gain level 0.0-1.0
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_mic_gain(float gain);

/**
 * Get microphone gain
 * @return Current mic gain
 */
float libopenrtx_get_mic_gain(void);

/**
 * Set CTCSS tone
 * @param hz CTCSS frequency in Hz (0 = off)
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_ctcss(uint32_t hz);

/**
 * Get CTCSS tone
 * @return CTCSS frequency in Hz (0 = off)
 */
uint32_t libopenrtx_get_ctcss(void);

/** @} */

/**
 * @name State Monitoring
 * @{
 */

/**
 * Get RSSI (Received Signal Strength Indicator)
 * @return RSSI in dBm (typically -120 to 0)
 */
float libopenrtx_get_rssi(void);

/**
 * Get battery voltage
 * @return Battery voltage in Volts
 */
float libopenrtx_get_battery(void);

/**
 * Get battery percentage
 * @return Battery percentage 0-100
 */
int libopenrtx_get_battery_percent(void);

/**
 * Check if squelch is open
 * @return true if squelch is open
 */
bool libopenrtx_get_squelch_open(void);

/** @} */

/**
 * @name Channel Memory
 * @{
 */

#define MAX_CHANNEL_NAME 16

typedef struct {
    char name[MAX_CHANNEL_NAME + 1];
    uint32_t rx_frequency;
    uint32_t tx_frequency;
    int mode;
    uint32_t ctcss;
    uint8_t power;      // 0-10W typically
    int8_t shift;       // TX offset direction: -1, 0, +1
    uint32_t offset;    // TX offset in kHz
    bool rx_enabled;
    bool tx_enabled;
} libopenrtx_channel_t;

/**
 * Get channel by index
 * @param index Channel index (0-based)
 * @param channel Output channel structure
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_channel_get(int index, libopenrtx_channel_t* channel);

/**
 * Get number of channels
 * @return Number of stored channels
 */
int libopenrtx_channel_count(void);

/**
 * Set current channel by index
 * @param index Channel index
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_channel_select(int index);

/**
 * Get current channel index
 * @return Current channel index
 */
int libopenrtx_channel_get_selected(void);

/** @} */

/**
 * @name Event Callbacks
 * @{
 */

/**
 * Event types for callbacks
 */
typedef enum {
    LIBOPENRTX_EVENT_RSSI_CHANGED,
    LIBOPENRTX_EVENT_MODE_CHANGED,
    LIBOPENRTX_EVENT_FREQUENCY_CHANGED,
    LIBOPENRTX_EVENT_PTT_CHANGED,
    LIBOPENRTX_EVENT_SQUELCH_OPEN,
    LIBOPENRTX_EVENT_SQUELCH_CLOSE,
    LIBOPENRTX_EVENT_BATTERY_LOW,
    LIBOPENRTX_EVENT_CHANNEL_CHANGED,
} libopenrtx_event_type_t;

/**
 * Event data union
 */
typedef union {
    float rssi;
    int mode;
    uint32_t frequency;
    bool ptt;
    bool squelch;
    float battery;
    int channel;
} libopenrtx_event_data_t;

/**
 * Event callback function type
 * @param event Event type
 * @param data Event-specific data
 * @param user_data User data passed to libopenrtx_set_event_callback
 */
typedef void (*libopenrtx_event_cb)(libopenrtx_event_type_t event, 
                                     libopenrtx_event_data_t* data,
                                     void* user_data);

/**
 * Register event callback
 * @param callback Callback function
 * @param user_data User data passed to callback
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_set_event_callback(libopenrtx_event_cb callback, void* user_data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LIBOPENRTX_H */
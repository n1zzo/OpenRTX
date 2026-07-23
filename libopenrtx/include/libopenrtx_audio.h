/**
 * @file libopenrtx_audio.h
 * @brief Audio API for OpenRTX Core Library
 * 
 * This module handles audio encoding/decoding for transmission and
 * reception. It provides callback-based audio streaming for
 * integration with UI audio pipelines.
 */

#ifndef LIBOPENRTX_AUDIO_H
#define LIBOPENRTX_AUDIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Audio Configuration
 * @{
 */

/** Audio sample rate */
#define LIBOPENRTX_AUDIO_SAMPLE_RATE  16000

/** Samples per frame (10ms at 16kHz) */
#define LIBOPENRTX_AUDIO_FRAME_SIZE   160

/** Audio buffer size in frames */
#define LIBOPENRTX_AUDIO_BUFFER_FRAMES 50

/** @} */

/**
 * @name Audio Formats
 * @{
 */

/** PCM format: signed 16-bit little-endian */
#define LIBOPENRTX_PCM_S16LE  0

/** @} */

/**
 * @name Audio Return Codes
 * @{
 */

#define LIBOPENRTX_AUDIO_OK        0
#define LIBOPENRTX_AUDIO_ENODEV   -10
#define LIBOPENRTX_AUDIO_EBUSY    -11
#define LIBOPENRTX_AUDIO_ESTATE   -12
#define LIBOPENRTX_AUDIO_EPARAM   -13

/** @} */

/**
 * @name Audio State
 * @{
 */

/** Audio not initialized */
#define LIBOPENRTX_AUDIO_STATE_CLOSED    0
/** Audio initialized, idle */
#define LIBOPENRTX_AUDIO_STATE_IDLE      1
/** Audio capturing (TX) */
#define LIBOPENRTX_AUDIO_STATE_CAPTURING 2
/** Audio playing (RX) */
#define LIBOPENRTX_AUDIO_STATE_PLAYING   3
/** Audio full duplex */
#define LIBOPENRTX_AUDIO_STATE_DUPLEX    4

/** @} */

/**
 * @name TX Audio Interface
 * @{
 */

/**
 * Audio frame callback for TX (microphone)
 * Called when a new frame of microphone data is available.
 * 
 * @param samples Pointer to PCM samples (16-bit signed, mono)
 * @param count Number of samples (typically LIBOPENRTX_AUDIO_FRAME_SIZE)
 * @param user_data User data from libopenrtx_audio_set_tx_callback
 * @return 0 to continue, -1 to stop capturing
 */
typedef int (*libopenrtx_tx_cb)(const int16_t* samples, 
                                 int count,
                                 void* user_data);

/**
 * Set TX (microphone) callback
 * @param callback Callback function
 * @param user_data User data passed to callback
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_set_tx_callback(libopenrtx_tx_cb callback, void* user_data);

/**
 * Start audio capture (microphone)
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_start_capture(void);

/**
 * Stop audio capture
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_stop_capture(void);

/**
 * Check if capturing
 * @return true if capturing
 */
bool libopenrtx_audio_is_capturing(void);

/** @} */

/**
 * @name RX Audio Interface
 * @{
 */

/**
 * Audio frame callback for RX (speaker)
 * Called when decoded audio is ready to play.
 * 
 * @param samples Pointer to PCM samples (16-bit signed, mono)
 * @param count Number of samples
 * @param user_data User data from libopenrtx_audio_set_rx_callback
 * @return 0 to continue, -1 to stop playing
 */
typedef int (*libopenrtx_rx_cb)(const int16_t* samples,
                                 int count,
                                 void* user_data);

/**
 * Set RX (speaker) callback
 * @param callback Callback function
 * @param user_data User data passed to callback
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_set_rx_callback(libopenrtx_rx_cb callback, void* user_data);

/**
 * Write decoded audio samples for playback
 * @param samples PCM samples
 * @param count Number of samples
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_write_rx(const int16_t* samples, int count);

/**
 * Start audio playback (speaker)
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_start_playback(void);

/**
 * Stop audio playback
 * @return LIBOPENRTX_AUDIO_OK on success
 */
int libopenrtx_audio_stop_playback(void);

/**
 * Check if playing
 * @return true if playing
 */
bool libopenrtx_audio_is_playing(void);

/** @} */

/**
 * @name Codec Interface
 * @{
 */

/**
 * Encode a single frame of audio
 * 
 * @param pcm_in Input PCM samples (LIBOPENRTX_AUDIO_FRAME_SIZE samples)
 * @param encoded_out Output buffer for encoded data
 * @param encoded_size Size of output buffer
 * @param encoded_len_out Pointer to receive actual encoded length
 * @param mode Operating mode (MODE_FM, MODE_M17, etc.)
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_encode(const int16_t* pcm_in,
                      uint8_t* encoded_out,
                      size_t encoded_size,
                      size_t* encoded_len_out,
                      int mode);

/**
 * Decode a single frame of audio
 * 
 * @param encoded_in Input encoded data
 * @param encoded_len Length of encoded data
 * @param pcm_out Output buffer for PCM
 * @param pcm_size Size of output buffer
 * @param pcm_len_out Pointer to receive actual sample count
 * @param mode Operating mode
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_decode(const uint8_t* encoded_in,
                      size_t encoded_len,
                      int16_t* pcm_out,
                      size_t pcm_size,
                      size_t* pcm_len_out,
                      int mode);

/**
 * Get the expected encoded frame size for a mode
 * @param mode Operating mode
 * @return Encoded frame size in bytes
 */
int libopenrtx_get_encoded_frame_size(int mode);

/**
 * Get the PCM frame size
 * @return PCM frame size in samples
 */
int libopenrtx_get_pcm_frame_size(void);

/** @} */

/**
 * @name TX Level Control
 * @{
 */

/**
 * Set TX pre-emphasis (transmit filtering)
 * @param enabled Enable pre-emphasis
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_audio_set_pre_emphasis(bool enabled);

/**
 * Set RX de-emphasis (receive filtering)
 * @param enabled Enable de-emphasis
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_audio_set_de_emphasis(bool enabled);

/**
 * Enable TX high-pass filter (removes low frequency rumble)
 * @param enabled Enable high-pass filter
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_audio_set_tx_highpass(bool enabled);

/**
 * Enable RX low-pass filter (removes noise)
 * @param enabled Enable low-pass filter
 * @return LIBOPENRTX_OK on success
 */
int libopenrtx_audio_set_rx_lowpass(bool enabled);

/** @} */

/**
 * @name Audio Status
 * @{
 */

/**
 * Get audio state
 * @return One of LIBOPENRTX_AUDIO_STATE_*
 */
int libopenrtx_audio_get_state(void);

/**
 * Get peak TX level (for VU meter)
 * @return Peak level 0.0-1.0
 */
float libopenrtx_audio_get_tx_peak(void);

/**
 * Get peak RX level (for VU meter)
 * @return Peak level 0.0-1.0
 */
float libopenrtx_audio_get_rx_peak(void);

/**
 * Reset peak meters
 */
void libopenrtx_audio_reset_peaks(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LIBOPENRTX_AUDIO_H */
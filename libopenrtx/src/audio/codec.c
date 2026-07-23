/**
 * @file codec.c
 * @brief Audio codec interface
 */

#include "libopenrtx_audio.h"
#include "libopenrtx.h"
#include <stdio.h>
#include <string.h>

// TX callback state
static libopenrtx_tx_cb g_tx_callback = NULL;
static void* g_tx_user_data = NULL;

// RX callback state
static libopenrtx_rx_cb g_rx_callback = NULL;
static void* g_rx_user_data = NULL;

// Audio state
static int g_audio_state = LIBOPENRTX_AUDIO_STATE_CLOSED;

// Peak levels
static float g_tx_peak = 0.0f;
static float g_rx_peak = 0.0f;

// Pre/de-emphasis state
static bool g_pre_emphasis = false;
static bool g_de_emphasis = true;

int libopenrtx_audio_set_tx_callback(libopenrtx_tx_cb callback, void* user_data)
{
    g_tx_callback = callback;
    g_tx_user_data = user_data;
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_start_capture(void)
{
    if (g_tx_callback == NULL) {
        printf("[libopenrtx] No TX callback set!\n");
        return LIBOPENRTX_AUDIO_ENODEV;
    }
    
    g_audio_state = LIBOPENRTX_AUDIO_STATE_CAPTURING;
    printf("[libopenrtx] Audio capture started\n");
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_stop_capture(void)
{
    g_audio_state = LIBOPENRTX_AUDIO_STATE_IDLE;
    printf("[libopenrtx] Audio capture stopped\n");
    return LIBOPENRTX_AUDIO_OK;
}

bool libopenrtx_audio_is_capturing(void)
{
    return g_audio_state == LIBOPENRTX_AUDIO_STATE_CAPTURING ||
           g_audio_state == LIBOPENRTX_AUDIO_STATE_DUPLEX;
}

int libopenrtx_audio_set_rx_callback(libopenrtx_rx_cb callback, void* user_data)
{
    g_rx_callback = callback;
    g_rx_user_data = user_data;
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_write_rx(const int16_t* samples, int count)
{
    if (g_rx_callback) {
        g_rx_callback(samples, count, g_rx_user_data);
    }
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_start_playback(void)
{
    g_audio_state = LIBOPENRTX_AUDIO_STATE_PLAYING;
    printf("[libopenrtx] Audio playback started\n");
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_stop_playback(void)
{
    g_audio_state = LIBOPENRTX_AUDIO_STATE_IDLE;
    printf("[libopenrtx] Audio playback stopped\n");
    return LIBOPENRTX_AUDIO_OK;
}

bool libopenrtx_audio_is_playing(void)
{
    return g_audio_state == LIBOPENRTX_AUDIO_STATE_PLAYING ||
           g_audio_state == LIBOPENRTX_AUDIO_STATE_DUPLEX;
}

int libopenrtx_encode(const int16_t* pcm_in,
                      uint8_t* encoded_out,
                      size_t encoded_size,
                      size_t* encoded_len_out,
                      int mode)
{
    // TODO: Actually encode using CODEC2 or other codecs
    // For now, just pass through with size indication
    
    if (pcm_in == NULL || encoded_out == NULL || encoded_len_out == NULL) {
        return LIBOPENRTX_EPARAM;
    }
    
    // Simulated encoding - just copy raw PCM (for testing)
    // Real implementation would use CODEC2_encode()
    size_t copy_size = (encoded_size < sizeof(int16_t) * LIBOPENRTX_AUDIO_FRAME_SIZE) 
                       ? encoded_size 
                       : sizeof(int16_t) * LIBOPENRTX_AUDIO_FRAME_SIZE;
    
    // For FM mode, just return the raw samples as a marker
    // Real FM would use different encoding
    *encoded_len_out = copy_size;
    
    return LIBOPENRTX_OK;
}

int libopenrtx_decode(const uint8_t* encoded_in,
                      size_t encoded_len,
                      int16_t* pcm_out,
                      size_t pcm_size,
                      size_t* pcm_len_out,
                      int mode)
{
    // TODO: Actually decode
    if (pcm_out == NULL || pcm_len_out == NULL) {
        return LIBOPENRTX_EPARAM;
    }
    
    // For now, return silence
    size_t samples = (pcm_size / sizeof(int16_t)) < LIBOPENRTX_AUDIO_FRAME_SIZE 
                     ? (pcm_size / sizeof(int16_t)) 
                     : LIBOPENRTX_AUDIO_FRAME_SIZE;
    
    memset(pcm_out, 0, samples * sizeof(int16_t));
    *pcm_len_out = samples;
    
    return LIBOPENRTX_OK;
}

int libopenrtx_get_encoded_frame_size(int mode)
{
    // TODO: Return actual sizes per mode
    switch (mode) {
        case MODE_FM:
        case MODE_NFM:
            return 128;  // Raw PCM frame
        case MODE_M17:
            return 16;   // CODEC2 3200
        case MODE_DMR:
            return 9;    // DMR vocoder
        default:
            return 128;
    }
}

int libopenrtx_get_pcm_frame_size(void)
{
    return LIBOPENRTX_AUDIO_FRAME_SIZE;
}

int libopenrtx_audio_set_pre_emphasis(bool enabled)
{
    g_pre_emphasis = enabled;
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_set_de_emphasis(bool enabled)
{
    g_de_emphasis = enabled;
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_set_tx_highpass(bool enabled)
{
    // TODO: Apply high-pass filter
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_set_rx_lowpass(bool enabled)
{
    // TODO: Apply low-pass filter
    return LIBOPENRTX_AUDIO_OK;
}

int libopenrtx_audio_get_state(void)
{
    return g_audio_state;
}

float libopenrtx_audio_get_tx_peak(void)
{
    return g_tx_peak;
}

float libopenrtx_audio_get_rx_peak(void)
{
    return g_rx_peak;
}

void libopenrtx_audio_reset_peaks(void)
{
    g_tx_peak = 0.0f;
    g_rx_peak = 0.0f;
}
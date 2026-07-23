/**
 * @file control.c
 * @brief Radio control functions
 */

#include "libopenrtx.h"
#include "state.h"
#include <stdio.h>
#include <string.h>

int libopenrtx_set_frequency(uint32_t khz)
{
    g_frequency = khz;
    printf("[libopenrtx] Frequency set to %u kHz\n", khz);
    return LIBOPENRTX_OK;
}

uint32_t libopenrtx_get_frequency(void)
{
    return g_frequency;
}

int libopenrtx_set_mode(int mode)
{
    if (mode < MODE_FM || mode > MODE_DMR) {
        return LIBOPENRTX_EPARAM;
    }
    g_mode = mode;
    
    const char* mode_names[] = {"FM", "NFM", "AM", "USB", "LSB", "M17", "DMR"};
    printf("[libopenrtx] Mode set to %s\n", mode_names[mode]);
    return LIBOPENRTX_OK;
}

int libopenrtx_get_mode(void)
{
    return g_mode;
}

void libopenrtx_get_mode_string(char* buffer, size_t size)
{
    const char* mode_names[] = {"FM", "NFM", "AM", "USB", "LSB", "M17", "DMR"};
    if (g_mode >= 0 && g_mode <= MODE_DMR) {
        strncpy(buffer, mode_names[g_mode], size - 1);
        buffer[size - 1] = '\0';
    } else {
        strncpy(buffer, "UNKNOWN", size - 1);
        buffer[size - 1] = '\0';
    }
}

int libopenrtx_set_ptt(bool enabled)
{
    g_ptt = enabled;
    printf("[libopenrtx] PTT %s\n", enabled ? "ON" : "OFF");
    return LIBOPENRTX_OK;
}

bool libopenrtx_get_ptt(void)
{
    return g_ptt;
}

int libopenrtx_enable_rx(void)
{
    printf("[libopenrtx] RX enabled\n");
    return LIBOPENRTX_OK;
}

int libopenrtx_disable_rx(void)
{
    printf("[libopenrtx] RX disabled\n");
    return LIBOPENRTX_OK;
}

int libopenrtx_set_volume(float level)
{
    if (level < 0.0f || level > 1.0f) {
        return LIBOPENRTX_EPARAM;
    }
    g_volume = level;
    return LIBOPENRTX_OK;
}

float libopenrtx_get_volume(void)
{
    return g_volume;
}

int libopenrtx_set_mic_gain(float gain)
{
    if (gain < 0.0f || gain > 1.0f) {
        return LIBOPENRTX_EPARAM;
    }
    g_mic_gain = gain;
    return LIBOPENRTX_OK;
}

float libopenrtx_get_mic_gain(void)
{
    return g_mic_gain;
}

int libopenrtx_set_ctcss(uint32_t hz)
{
    g_ctcss = hz;
    if (hz > 0) {
        printf("[libopenrtx] CTCSS set to %u Hz\n", hz);
    } else {
        printf("[libopenrtx] CTCSS disabled\n");
    }
    return LIBOPENRTX_OK;
}

uint32_t libopenrtx_get_ctcss(void)
{
    return g_ctcss;
}
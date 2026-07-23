/**
 * @file channel.c
 * @brief Channel memory functions
 */

#include "libopenrtx.h"
#include <string.h>

// Simulated channel data - using explicit initialization
static libopenrtx_channel_t g_channels[5];

__attribute__((constructor))
static void init_channels(void) {
    // Channel 0
    strncpy(g_channels[0].name, "145.500", MAX_CHANNEL_NAME);
    g_channels[0].rx_frequency = 145500000;
    g_channels[0].tx_frequency = 145500000;
    g_channels[0].mode = MODE_FM;
    g_channels[0].ctcss = 0;
    g_channels[0].rx_enabled = true;
    g_channels[0].tx_enabled = true;
    
    // Channel 1
    strncpy(g_channels[1].name, "145.525", MAX_CHANNEL_NAME);
    g_channels[1].rx_frequency = 145525000;
    g_channels[1].tx_frequency = 145525000;
    g_channels[1].mode = MODE_FM;
    g_channels[1].ctcss = 0;
    g_channels[1].rx_enabled = true;
    g_channels[1].tx_enabled = true;
    
    // Channel 2
    strncpy(g_channels[2].name, "145.550", MAX_CHANNEL_NAME);
    g_channels[2].rx_frequency = 145550000;
    g_channels[2].tx_frequency = 145550000;
    g_channels[2].mode = MODE_FM;
    g_channels[2].ctcss = 0;
    g_channels[2].rx_enabled = true;
    g_channels[2].tx_enabled = true;
    
    // Channel 3
    strncpy(g_channels[3].name, "144.800", MAX_CHANNEL_NAME);
    g_channels[3].rx_frequency = 144800000;
    g_channels[3].tx_frequency = 144800000;
    g_channels[3].mode = MODE_FM;
    g_channels[3].ctcss = 0;
    g_channels[3].rx_enabled = true;
    g_channels[3].tx_enabled = true;
    
    // Channel 4 (M17)
    strncpy(g_channels[4].name, "M17-C4FM", MAX_CHANNEL_NAME);
    g_channels[4].rx_frequency = 433400000;
    g_channels[4].tx_frequency = 433400000;
    g_channels[4].mode = MODE_M17;
    g_channels[4].ctcss = 0;
    g_channels[4].rx_enabled = true;
    g_channels[4].tx_enabled = true;
}

static int g_selected_channel = 0;

int libopenrtx_channel_get(int index, libopenrtx_channel_t* channel)
{
    if (index < 0 || index >= libopenrtx_channel_count()) {
        return LIBOPENRTX_EPARAM;
    }
    
    if (channel == NULL) {
        return LIBOPENRTX_EPARAM;
    }
    
    memcpy(channel, &g_channels[index], sizeof(libopenrtx_channel_t));
    return LIBOPENRTX_OK;
}

int libopenrtx_channel_count(void)
{
    return 5;  // Number of predefined channels
}

int libopenrtx_channel_select(int index)
{
    if (index < 0 || index >= libopenrtx_channel_count()) {
        return LIBOPENRTX_EPARAM;
    }
    
    g_selected_channel = index;
    
    // Auto-tune to channel frequency
    libopenrtx_set_frequency(g_channels[index].rx_frequency);
    libopenrtx_set_mode(g_channels[index].mode);
    
    return LIBOPENRTX_OK;
}

int libopenrtx_channel_get_selected(void)
{
    return g_selected_channel;
}
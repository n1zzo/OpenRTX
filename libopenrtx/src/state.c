/**
 * @file state.c
 * @brief State monitoring functions
 */

#include "libopenrtx.h"

float libopenrtx_get_rssi(void)
{
    // TODO: Return actual RSSI from OpenRTX
    // For now, return simulated value
    return -87.5f;
}

float libopenrtx_get_battery(void)
{
    // TODO: Return actual battery from OpenRTX
    return 8.2f;
}

int libopenrtx_get_battery_percent(void)
{
    float voltage = libopenrtx_get_battery();
    // Simple 2S LiPo calculation (6.0V - 8.4V range)
    float percent = ((voltage - 6.0f) / 2.4f) * 100.0f;
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    return (int)percent;
}

bool libopenrtx_get_squelch_open(void)
{
    // TODO: Return actual squelch state
    float rssi = libopenrtx_get_rssi();
    return rssi > -100.0f;
}
/**
 * @file init.c
 * @brief libopenrtx initialization and lifecycle
 */

#include "libopenrtx.h"
#include "state.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initialization flag
static bool g_initialized = false;

// Shared state (defined here, declared extern in state.h)
uint32_t g_frequency = 145500000;  // Default to 2m ham band
int g_mode = MODE_FM;
bool g_ptt = false;
float g_volume = 0.8f;
float g_mic_gain = 0.5f;
uint32_t g_ctcss = 0;
float g_battery = 8.2f;

int libopenrtx_init(void)
{
    if (g_initialized) {
        return LIBOPENRTX_OK;
    }
    
    printf("libopenrtx v%d.%d.%d initializing...\n",
           LIBOPENRTX_VERSION_MAJOR,
           LIBOPENRTX_VERSION_MINOR,
           LIBOPENRTX_VERSION_PATCH);
    
    // TODO: Initialize OpenRTX core subsystems
    // This would call OpenRTX's init functions
    
    g_initialized = true;
    return LIBOPENRTX_OK;
}

void libopenrtx_terminate(void)
{
    if (!g_initialized) {
        return;
    }
    
    printf("libopenrtx terminating...\n");
    
    // TODO: Cleanup OpenRTX resources
    
    g_initialized = false;
}

bool libopenrtx_is_initialized(void)
{
    return g_initialized;
}
/**
 * @file state.h
 * @brief Internal shared state
 */

#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

// Shared state variables
extern uint32_t g_frequency;
extern int g_mode;
extern bool g_ptt;
extern float g_volume;
extern float g_mic_gain;
extern uint32_t g_ctcss;
extern float g_battery;

#endif // STATE_H
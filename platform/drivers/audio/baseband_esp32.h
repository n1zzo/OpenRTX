/***************************************************************************
 *   Copyright (C) 2026 by Niccolò Izzo IU2KIN                            *
 *                                                                         *
 *   M17 baseband output using ESP32-S3 I2S with GDMA                     *
 ***************************************************************************/

#ifndef BASEBAND_ESP32_H
#define BASEBAND_ESP32_H

#include <stdint.h>
#include <stddef.h>

/**
 * Initialize M17 baseband output at 48 kHz on GPIO18
 * Uses I2S with GDMA for continuous streaming
 * @return 0 on success, negative error code on failure
 */
int baseband_init(void);

/**
 * Start baseband output transmission
 * @return 0 on success, negative error code on failure
 */
int baseband_start(void);

/**
 * Stop baseband output transmission
 * @return 0 on success, negative error code on failure
 */
int baseband_stop(void);

/**
 * Write baseband samples to output (4FSK at 48 kHz)
 * Uses GDMA for continuous streaming with ping-pong buffers
 * @param samples: pointer to 16-bit PCM samples (4FSK levels)
 * @param count: number of samples to write
 * @return number of samples written or negative error code
 */
int baseband_write(const int16_t *samples, size_t count);

/**
 * Get idle buffer for direct writing by M17Modulator
 * Allows zero-copy operation with DMA buffers
 * @return pointer to idle buffer
 */
int16_t *baseband_get_buffer(void);

/**
 * Get size of DMA buffer in samples
 * @return buffer size (960 samples = 20ms at 48 kHz)
 */
size_t baseband_get_buffer_size(void);

#endif // BASEBAND_ESP32_H

/*
    Sensor Smoother
    Author: Brian Khuu (2025)

    # MIT License

    Copyright (c) 2025 Brian Khuu

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */
#ifndef SENSOR_SMOOTHER_H
#define SENSOR_SMOOTHER_H
#include <stddef.h>

#define SENSOR_SMOOTHER_VERSION_MAJOR 0
#define SENSOR_SMOOTHER_VERSION_MINOR 2
#define SENSOR_SMOOTHER_VERSION_PATCH 1
#define SENSOR_SMOOTHER_VERSION "0.2.1"

// Structure to hold the buffer for simple moving average calculation
typedef struct
{
    size_t write_index;  // Index of the next position in the buffer to write to
    size_t buffer_count; // Number of values currently in the buffer
    size_t buffer_size;  // Max number of values the buffer can hold
    float *buffer;       // Pointer to the buffer holding float values
} sensor_smoother_simple_moving_average_t;

// Resets SMA state without clearing the buffer allocation.
#define SENSOR_SMOOTHER_SMA_RESET(state)                                                                                                                                                               \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        (state)->write_index = 0;                                                                                                                                                                      \
        (state)->buffer_count = 0;                                                                                                                                                                     \
    } while (0)

// Structure to hold the state for exponential moving average calculation
typedef struct
{
    int init;          // Flag: 0 = not initialized, 1 = initialized
    float alpha;       // EMA alpha, 0 < alpha < 1 (DSP convention: smaller = more smoothing, e.g. 0.1 heavy, 0.9 light)
    float last_output; // Most recent output value
} sensor_smoother_exponential_moving_average_t;

// Resets EMA state so the next input seeds the filter from scratch.
#define SENSOR_SMOOTHER_EMA_RESET(state)                                                                                                                                                               \
    do                                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        (state)->init = 0;                                                                                                                                                                             \
    } while (0)

// Helper: convert an intuitive smoothing level to the DSP alpha parameter.
// smoothing=0.0 → no smoothing (alpha=1.0), smoothing=0.9 → heavy smoothing (alpha=0.1).
#define SENSOR_SMOOTHER_EMA_ALPHA(smoothing) (1.0f - (smoothing))

/**
 * Calculate the simple moving average (SMA) of a sequence of values.
 *
 * This function maintains a circular buffer of recent values and returns their average.
 * If the buffer is uninitialized or its size is 0, the input value is returned unchanged.
 *
 * @param state        Pointer to the simple moving average state structure.
 *                     Must be pre-initialized with buffer and size.
 * @param input_value  New sensor/sample value to add.
 *
 * @return The current simple moving average value.
 */
float sensor_smoother_simple_moving_average(sensor_smoother_simple_moving_average_t *state, float input_value);

/**
 * Calculate the exponential moving average (EMA) of a sequence of values.
 *
 * This function applies exponential smoothing using a user-provided alpha.
 * On first call (init == 0), it seeds the filter with the input value.
 * If alpha is not a finite value strictly between 0 and 1, the input value is returned unchanged.
 *
 * @param state        Pointer to the exponential moving average state structure.
 *                     Must be pre-initialized with a valid alpha.
 * @param input_value  New sensor/sample value to include.
 *
 * @return The current exponential moving average value.
 */
float sensor_smoother_exponential_moving_average(sensor_smoother_exponential_moving_average_t *state, float input_value);

#endif // SENSOR_SMOOTHER_H

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

#define SENSOR_SMOOTHER_SIMPLE_MOVING_AVERAGE_BUFFER_COUNT 10

// Structure to hold the buffer for simple average calculation
typedef struct
{
    unsigned int write_index; // Index of the next position in the buffer to write to
    unsigned int buffer_count; // Number of values stored in the buffer
    unsigned int buffer_size; // Max number of values stored in the buffer
    float *buffer; // Buffer to store values for simple average calculation
} sensor_smoother_simple_moving_average_t;

// Structure to hold the state for exponential moving average calculation
typedef struct
{
    bool init; // Flag to indicate if the first value has been added
    float lastOutput; // The latest calculated output value
    float alpha; // Smoothing factor (0 <= alpha < 1) used in exponential moving average formula
} sensor_smoother_exponential_moving_average_t;

/**
 * Calculate the simple average of a set of values over a window.
 *
 * This function uses a buffer to store the most recent values and calculates the average of these values.
 *
 * @param state  Pointer to the state structure holding the current values and index.
 * @param input_value  The new value to add to the buffer.
 *
 * @return The calculated simple average value.
 */
float sensor_smoother_simple_moving_average(sensor_smoother_simple_moving_average_t *state, float input_value);

/**
 * Calculate the exponential moving average of a set of values.
 *
 * This function uses the exponential moving average formula to calculate the latest output value based on the most recent input value and previous output values.
 *
 * @param state  Pointer to the state structure holding the current values and index.
 * @param input_value  The new value to add to the buffer.
 *
 * @return The calculated exponential moving average value.
 */
float sensor_smoother_exponential_moving_average(sensor_smoother_exponential_moving_average_t *state, float input_value);

#endif  // SENSOR_SMOOTHER_H
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
#include <stddef.h>
#include "sensor_smoother.h"

float sensor_smoother_simple_moving_average(sensor_smoother_simple_moving_average_t *state, float input_value)
{
    // Ref: https://en.wikipedia.org/wiki/Moving_average

#ifndef DISABLE_SENSOR_SMOOTHER_SIMPLE_MOVING_AVERAGE_CHECKS
    if ((state->buffer == NULL) || (state->buffer_size == 0))
    {
        // Missing Buffer
        return input_value;
    }
#endif

    // Add the new value to buffer
    state->buffer[state->write_index] = input_value;

    // Move to the next position in the buffer, wrapping around if necessary
    state->write_index = (state->write_index + 1) % state->buffer_size;

    // Check if buffer is not full and increment if so
    if (state->buffer_count < state->buffer_size)
    {
        state->buffer_count++;
    }

    // Calculate the average of all values in the buffer
    float avg = 0.0f;
    for (size_t i = 0; i < state->buffer_count; i++)
    {
        avg += state->buffer[i];
    }
    avg = avg / state->buffer_count;

    // Return the calculated simple average value
    return avg;
}

float sensor_smoother_exponential_moving_average(sensor_smoother_exponential_moving_average_t *state, float input_value)
{
    // Ref: https://en.wikipedia.org/wiki/Exponential_smoothing

#ifndef DISABLE_SENSOR_SMOOTHER_EXPONENTIAL_MOVING_AVERAGE_CHECKS
    if (!(state->alpha > 0.0f && state->alpha < 1.0f))
    {
        return input_value;
    }
#endif

    // If this is the first time we're calculating the average, initialize the last output value
    if (!state->init)
    {
        state->last_output = input_value;
        state->init = 1;
    }

    // Update the last output value using the exponential moving average formula
    state->last_output = input_value * state->alpha + state->last_output * (1.0f - state->alpha);

    return state->last_output;
}

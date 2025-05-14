#include <stdbool.h>
#include "sensor_smoother.h"

float sensor_smoother_array_buffered_simple_average(sensor_smoother_array_buffered_simple_average_t *state, float input_value)
{
    // If this is the first time we're calculating the average, initialize the buffer
    if (!state->init)
    {
        // Initialize all values in the buffer to the input value
        for (int i = 0; i < SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT; i++)
        {
            state->buffer[i] = input_value;
        }

        // Mark that the buffer has been initialized
        state->init = true;
    }

    // Add the new value to the end of the buffer
    state->buffer[state->write_index] = input_value;

    // Move to the next position in the buffer, wrapping around if necessary
    state->write_index = (state->write_index + 1) % SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT;

    // Calculate the average of all values in the buffer
    float avg = 0.0f;
    for (int i = 0; i < SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT; i++)
    {
        avg += state->buffer[i];
    }

    // Return the calculated simple average value
    return avg / SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT;
}

float sensor_smoother_exponential_moving_average(sensor_smoother_exponential_moving_average_t *state, float input_value)
{
    // If this is the first time we're calculating the average, initialize the last output value
    if (!state->init)
    {
        state->lastOutput = input_value;
        state->init = true;
    }

    // Update the last output value using the exponential moving average formula
    state->lastOutput = input_value * state->alpha + state->lastOutput * (1.0 - state->alpha);

    return state->lastOutput;
}
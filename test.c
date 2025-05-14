#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sensor_smoother.h"

int main()
{
    // Create instances of the state structures for simple average and exponential moving average calculations
    sensor_smoother_simple_moving_average_t simple_state = {0};
    sensor_smoother_exponential_moving_average_t exponential_state = {0};

    exponential_state.alpha = 0.1;

    // Calculate the simple average of a set of values over a window
    for (int i = 0; i < 10; i++)
    {
        float input_value = i * 2.0f; // Input value to add to the buffer
        printf("Simple Average: %f\n", sensor_smoother_simple_moving_average(&simple_state, input_value));
    }

    // Calculate the exponential moving average of a set of values
    for (int i = 0; i < 10; i++)
    {
        float input_value = i * 2.0f; // Input value to add to the buffer
        printf("Exponential Moving Average: %f\n", sensor_smoother_exponential_moving_average(&exponential_state, input_value));
    }

    return 0;
}
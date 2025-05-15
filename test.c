#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "sensor_smoother.h"

int main()
{
    int num_steps = 1000;

    sensor_smoother_simple_moving_average_t simple_state = {0};
    sensor_smoother_exponential_moving_average_t exponential_state = {0};

    exponential_state.alpha = 0.1;

    double input_values[num_steps];

    // Generate sine wave with random noise
    for (int i = 0; i < num_steps; i++)
    {
        double input_value = sin((double)(i/20.0) / (3.14)); // Generate a sine value
        double noise = (((double)rand() / (double)RAND_MAX) * 0.2) - 0.1;    // Add random noise between -0.1 and 0.1
        input_values[i] = input_value + noise;
    }

    FILE *file_csv = fopen("test.csv", "w+");

    if (file_csv == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    fprintf(file_csv, "Time (ms),Input,Simple Moving Average,Exponential Moving Average\n");

    for (int i = 0; i < num_steps; i++)
    {
        float sma = sensor_smoother_simple_moving_average(&simple_state, input_values[i]);
        float ema = sensor_smoother_exponential_moving_average(&exponential_state, input_values[i]);

        fprintf(file_csv, "%u,%f,%f,%f\n", i, input_values[i], sma, ema);
    }

    fclose(file_csv);

    return 0;
}

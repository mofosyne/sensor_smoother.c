#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "sensor_smoother.h"

#define NUM_STEPS 1000

int main() {
    sensor_smoother_simple_moving_average_t simple_state;
    sensor_smoother_exponential_moving_average_t exponential_state;

    // Set alpha value for exponential moving average (smoothing factor)
    exponential_state.alpha = 0.1;

    double input_values[NUM_STEPS] = {0};

    FILE *file_csv = fopen("test.csv", "w+");

    if (file_csv == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    // Write header row to CSV file
    fprintf(file_csv, "Time (ms),Input,SMA,EMA\n");

    for (int i = 0; i < NUM_STEPS; i++) {
        double sine_value = sin((double)i / 20.0 / 3.14);
        double noise = ((double)rand() / RAND_MAX * 0.2) - 0.1;
        input_values[i] = sine_value + noise;

        float sma = sensor_smoother_simple_moving_average(&simple_state, input_values[i]);
        float ema = sensor_smoother_exponential_moving_average(&exponential_state, input_values[i]);

        // Write row to CSV file with time and averages values
        fprintf(file_csv, "%u,%f,%f,%f\n", i, input_values[i], sma, ema);
    }

    fclose(file_csv);

    return 0;
}
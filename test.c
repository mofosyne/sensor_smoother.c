/*
    Sensor Smoother (Test Program)
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
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "sensor_smoother.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NUM_STEPS 200

// Sum of absolute differences between consecutive samples — lower means smoother.
static float total_variation(const float *data, int n)
{
    float tv = 0.0f;
    for (int i = 1; i < n; i++)
        tv += fabsf(data[i] - data[i - 1]);
    return tv;
}

// Average squared difference between two signals.
static float mean_squared_error(const float *a, const float *b, int n)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
    {
        float diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum / n;
}

int main(void)
{
    sensor_smoother_simple_moving_average_t simple_state = {0};
    float buffer[10] = {0};
    simple_state.buffer = buffer;
    simple_state.buffer_size = sizeof(buffer) / sizeof(buffer[0]);

    sensor_smoother_exponential_moving_average_t exponential_state = {0};
    exponential_state.alpha = 0.1f;

    float clean_values[NUM_STEPS];
    float input_values[NUM_STEPS];
    float sma_values[NUM_STEPS];
    float ema_values[NUM_STEPS];

    // Generate clean signal and noisy input (fixed seed = deterministic)
    for (int i = 0; i < NUM_STEPS; i++)
    {
        clean_values[i] = (float)sin(i / 8.0 / M_PI);
        double noise    = ((double)rand() / RAND_MAX * 0.2) - 0.1;
        input_values[i] = clean_values[i] + (float)noise;
    }

    // Run filters
    for (int i = 0; i < NUM_STEPS; i++)
    {
        sma_values[i] = sensor_smoother_simple_moving_average(&simple_state, input_values[i]);
        ema_values[i] = sensor_smoother_exponential_moving_average(&exponential_state, input_values[i]);
    }

    // Write CSV
    FILE *file_csv = fopen("test.csv", "w+");
    if (file_csv == NULL)
    {
        fprintf(stderr, "Could not open test.csv\n");
        return 1;
    }
    fprintf(file_csv, "Time (ms),Input,SMA,EMA\n");
    for (int i = 0; i < NUM_STEPS; i++)
        fprintf(file_csv, "%d,%f,%f,%f\n", i, input_values[i], sma_values[i], ema_values[i]);
    fclose(file_csv);

    // Compute statistics
    float tv_input = total_variation(input_values, NUM_STEPS);
    float tv_sma   = total_variation(sma_values,   NUM_STEPS);
    float tv_ema   = total_variation(ema_values,   NUM_STEPS);

    float mse_input = mean_squared_error(input_values, clean_values, NUM_STEPS);
    float mse_sma   = mean_squared_error(sma_values,   clean_values, NUM_STEPS);
    float mse_ema   = mean_squared_error(ema_values,   clean_values, NUM_STEPS);

    float sma_tv_reduction = (1.0f - tv_sma / tv_input) * 100.0f;
    float ema_tv_reduction = (1.0f - tv_ema / tv_input) * 100.0f;

    // Both filters must reduce signal roughness (total variation)
    assert(tv_sma < tv_input && "SMA did not reduce total variation");
    assert(tv_ema < tv_input && "EMA did not reduce total variation");

    printf("All assertions passed.\n");
    printf("  SMA (window=10): TV reduction %.1f%%, MSE vs clean %.5f\n", sma_tv_reduction, mse_sma);
    printf("  EMA (alpha=0.1): TV reduction %.1f%%, MSE vs clean %.5f\n", ema_tv_reduction, mse_ema);
    printf("  Raw noisy input: MSE vs clean %.5f\n", mse_input);

    // Write stats file for readme_update
    FILE *file_stats = fopen("test_stats.txt", "w+");
    if (file_stats == NULL)
    {
        fprintf(stderr, "Could not open test_stats.txt\n");
        return 1;
    }
    fprintf(file_stats, "sma-tv-reduction=%.1f%%\n", sma_tv_reduction);
    fprintf(file_stats, "ema-tv-reduction=%.1f%%\n", ema_tv_reduction);
    fprintf(file_stats, "sma-mse=%.5f\n", mse_sma);
    fprintf(file_stats, "ema-mse=%.5f\n", mse_ema);
    fprintf(file_stats, "input-mse=%.5f\n", mse_input);
    fclose(file_stats);

    return 0;
}

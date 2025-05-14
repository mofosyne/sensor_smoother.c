#ifndef SENSOR_SMOOTHER_H
#define SENSOR_SMOOTHER_H

#define SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT 10

// Structure to hold the buffer for simple average calculation
typedef struct
{
    bool init; // Flag to indicate if the buffer has been initialized
    unsigned int write_index; // Index of the next position in the buffer to write to
    float buffer[SENSOR_SMOOTHER_SIMPLE_AVERAGE_BUFFER_COUNT]; // Buffer to store values for simple average calculation
} sensor_smoother_array_buffered_simple_average_t;

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
float sensor_smoother_array_buffered_simple_average(sensor_smoother_array_buffered_simple_average_t *state, float input_value);

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
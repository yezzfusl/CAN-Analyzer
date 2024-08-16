#include "can_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_TEMP 150
#define MAX_SPEED 300
#define BATTERY_VOLTAGE_FACTOR 0.1
#define RPM_FACTOR 10
#define ACCELERATION_FACTOR 0.01
#define TIRE_PRESSURE_FACTOR 0.1
#define OIL_PRESSURE_FACTOR 0.1
#define FUEL_CONSUMPTION_FACTOR 0.1

// Function prototypes
double calculate_moving_average(double *values, int window_size);
int detect_anomaly(double value, double mean, double std_dev);
double calculate_standard_deviation(double *values, int count);

// Circular buffer for storing historical values
#define HISTORY_SIZE 100
double temperature_history[HISTORY_SIZE] = {0};
double speed_history[HISTORY_SIZE] = {0};
int history_index = 0;

void decode_can_message(const struct can_frame *frame) {
    printf("Decoded message: ");
    
    switch (frame->can_id) {
        case 0x123:
            {
                int temp = frame->data[0];
                printf("Engine Temperature: %d°C\n", temp);
                
                // Store in circular buffer
                temperature_history[history_index] = temp;
                
                // Calculate moving average
                double avg_temp = calculate_moving_average(temperature_history, HISTORY_SIZE);
                printf("Average Temperature (last %d readings): %.2f°C\n", HISTORY_SIZE, avg_temp);
                
                // Anomaly detection
                double std_dev = calculate_standard_deviation(temperature_history, HISTORY_SIZE);
                if (detect_anomaly(temp, avg_temp, std_dev)) {
                    printf("WARNING: Anomalous temperature detected!\n");
                }
                
                // Temperature rate of change
                static double last_temp = 0;
                static time_t last_time = 0;
                time_t current_time = time(NULL);
                if (last_time != 0) {
                    double temp_rate = (temp - last_temp) / difftime(current_time, last_time);
                    printf("Temperature Rate of Change: %.2f°C/s\n", temp_rate);
                }
                last_temp = temp;
                last_time = current_time;
            }
            break;
        case 0x456:
            {
                int speed = frame->data[0];
                printf("Vehicle Speed: %d km/h\n", speed);
                
                // Store in circular buffer
                speed_history[history_index] = speed;
                
                // Calculate moving average
                double avg_speed = calculate_moving_average(speed_history, HISTORY_SIZE);
                printf("Average Speed (last %d readings): %.2f km/h\n", HISTORY_SIZE, avg_speed);
            }
            break;
        case 0x789:
            {
                int battery_voltage = (frame->data[0] << 8 | frame->data[1]) * BATTERY_VOLTAGE_FACTOR;
                printf("Battery Voltage: %.1f V\n", battery_voltage / 10.0);
            }
            break;
        case 0xABC:
            {
                int rpm = (frame->data[0] << 8 | frame->data[1]) * RPM_FACTOR;
                printf("Engine RPM: %d\n", rpm);
            }
            break;
        case 0xDEF:
            {
                int16_t acceleration = (frame->data[0] << 8 | frame->data[1]);
                printf("Vehicle Acceleration: %.2f m/s^2\n", acceleration * ACCELERATION_FACTOR);
            }
            break;
        case 0x321:
            {
                for (int i = 0; i < 4; i++) {
                    printf("Tire Pressure %d: %.1f PSI\n", i+1, frame->data[i] * TIRE_PRESSURE_FACTOR);
                }
            }
            break;
        case 0x654:
            {
                int oil_pressure = (frame->data[0] << 8 | frame->data[1]) * OIL_PRESSURE_FACTOR;
                printf("Oil Pressure: %.1f PSI\n", oil_pressure / 10.0);
            }
            break;
        case 0x987:
            {
                int fuel_consumption = (frame->data[0] << 8 | frame->data[1]) * FUEL_CONSUMPTION_FACTOR;
                printf("Fuel Consumption: %.1f L/100km\n", fuel_consumption / 10.0);
            }
            break;
        default:
            printf("Unknown message type\n");
    }
    
    history_index = (history_index + 1) % HISTORY_SIZE;
}

double calculate_moving_average(double *values, int window_size) {
    double sum = 0;
    for (int i = 0; i < window_size; i++) {
        sum += values[i];
    }
    return sum / window_size;
}

int detect_anomaly(double value, double mean, double std_dev) {
    return fabs(value - mean) > 3 * std_dev;
}

double calculate_standard_deviation(double *values, int count) {
    double sum = 0, mean, std_dev = 0;
    int i;
    for (i = 0; i < count; ++i) {
        sum += values[i];
    }
    mean = sum / count;
    for (i = 0; i < count; ++i) {
        std_dev += pow(values[i] - mean, 2);
    }
    return sqrt(std_dev / count);
}

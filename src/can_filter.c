#include "can_filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILTERS 16
#define SAMPLING_RATE 1000 // Hz
#define VOLTAGE_THRESHOLD 2.5 // Volts
#define CAN_ERROR_FRAME 0x20000000 // Error frame bit in CAN ID

// Advanced filter structure
typedef struct {
    struct can_filter base;
    uint8_t priority;
    uint64_t timestamp;
    double signal_strength;
    int (*custom_filter_func)(const struct can_frame *);
} advanced_can_filter;

// Global filter array
static advanced_can_filter filter_array[MAX_FILTERS];
static int filter_count = 0;

// Function prototypes
static double calculate_signal_strength(const struct can_frame *frame);
static int check_timing_requirements(const struct can_frame *frame, const advanced_can_filter *filter);
static int apply_custom_filter(const struct can_frame *frame, const advanced_can_filter *filter);
static int detect_error_frame(const struct can_frame *frame);

int apply_can_filter(const struct can_frame *frame, const struct can_filter *filter) {
    if (filter_count == 0) {
        printf("Warning: No filters configured\n");
        return 1; // Accept all frames if no filters are set
    }

    for (int i = 0; i < filter_count; i++) {
        const advanced_can_filter *adv_filter = &filter_array[i];
        
        // Basic ID and mask check
        if ((frame->can_id & adv_filter->base.can_mask) != (adv_filter->base.can_id & adv_filter->base.can_mask)) {
            continue;
        }

        // Check signal strength
        double signal_strength = calculate_signal_strength(frame);
        if (signal_strength < adv_filter->signal_strength) {
            printf("Frame rejected due to weak signal strength: %.2f\n", signal_strength);
            continue;
        }

        // Check timing requirements
        if (!check_timing_requirements(frame, adv_filter)) {
            continue;
        }

        // Apply custom filter function if available
        if (!apply_custom_filter(frame, adv_filter)) {
            continue;
        }

        // Check for error frames
        if (detect_error_frame(frame)) {
            printf("Error frame detected\n");
            return 0;
        }

        // All checks passed, accept the frame
        return 1;
    }

    return 0; // Frame doesn't match any filter
}

int add_advanced_filter(struct can_filter *base_filter, uint8_t priority, double min_signal_strength, 
                        int (*custom_func)(const struct can_frame *)) {
    if (filter_count >= MAX_FILTERS) {
        printf("Error: Maximum number of filters reached\n");
        return 0;
    }

    advanced_can_filter *new_filter = &filter_array[filter_count];
    memcpy(&new_filter->base, base_filter, sizeof(struct can_filter));
    new_filter->priority = priority;
    new_filter->timestamp = time(NULL);
    new_filter->signal_strength = min_signal_strength;
    new_filter->custom_filter_func = custom_func;

    filter_count++;
    return 1;
}

static double calculate_signal_strength(const struct can_frame *frame) {
    // Simulating signal strength calculation based on data bytes
    double strength = 0;
    for (int i = 0; i < frame->can_dlc; i++) {
        strength += (double)frame->data[i] / 255.0;
    }
    return strength / frame->can_dlc * 5.0; // Normalize to 0-5V range
}

static int check_timing_requirements(const struct can_frame *frame, const advanced_can_filter *filter) {
    uint64_t current_time = time(NULL);
    double time_diff = difftime(current_time, filter->timestamp);
    
    // Check if the frame arrives within the expected time window
    if (time_diff > 1.0 / SAMPLING_RATE) {
        printf("Frame rejected due to timing requirements\n");
        return 0;
    }
    return 1;
}

static int apply_custom_filter(const struct can_frame *frame, const advanced_can_filter *filter) {
    if (filter->custom_filter_func != NULL) {
        return filter->custom_filter_func(frame);
    }
    return 1; // No custom filter function, so pass
}

static int detect_error_frame(const struct can_frame *frame) {
    return (frame->can_id & CAN_ERROR_FRAME) != 0;
}

void print_filter_stats() {
    printf("Current filter configuration:\n");
    for (int i = 0; i < filter_count; i++) {
        printf("Filter %d: ID=0x%X, Mask=0x%X, Priority=%d, Min Signal Strength=%.2fV\n",
               i, filter_array[i].base.can_id, filter_array[i].base.can_mask,
               filter_array[i].priority, filter_array[i].signal_strength);
    }
}

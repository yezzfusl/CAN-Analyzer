#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "can_interface.h"
#include "can_filter.h"
#include "can_decode.h"
#include "real_time_display.h"
#include "error_handling.h"
#include "statistical_analysis.h"
#include "../include/utils.h"

volatile sig_atomic_t running = 1;

void signal_handler(int signum) {
    (void)signum;
    running = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <CAN interface>\n", argv[0]);
        exit(1);
    }

    const char *ifname = argv[1];
    int socket_fd = can_init(ifname);

    if (socket_fd < 0) {
        handle_error("Failed to initialize CAN interface");
    }

    printf("CAN Analyzer initialized. Listening on %s\n", ifname);

    // Initialize filter (example: only messages with ID 0x123)
    struct can_filter filter;
    filter.can_id = 0x123;
    filter.can_mask = CAN_SFF_MASK;
    set_can_filter(socket_fd, &filter);

    // Set up signal handling
    signal(SIGINT, signal_handler);

    // Initialize real-time display and statistical analysis
    init_real_time_display();
    init_statistical_analysis();

    struct timespec last_update_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &last_update_time);

    while (running) {
        struct can_frame frame;
        int recv_result = can_receive(socket_fd, &frame);

        if (recv_result == 0) {
            if (apply_can_filter(&frame, &filter)) {
                decode_can_message(&frame);
                update_real_time_display(&frame);
                update_statistical_analysis(&frame);
            }
        } else if (recv_result < 0) {
            handle_error("Error receiving CAN frame");
        }

        clock_gettime(CLOCK_MONOTONIC, &current_time);
        if (current_time.tv_sec - last_update_time.tv_sec >= 1) {
            refresh_real_time_display();
            print_statistical_analysis();
            last_update_time = current_time;
        }
    }

    cleanup_real_time_display();
    cleanup_statistical_analysis();
    can_close(socket_fd);
    printf("\nCAN Analyzer shutting down...\n");
    return 0;
}

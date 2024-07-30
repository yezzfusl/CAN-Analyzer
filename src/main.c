#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "can_interface.h"
#include "can_filter.h"
#include "can_decode.h"
#include "../include/utils.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <CAN interface>\n", argv[0]);
        exit(1);
    }

    const char *ifname = argv[1];
    int socket_fd = can_init(ifname);

    if (socket_fd < 0) {
        fprintf(stderr, "Failed to initialize CAN interface\n");
        exit(1);
    }

    printf("CAN Analyzer initialized. Listening on %s\n", ifname);

    // Initialize filter (example: only messages with ID 0x123)
    struct can_filter filter;
    filter.can_id = 0x123;
    filter.can_mask = CAN_SFF_MASK;
    set_can_filter(socket_fd, &filter);

    while (1) {
        struct can_frame frame;
        if (can_receive(socket_fd, &frame) == 0) {
            if (apply_can_filter(&frame, &filter)) {
                print_can_frame(&frame);
                decode_can_message(&frame);
            }
        }
    }

    can_close(socket_fd);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "can_interface.h"
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

    while (1) {
        struct can_frame frame;
        if (can_receive(socket_fd, &frame) == 0) {
            print_can_frame(&frame);
        }
    }

    can_close(socket_fd);
    return 0;
}

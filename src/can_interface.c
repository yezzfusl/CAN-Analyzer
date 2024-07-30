#include "can_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

int can_init(const char *ifname) {
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(socket_fd, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -2;
    }

    return socket_fd;
}

int can_receive(int socket_fd, struct can_frame *frame) {
    int nbytes = read(socket_fd, frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("Error in can_receive");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    return 0;
}

void can_close(int socket_fd) {
    close(socket_fd);
}

#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

#include <linux/can.h>
#include <linux/can/raw.h>

int can_init(const char *ifname);
int can_receive(int socket_fd, struct can_frame *frame);
void can_close(int socket_fd);

#endif

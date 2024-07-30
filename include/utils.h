#ifndef UTILS_H
#define UTILS_H

#include <linux/can.h>

void print_can_frame(const struct can_frame *frame);

#endif

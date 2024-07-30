#ifndef CAN_FILTER_H
#define CAN_FILTER_H

#include <linux/can.h>

int apply_can_filter(const struct can_frame *frame, const struct can_filter *filter);

#endif

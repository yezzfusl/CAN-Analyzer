#ifndef CAN_DECODE_H
#define CAN_DECODE_H

#include <linux/can.h>

void decode_can_message(const struct can_frame *frame);

#endif

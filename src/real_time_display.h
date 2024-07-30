#ifndef REAL_TIME_DISPLAY_H
#define REAL_TIME_DISPLAY_H

#include <linux/can.h>

void init_real_time_display(void);
void update_real_time_display(const struct can_frame *frame);
void refresh_real_time_display(void);
void cleanup_real_time_display(void);

#endif

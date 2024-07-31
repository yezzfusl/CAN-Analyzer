#ifndef GRAPHICAL_DISPLAY_H
#define GRAPHICAL_DISPLAY_H

#include <linux/can.h>

void init_graphical_display(void);
void update_graphical_display(const struct can_frame *frame);
void generate_graphs(void);
void cleanup_graphical_display(void);

#endif

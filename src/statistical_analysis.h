#ifndef STATISTICAL_ANALYSIS_H
#define STATISTICAL_ANALYSIS_H

#include <linux/can.h>

void init_statistical_analysis(void);
void update_statistical_analysis(const struct can_frame *frame);
void print_statistical_analysis(void);
void cleanup_statistical_analysis(void);

#endif

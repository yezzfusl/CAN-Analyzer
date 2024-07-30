#include "can_filter.h"

int apply_can_filter(const struct can_frame *frame, const struct can_filter *filter) {
    return (frame->can_id & filter->can_mask) == (filter->can_id & filter->can_mask);
}

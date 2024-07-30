#include "../include/utils.h"
#include <stdio.h>

void print_can_frame(const struct can_frame *frame) {
    printf("ID: 0x%X [%d] ", frame->can_id, frame->can_dlc);
    
    for (int i = 0; i < frame->can_dlc; i++) {
        printf("%02X ", frame->data[i]);
    }
    printf("\n");
}

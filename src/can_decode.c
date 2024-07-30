#include "can_decode.h"
#include <stdio.h>

void decode_can_message(const struct can_frame *frame) {
    printf("Decoded message: ");
    
    switch (frame->can_id) {
        case 0x123:
            printf("Engine Temperature: %d°C\n", frame->data[0]);
            break;
        case 0x456:
            printf("Vehicle Speed: %d km/h\n", frame->data[0]);
            break;
        default:
            printf("Unknown message type\n");
    }
}

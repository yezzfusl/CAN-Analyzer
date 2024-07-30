#include "real_time_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_MESSAGES 10

struct display_message {
    canid_t id;
    __u8 data[8];
    __u8 dlc;
    time_t timestamp;
};

static struct display_message messages[MAX_MESSAGES];
static int message_count = 0;

void init_real_time_display(void) {
    printf("\033[2J\033[H"); // Clear screen and move cursor to top-left
    printf("Real-time CAN Message Display\n");
    printf("-----------------------------\n");
}

void update_real_time_display(const struct can_frame *frame) {
    time_t now = time(NULL);

    if (message_count < MAX_MESSAGES) {
        messages[message_count].id = frame->can_id;
        memcpy(messages[message_count].data, frame->data, frame->can_dlc);
        messages[message_count].dlc = frame->can_dlc;
        messages[message_count].timestamp = now;
        message_count++;
    } else {
        memmove(&messages[0], &messages[1], sizeof(struct display_message) * (MAX_MESSAGES - 1));
        messages[MAX_MESSAGES - 1].id = frame->can_id;
        memcpy(messages[MAX_MESSAGES - 1].data, frame->data, frame->can_dlc);
        messages[MAX_MESSAGES - 1].dlc = frame->can_dlc;
        messages[MAX_MESSAGES - 1].timestamp = now;
    }
}

void refresh_real_time_display(void) {
    printf("\033[3;1H"); // Move cursor to the third line
    printf("\033[J");    // Clear from cursor to end of screen

    for (int i = 0; i < message_count; i++) {
        char time_str[9];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&messages[i].timestamp));

        printf("%s - ID: 0x%X [%d] ", time_str, messages[i].id, messages[i].dlc);
        for (int j = 0; j < messages[i].dlc; j++) {
            printf("%02X ", messages[i].data[j]);
        }
        printf("\n");
    }

    fflush(stdout);
}

void cleanup_real_time_display(void) {
    printf("\033[2J\033[H"); // Clear screen and move cursor to top-left
}

#include "statistical_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CAN_IDS 4096

struct can_stats {
    unsigned long long count;
    unsigned long long total_dlc;
    time_t first_seen;
    time_t last_seen;
};

static struct can_stats stats[MAX_CAN_IDS];
static unsigned long long total_messages = 0;
static time_t start_time;

void init_statistical_analysis(void) {
    memset(stats, 0, sizeof(stats));
    start_time = time(NULL);
}

void update_statistical_analysis(const struct can_frame *frame) {
    canid_t id = frame->can_id & CAN_SFF_MASK;
    time_t now = time(NULL);

    if (stats[id].count == 0) {
        stats[id].first_seen = now;
    }

    stats[id].count++;
    stats[id].total_dlc += frame->can_dlc;
    stats[id].last_seen = now;

    total_messages++;
}

void print_statistical_analysis(void) {
    time_t now = time(NULL);
    double elapsed_time = difftime(now, start_time);

    printf("\nStatistical Analysis:\n");
    printf("--------------------\n");
    printf("Total messages: %llu\n", total_messages);
    printf("Average message rate: %.2f msg/s\n", total_messages / elapsed_time);
    printf("\nTop 5 message IDs by frequency:\n");

    struct {
        canid_t id;
        unsigned long long count;
    } top_ids[5] = {{0}};

    for (int i = 0; i < MAX_CAN_IDS; i++) {
        if (stats[i].count > 0) {
            for (int j = 0; j < 5; j++) {
                if (stats[i].count > top_ids[j].count) {
                    for (int k = 4; k > j; k--) {
                        top_ids[k] = top_ids[k-1];
                    }
                    top_ids[j].id = i;
                    top_ids[j].count = stats[i].count;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (top_ids[i].count > 0) {
            printf("ID: 0x%03X, Count: %llu, Avg DLC: %.2f\n",
                   top_ids[i].id,
                   top_ids[i].count,
                   (double)stats[top_ids[i].id].total_dlc / top_ids[i].count);
        }
    }
}

void cleanup_statistical_analysis(void) {
    // No dynamic allocations, so nothing to clean up
}

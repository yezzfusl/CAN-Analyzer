#include "graphical_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CAN_IDS 4096
#define DATA_POINTS 60 // Store data for the last 60 seconds

struct can_data_point {
    time_t timestamp;
    int value;
};

static struct can_data_point data[MAX_CAN_IDS][DATA_POINTS];
static int data_index[MAX_CAN_IDS] = {0};

void init_graphical_display(void) {
    memset(data, 0, sizeof(data));
    memset(data_index, 0, sizeof(data_index));
}

void update_graphical_display(const struct can_frame *frame) {
    canid_t id = frame->can_id & CAN_SFF_MASK;
    time_t now = time(NULL);

    // Assume the first byte of data is the value we want to graph
    int value = frame->data[0];

    data[id][data_index[id]].timestamp = now;
    data[id][data_index[id]].value = value;

    data_index[id] = (data_index[id] + 1) % DATA_POINTS;
}

void generate_graphs(void) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        fprintf(stderr, "Error opening gnuplot\n");
        return;
    }

    fprintf(gnuplot, "set term png\n");
    fprintf(gnuplot, "set output 'can_data.png'\n");
    fprintf(gnuplot, "set title 'CAN Data Graph'\n");
    fprintf(gnuplot, "set xlabel 'Time (s)'\n");
    fprintf(gnuplot, "set ylabel 'Value'\n");
    fprintf(gnuplot, "set xdata time\n");
    fprintf(gnuplot, "set timefmt '%%s'\n");
    fprintf(gnuplot, "set format x '%%H:%%M:%%S'\n");

    fprintf(gnuplot, "plot ");

    int plot_count = 0;
    for (int id = 0; id < MAX_CAN_IDS; id++) {
        if (data_index[id] > 0) {
            if (plot_count > 0) {
                fprintf(gnuplot, ", ");
            }
            fprintf(gnuplot, "'-' using 1:2 title '0x%03X' with lines", id);
            plot_count++;
        }
    }
    fprintf(gnuplot, "\n");

    for (int id = 0; id < MAX_CAN_IDS; id++) {
        if (data_index[id] > 0) {
            for (int i = 0; i < DATA_POINTS; i++) {
                if (data[id][i].timestamp != 0) {
                    fprintf(gnuplot, "%ld %d\n", data[id][i].timestamp, data[id][i].value);
                }
            }
            fprintf(gnuplot, "e\n");
        }
    }

    fflush(gnuplot);
    pclose(gnuplot);

    printf("Graph generated: can_data.png\n");
}

void cleanup_graphical_display(void) {
    // No dynamic allocations, so nothing to clean up
}

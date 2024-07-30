#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>

void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

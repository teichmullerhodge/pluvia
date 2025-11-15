#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../definitions/definitions.h"

void generate_log_id(char *buffer, size_t buffer_size) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    u64 id = (u64)ts.tv_sec * 1000000000 + ts.tv_nsec;
    id = (id << 32) | getpid();
    snprintf(buffer, buffer_size, "@%lu", id);
}


static void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}


static void log_message(const char *level, const char *fg, const char *bg, const char *message) {
    char id[256];
    char timestamp[64];
    generate_log_id(id, sizeof(id));
    get_timestamp(timestamp, sizeof(timestamp));

    fprintf(stdout, "%s%s%s[%s] %s %s%s%s %s%s\n",
            bg, fg, BOLD, level, timestamp, id, RESET,       
            fg, message, RESET);             
}
void logger_debug(const char *message) {
    log_message("DEBUG", FG_WHITE, BG_BLUE, message);
}

void logger_success(const char *message) {
    log_message("OK", FG_WHITE, BG_GREEN, message);
}

void logger_warning(const char *message) {
    log_message("WARN", FG_WHITE, BG_YELLOW, message);
}

void logger_error(const char *message) {
    log_message("ERROR", FG_WHITE, BG_RED, message);
}

void logger_fatal(const char *message) {
    log_message("FATAL", FG_WHITE, BG_MAGENTA, message);
    exit(-1);
}

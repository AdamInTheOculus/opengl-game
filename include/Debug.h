#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifndef DEBUG
    #define DEBUG 0
#endif

#define GREEN(string) "\x1b[32m" string "\x1b[0m"
#define YELLOW(string) "\x1b[33m" string "\x1b[0m"
#define RED(string) "\x1b[31m" string "\x1b[0m"

#define DEBUG_LOG(format, ...) \
    if (DEBUG) { \
        fprintf(stdout, "LOG | %s | Line %d | %s | ", __FILE__, __LINE__, __FUNCTION__); \
        fprintf(stdout, format, ##__VA_ARGS__); \
    }

#define DEBUG_SUCCESS(format, ...) \
    if (DEBUG) { \
        fprintf(stdout, GREEN("SUCCESS | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        fprintf(stdout, GREEN(format), ##__VA_ARGS__); \
    }

#define DEBUG_WARN(format, ...) \
    if (DEBUG) { \
        fprintf(stdout, YELLOW("WARN | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        fprintf(stdout, YELLOW(format), ##__VA_ARGS__); \
    }

#define DEBUG_ERROR(format, ...) \
    if(DEBUG) { \
        fprintf(stderr, RED("ERROR | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        fprintf(stderr, RED(format), ##__VA_ARGS__); \
    }

#endif
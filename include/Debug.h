#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifndef DEBUG
    #define DEBUG 1
#endif

#ifndef VERBOSE
    #define VERBOSE 0
#endif

#define LOG(string) string "\n"
#define GREEN(string) "\x1b[32m" string "\x1b[0m\n"
#define YELLOW(string) "\x1b[33m" string "\x1b[0m\n"
#define RED(string) "\x1b[31m" string "\x1b[0m\n"

#define DEBUG_LOG(format, ...) \
    if (DEBUG) { \
        if(VERBOSE) { \
            fprintf(stdout, "LOG | %s | Line %d | %s | ", __FILE__, __LINE__, __FUNCTION__); \
        } \
        fprintf(stdout, format, ##__VA_ARGS__); \
        fprintf(stdout, "\n"); \
    }

#define DEBUG_SUCCESS(format, ...) \
    if (DEBUG) { \
        if(VERBOSE) { \
        fprintf(stdout, GREEN("SUCCESS | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        } \
        fprintf(stdout, GREEN(format), ##__VA_ARGS__); \
    }

#define DEBUG_WARN(format, ...) \
    if (DEBUG) { \
        if(VERBOSE) { \
        fprintf(stdout, YELLOW("WARN | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        } \
        fprintf(stdout, YELLOW(format), ##__VA_ARGS__); \
    }

#define DEBUG_ERROR(format, ...) \
    if(DEBUG) { \
        if(VERBOSE) { \
        fprintf(stderr, RED("ERROR | %s | Line %d | %s | "), __FILE__, __LINE__, __FUNCTION__); \
        } \
        fprintf(stderr, RED(format), ##__VA_ARGS__); \
    }

#endif
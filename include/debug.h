#pragma once

#define DEBUG_LOG(format, ...) \
    if (DEBUG) { \
        fprintf(stdout, "%s :: %s :: Line %d :: ", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stdout, format, ##__VA_ARGS__); \
    }

#define DEBUG_ERROR(format, ...) \
    if(DEBUG) { \
        fprintf(stderr, "%s :: %s :: Line %d :: ", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, format, ##__VA_ARGS__); \
    }
#pragma once
// #include <crtdbg.h>
#include "Deffinitions.h"


/**
 * 
 */
#define LOG(stream, ...) fprintf(stream, __VA_ARGS__)

/**
 * 
 */
#define LOG_ERROR(...) LOG(stderr, __VA_ARGS__)

/**
 * 
 */
#define PRINT_ERROR_LOADING(format, ptr) LOG_ERROR(format, ptr)

/**
 * check condition isboolean return 
 */ 
#define CHECK(condition, msg)       \
    if (!(condition))               \
    {                               \
        LOG_ERROR(msg);             \
        return false;               \
    }


/** 
 * tick function for wait for next frame 
 */
#define WAIT_FOR_NEXT_FRAME(ticksLastFrame, FRAME_TIME_LENGTH) \
     while (!SDL_TICKS_PASSED(SDL_GetTicks(), (ticksLastFrame) + (FRAME_TIME_LENGTH)))

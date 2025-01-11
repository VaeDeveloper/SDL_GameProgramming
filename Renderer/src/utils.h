#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
/** logging stream */
#define LOG(stream, ...) fprintf(stream, __VA_ARGS__)

/** error log stream */
#define LOG_ERROR(...) LOG(stderr, __VA_ARGS__)

/** check condition is boolean return */ 
#define check(condition, msg)       \
    if (!(condition))               \
    {                               \
        LOG_ERROR(msg);             \
        return false;               \
    }


/** tick function for wait for next frame */
#define WAIT_FOR_NEXT_FRAME(ticksLastFrame, FRAME_TIME_LENGTH) \
     while (!SDL_TICKS_PASSED(SDL_GetTicks(), (ticksLastFrame) + (FRAME_TIME_LENGTH)))


#endif
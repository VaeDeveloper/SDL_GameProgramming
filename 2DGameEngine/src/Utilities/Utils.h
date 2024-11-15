#ifndef UTILS_H
#define UTILS_H

#include "../Logger/Logger.h"

/**
 * check condition isboolean return 
 */ 
#define check(condition, msg)       \
    if (!(condition))               \
    {                               \
        Logger::Err(msg);           \
        return;               \
    }

#endif
#ifndef UTILS_H
#define UTILS_H

#include "../Logger/Logger.h"



namespace GEngine
{
    namespace Math
    {
        int randomInt(int min, int max);
    }
}


/**
 * check condition isboolean return 
 */
#define check(condition, msg)       \
    if (!(condition))               \
    {                               \
        Logger::Err(msg);           \
        return;                     \
    }




#endif
#ifndef MACRO_H
#define MACRO_H



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

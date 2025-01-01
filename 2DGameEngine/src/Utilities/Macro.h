#ifndef MACRO_H
#define MACRO_H


#if ASSERTIONS_ENABLED
#define debugBreak() asm { int 3 }
#define ASSERT(expr) \
    if (expr) { } \
    else \
    { \
        reportAssertionFailure(#expr, \
            __FILE__, __LINE__); \
        debugBreak(); \
    } \
#else

#define ASSERT(expr) 

#endif



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

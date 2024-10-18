#ifndef MACRO_H_
#define MACRO_H_

#include <assert.h>

#define SUCCESS 0
#define FAILED -1

#define ASSERT(cond) assert(cond)

#define CHECK_RESULT_RETURN(result) \
    if (result != SUCCESS)          \
    {                               \
        return (FAILED);            \
    }

#define CHECK_RESULT_GOTO(result, label) \
    if (result != SUCCESS)               \
    {                                    \
        goto label;                      \
    }

#define ASSERT(cond) assert(cond)
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

#define BIT(n) (0x1 << (n))
#define MASK(n) (BIT(n) - 1)

#ifdef CPU_MIMXRT1176DVMAA_cm7
#define MACH_NOP() asm("nop; nop;") // CM7 has dual issue capability enabled
#else
#define MACH_NOP() asm("nop;")
#endif

#define NOP_10()    \
    do              \
    {               \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
        MACH_NOP(); \
    } while (0)
#define NOP_100() \
    do            \
    {             \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
        NOP_10(); \
    } while (0)

#if defined(__ADSPSHARC__)
#define AUDIO_BUFFER_SECTION dm
#define TEST_CONST_SECTION
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define AUDIO_BUFFER_SECTION __attribute__((section("DataQuickAccess")))
#define TEST_CONST_SECTION __attribute__((section("NonCacheable")))
#else
#define AUDIO_BUFFER_SECTION
#define TEST_CONST_SECTION
#endif

#endif /* MACRO_H_ */

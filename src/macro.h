#ifndef MACRO_H_
#define MACRO_H_

#include <assert.h>

#define SUCCESS 0
#define FAILED -1

#define ASSERT(cond)    assert(cond)

#define CHECK_RESULT_RETURN(result) \
    if (result != SUCCESS)\
    { \
        return (FAILED); \
    }

#define CHECK_RESULT_GOTO(result, label) \
    if (result != SUCCESS)\
    { \
        goto label; \
    }

#define ASSERT(cond)    assert(cond)
#define ARRAYSIZE(x)    (sizeof(x) / sizeof(x[0]))

#define BIT(n)          (0x1 << (n))
#define MASK(n)         (BIT(n) - 1)

#endif /* MACRO_H_ */

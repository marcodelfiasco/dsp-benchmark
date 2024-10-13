// Target initialization

#include <stdint.h>

extern void tp_init(void);

void target_init(void)
{
    tp_init();
}

uint64_t get_timestamp(void)
{
    // TODO
    return 0;
}

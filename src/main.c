// Main

#include "log.h"
#include "measure.h"
#include "test.h"

extern void target_init();

void main(void)
{
    target_init();

    log_msg("Hello World\n");

    test_nop_100();
    test_nop_1000();
}

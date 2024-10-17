// Main

#include "log.h"
#include "measure.h"
#include "test.h"

extern void target_init();

void main(void)
{
    target_init();

    log_msg("Executing tests\n");

    test_init();
    test_nop_100();
    test_nop_1000();
    test_fir_basic();
    test_fir_circular();
}

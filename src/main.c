// Main

#include <stdlib.h>
#include "log.h"
#include "measure.h"
#include "test.h"

extern void target_init();

void main(void)
{
    target_init();
    test_run();
}

void require_failed(const char *cond)
{
    log_msg("Require failed: %s\n", cond);
    exit(1);
}

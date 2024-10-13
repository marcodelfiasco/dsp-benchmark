// Main

#include "log.h"
#include "macro.h"
#include "measure.h"
#include "tp.h"

void test_measure(void)
{
    struct measure_t meas;
    measure_reset(&meas);

    tp0_set();
    for (int loop = 0; loop < 1000; loop++)
    {
        measure_start(&meas);
        NOP_100();
        measure_stop(&meas);
    }
    tp0_clr();

    log_msg("100 NOP loop:\n");
    log_msg("min: %ld nsec\n", (long int)measure_get_min(&meas));
    log_msg("max: %ld nsec\n", (long int)measure_get_max(&meas));
    log_msg("avg: %ld nsec\n", (long int)measure_get_avg(&meas));
}

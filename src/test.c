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
    log_msg("min: %5u cyc - %5u nsec\n", measure_get_min_cycles(&meas),
            measure_get_min_nsec(&meas));
    log_msg("max: %5u cyc - %5u nsec\n", measure_get_max_cycles(&meas),
            measure_get_max_nsec(&meas));
    log_msg("avg: %5u cyc - %5u nsec\n", measure_get_avg_cycles(&meas),
            measure_get_avg_nsec(&meas));
}

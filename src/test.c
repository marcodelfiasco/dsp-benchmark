// Main

#include "log.h"
#include "macro.h"
#include "measure.h"
#include "tp.h"

static void print_test_results(const char *name, struct measure_t *meas)
{
    log_msg("%s test:\n", name);
    log_msg("iterations: %u\n", (unsigned int)measure_get_count(meas));
    log_msg("min: %5u cyc - %5u nsec\n", measure_get_min_cycles(meas),
            measure_get_min_nsec(meas));
    log_msg("max: %5u cyc - %5u nsec\n", measure_get_max_cycles(meas),
            measure_get_max_nsec(meas));
    log_msg("avg: %5u cyc - %5u nsec\n", measure_get_avg_cycles(meas),
            measure_get_avg_nsec(meas));
}

void test_nop_100(void)
{
    struct measure_t meas;
    measure_reset(&meas);

    tp0_set();
    for (int loop = 0; loop < 1000; loop++)
    {
        tp1_set();
        measure_start(&meas);
        NOP_100();
        measure_stop(&meas);
        tp1_clr();
    }
    tp0_clr();

    print_test_results("100 NOP", &meas);
}

void test_nop_1000(void)
{
    struct measure_t meas;
    measure_reset(&meas);

    tp0_set();
    for (int loop = 0; loop < 1000; loop++)
    {
        tp1_set();
        measure_start(&meas);
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        measure_stop(&meas);
        tp1_clr();
    }
    tp0_clr();

    print_test_results("1000 NOP", &meas);
}

// Timing measurement

#include "measure.h"

#include <limits.h>

extern uint64_t get_timestamp(void);
uint64_t timestamp_to_nsec(uint64_t timestamp);
uint64_t timestamp_to_cycles(uint64_t timestamp);

static void _reset(struct measure_t *data, uint64_t overhead)
{
    data->start = 0;
    data->count = 0;
    data->sum = 0;
    data->min = UINT64_MAX;
    data->max = 0;
    data->overhead = overhead;
    data->skip = MEASURE_SKIP_COUNT;
}

void measure_reset(struct measure_t *data)
{
    int i;
    uint64_t overhead;

    _reset(data, 0);
    for (i = 0; i < 10000; i++)
    {
        measure_start(data);
        measure_stop(data);
    }
    overhead = data->sum / data->count;
    _reset(data, overhead);
}

#ifdef __ADSPSHARC__
#pragma never_inline
#endif
void __attribute__((noinline)) measure_start(struct measure_t *data)
{
    uint64_t initial_ts = get_timestamp();
    uint64_t ts;

    // Wait for the timestamp to change
    do
    {
        ts = get_timestamp();
    } while (ts == initial_ts);

    data->start = ts;
}

#ifdef __ADSPSHARC__
#pragma never_inline
#endif
void __attribute__((noinline)) measure_stop(struct measure_t *data)
{
    uint64_t period;

    // Skip initial measurements
    if (data->skip > 0)
    {
        data->skip--;
        return;
    }

#ifdef CPU_MIMXRT1176DVMAA_cm7
    /* Handle ARMv7 32 bit counter */
    int32_t int_diff = (int32_t)get_timestamp() - (int32_t)data->start;
    period = (uint64_t)(int_diff < 0 ? -int_diff : int_diff);
#else
    period = get_timestamp() - data->start;
#endif

    if (period >= data->overhead)
    {
        period -= data->overhead;
    }
    else
    {
        period = 0;
    }
    data->count++;
    data->sum += period;
    if (period < data->min)
    {
        data->min = period;
    }
    if (period > data->max)
    {
        data->max = period;
    }
}

uint64_t measure_get_min_cycles(struct measure_t *data)
{
    return timestamp_to_cycles(data->min);
}

uint64_t measure_get_max_cycles(struct measure_t *data)
{
    return timestamp_to_cycles(data->max);
}

uint64_t measure_get_avg_cycles(struct measure_t *data)
{
    return timestamp_to_cycles(data->sum / data->count);
}

uint64_t measure_get_min_nsec(struct measure_t *data)
{
    return timestamp_to_nsec(data->min);
}

uint64_t measure_get_max_nsec(struct measure_t *data)
{
    return timestamp_to_nsec(data->max);
}

uint64_t measure_get_avg_nsec(struct measure_t *data)
{
    return timestamp_to_nsec(data->sum / data->count);
}

uint64_t measure_get_count(struct measure_t *data)
{
    return data->count;
}

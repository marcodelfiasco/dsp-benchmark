// Target initialization

#include <stdint.h>
#include <time.h>

#define CPU_FREQ 1200000000

void target_init(void)
{
    // TODO
}

uint64_t get_timestamp(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return (uint64_t)time.tv_sec * (uint64_t)1000000000 + time.tv_nsec;
}

uint64_t timestamp_to_nsec(uint64_t timestamp)
{
    // Timestamp is already in nsec
    return timestamp;
}

uint64_t timestamp_to_cycles(uint64_t timestamp)
{
    return (uint64_t)(0.5 + timestamp * (double)CPU_FREQ / 1.0E9);
}
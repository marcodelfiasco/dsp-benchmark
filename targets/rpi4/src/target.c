// Target initialization

#include <stdint.h>
#include <time.h>

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

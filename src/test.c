// Main

#include "log.h"
#include "measure.h"

void test_measure(void)
{
	log_msg("Testing measure\n");

	struct measure_t meas;
	measure_reset(&meas);
	for (int outer_loop = 0; outer_loop < 1000; outer_loop++)
	{
		measure_start(&meas);
		volatile int x = 0;
		for (int inner_loop = 0; inner_loop < 1000; inner_loop++)
		{
			x++;
		}
		measure_stop(&meas);
	}

	log_msg("%ld iterations\n", (long int)measure_get_count(&meas));
	log_msg("min: %ld nsec\n", (long int)measure_get_min(&meas));
	log_msg("max: %ld nsec\n", (long int)measure_get_max(&meas));
	log_msg("avg: %ld nsec\n", (long int)measure_get_avg(&meas));
}

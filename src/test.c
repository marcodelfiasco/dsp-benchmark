// Main

#include "log.h"
#include "measure.h"
#include "tp.h"

void test_measure(void)
{
	tp0_set();

	log_msg("Testing measure\n");

	struct measure_t meas;
	measure_reset(&meas);
	for (int outer_loop = 0; outer_loop < 1000; outer_loop++)
	{
		tp1_set();

		measure_start(&meas);
		volatile int x = 0;
		for (int inner_loop = 0; inner_loop < 1000; inner_loop++)
		{
			x++;
		}
		measure_stop(&meas);

		tp1_clr();
	}

	log_msg("%ld iterations\n", (long int)measure_get_count(&meas));
	log_msg("min: %ld nsec\n", (long int)measure_get_min(&meas));
	log_msg("max: %ld nsec\n", (long int)measure_get_max(&meas));
	log_msg("avg: %ld nsec\n", (long int)measure_get_avg(&meas));

	tp0_clr();
}

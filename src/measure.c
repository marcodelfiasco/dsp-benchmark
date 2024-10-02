// Timing measurement

#include "measure.h"
#include <limits.h>

extern uint64_t get_timestamp(void);

void measure_reset(struct measure_t *data)
{
	data->start = 0;
	data->count = 0;
	data->sum = 0;
	data->min = UINT64_MAX;
	data->max = 0;
}

void measure_start(struct measure_t *data)
{
	data->start = get_timestamp();
}

void measure_stop(struct measure_t *data)
{
	uint64_t period;

	period = get_timestamp() - data->start;
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

uint64_t measure_get_min(struct measure_t *data)
{
	return data->min;
}

uint64_t measure_get_max(struct measure_t *data)
{
	return data->max;
}

uint64_t measure_get_avg(struct measure_t *data)
{
	return data->sum / data->count;
}

uint64_t measure_get_count(struct measure_t *data)
{
	return data->count;
}

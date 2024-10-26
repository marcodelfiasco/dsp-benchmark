#ifndef MEASURE_H_
#define MEASURE_H_

#include <stdint.h>

struct measure_t
{
    uint64_t start;
    uint64_t count;
    uint64_t sum;
    uint64_t min;
    uint64_t max;
    uint64_t overhead;
};

void measure_reset(struct measure_t *data);
void measure_start(struct measure_t *data);
void measure_stop(struct measure_t *data);

// Measure getters: all returned values are in microseconds
uint64_t measure_get_min_cycles(struct measure_t *data);
uint64_t measure_get_max_cycles(struct measure_t *data);
uint64_t measure_get_avg_cycles(struct measure_t *data);
uint64_t measure_get_min_nsec(struct measure_t *data);
uint64_t measure_get_max_nsec(struct measure_t *data);
uint64_t measure_get_avg_nsec(struct measure_t *data);
uint64_t measure_get_count(struct measure_t *data);

#endif // MEASURE_H_

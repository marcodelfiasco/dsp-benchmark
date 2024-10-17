// Basic FIR implementation

#ifndef FIR_CIRCULAR_H_
#define FIR_CIRCULAR_H_

#include <stdbool.h>
#include <string.h>
#include "utils.h"

struct fir_circular_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size' floats)
    int size; // FIR size (i.e. number of coefficients)
    int pos; // State memory position
};

static void fir_circular_init(struct fir_circular_t *fir, float *coeff,
                              float *state, int size)
{
    fir->coeff = coeff;
    fir->state = state;
    fir->size = size;
    fir->pos = 0;

    clear_float_buffer(fir->state, size);
}

static void fir_circular_set_coeff(struct fir_circular_t *fir,
                                   const float *coeff)
{
    reverse_float_buffer(coeff, fir->coeff, fir->size);
}

static void fir_circular_run(struct fir_circular_t *fir, const float *input,
                             float *output, int buffer_size)
{
    float *coeff = fir->coeff;
    float *state = fir->state;
    int fir_size = fir->size;
    int pos = fir->pos;

    for (int i = 0; i < buffer_size; i++)
    {
        state[pos++] = *input++;
        pos = pos % fir_size;

        float sum = 0;
        int taps_pos = pos;
        for (int j = 0; j < fir_size; j++)
        {
            sum += coeff[j] * state[taps_pos++];
            taps_pos %= fir_size;
        }
        *output++ = sum;
    }
    fir->pos = pos;
}

static void fir_circular_restrict_run(struct fir_circular_t *fir,
                                      const float *restrict input,
                                      float *restrict output, int buffer_size)
{
    float *restrict coeff = fir->coeff;
    float *restrict state = fir->state;
    int fir_size = fir->size;
    int pos = fir->pos;

    for (int i = 0; i < buffer_size; i++)
    {
        state[pos++] = *input++;
        pos = pos % fir_size;

        float sum = 0;
        int taps_pos = pos;
        for (int j = 0; j < fir_size; j++)
        {
            sum += coeff[j] * state[taps_pos++];
            taps_pos %= fir_size;
        }
        *output++ = sum;
    }
    fir->pos = pos;
}

#endif // FIR_CIRCULAR_H_

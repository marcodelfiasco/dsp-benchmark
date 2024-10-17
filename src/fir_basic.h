// Basic FIR implementation

#ifndef FIR_BASIC_H_
#define FIR_BASIC_H_

#include <stdbool.h>
#include <string.h>
#include "utils.h"

struct fir_basic_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size' floats)
    int size; // FIR size (i.e. number of coefficients)
};

static void fir_basic_init(struct fir_basic_t *fir, float *coeff, float *state,
                           int size)
{
    fir->coeff = coeff;
    fir->state = state;
    fir->size = size;

    clear_float_buffer(fir->state, size);
}

static void fir_basic_set_coeff(struct fir_basic_t *fir, const float *coeff)
{
    copy_float_buffer(coeff, fir->coeff, fir->size);
}

static void fir_basic_run(const struct fir_basic_t *fir, const float *input,
                          float *output, int buffer_size)
{
    float *coeff = fir->coeff;
    float *state = fir->state;
    int fir_size = fir->size;

    memcpy(&state[fir_size - 1], input, buffer_size * sizeof(float));
    for (int i = 0; i < buffer_size; i++)
    {
        float sum = 0;
        float *taps = &state[fir_size - 1 + i];
        for (int j = 0; j < fir_size; j++)
        {
            sum += coeff[j] * (*taps--);
        }
        *output++ = sum;
    }
    memmove(&state[0], &state[buffer_size], (fir_size - 1) * sizeof(float));
}

static void fir_basic_restrict_run(const struct fir_basic_t *fir,
                                   const float *restrict input,
                                   float *restrict output, int buffer_size)
{
    float *restrict coeff = fir->coeff;
    float *restrict state = fir->state;
    int fir_size = fir->size;

    memcpy(&state[fir_size - 1], input, buffer_size * sizeof(float));
    for (int i = 0; i < buffer_size; i++)
    {
        float sum = 0;
        float *taps = &state[fir_size - 1 + i];
        for (int j = 0; j < fir_size; j++)
        {
            sum += coeff[j] * (*taps--);
        }
        *output++ = sum;
    }
    memmove(&state[0], &state[buffer_size], (fir_size - 1) * sizeof(float));
}

#endif // FIR_BASIC_H_

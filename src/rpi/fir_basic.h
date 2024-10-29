// Basic FIR implementation

#ifndef FIR_BASIC_H_
#define FIR_BASIC_H_

#include <string.h>
#include "fir_common.h"
#include "mem.h"
#include "utils.h"

struct fir_basic_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size + buffer_size' floats)
    int size; // FIR size (i.e. number of coefficients)
};

static void fir_basic_init(struct fir_basic_t *fir, const float *coeff,
                           int fir_size, int buffer_size)
{
    fir->coeff = mem_alloc(DDR, fir_size * sizeof(float));
    fir->state = mem_alloc(DDR, (fir_size + buffer_size) * sizeof(float));
    fir->size = fir_size;

    copy_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size + buffer_size);
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

static void fir_basic_run_restrict(const struct fir_basic_t *fir,
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

static void fir_basic_run_known_size(const struct fir_basic_t *fir,
                                     const float *restrict input,
                                     float *restrict output, int buffer_size)
{
    float *restrict coeff = fir->coeff;
    float *restrict state = fir->state;

    REQUIRE(fir->size == FIR_LEN_KNOWN);
    REQUIRE(buffer_size == BUFFER_LEN_KNOWN);

    memcpy(&state[FIR_LEN_KNOWN - 1], input, BUFFER_LEN_KNOWN * sizeof(float));
    for (int i = 0; i < BUFFER_LEN_KNOWN; i++)
    {
        float sum = 0;
        float *taps = &state[FIR_LEN_KNOWN - 1 + i];
        for (int j = 0; j < FIR_LEN_KNOWN; j++)
        {
            sum += coeff[j] * (*taps--);
        }
        *output++ = sum;
    }
    memmove(&state[0], &state[BUFFER_LEN_KNOWN],
            (FIR_LEN_KNOWN - 1) * sizeof(float));
}

#endif // FIR_BASIC_H_

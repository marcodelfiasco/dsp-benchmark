// Hand optimized C FIR implementation

#ifndef FIR_OPT_H_
#define FIR_OPT_H_

#include <string.h>
#include "fir_common.h"
#include "macro.h"
#include "mem.h"
#include "utils.h"

#define FIR_OPT_VECTOR_SIZE 4

struct fir_opt_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size + buffer_size' floats)
    int size; // FIR size (i.e. number of coefficients)
};

static void fir_opt_init_ddr(struct fir_opt_t *fir, const float *coeff,
                             int fir_size, int buffer_size)
{
    ASSERT((fir_size % 8) == 0); // due to unroll 8 pragma
    ASSERT((buffer_size % FIR_OPT_VECTOR_SIZE) == 0);

    fir->coeff = mem_alloc(DDR, fir_size * sizeof(float));
    fir->state = mem_alloc(DDR, (fir_size + buffer_size) * sizeof(float));
    fir->size = fir_size;

    reverse_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size + buffer_size);
}

static void fir_opt_init_tcm(struct fir_opt_t *fir, const float *coeff,
                             int fir_size, int buffer_size)
{
    ASSERT((fir_size % 8) == 0); // due to unroll 8 pragma
    ASSERT((buffer_size % FIR_OPT_VECTOR_SIZE) == 0);

    fir->coeff = mem_alloc(TCM, fir_size * sizeof(float));
    fir->state = mem_alloc(TCM, (fir_size + buffer_size) * sizeof(float));
    fir->size = fir_size;

    reverse_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size + buffer_size);
}

static void fir_opt_run(const struct fir_opt_t *fir,
                        const float *restrict input, float *restrict output,
                        int buffer_size)
{
    float *restrict coeff = fir->coeff;
    float *restrict state = fir->state;
    int fir_size = fir->size;

    memcpy(&state[fir_size - 1], input, buffer_size * sizeof(float));
    for (int i = 0; i < buffer_size; i += FIR_OPT_VECTOR_SIZE)
    {
        float sum[FIR_OPT_VECTOR_SIZE];
        float *taps[FIR_OPT_VECTOR_SIZE];

#pragma GCC ivdep
        for (int v = 0; v < FIR_OPT_VECTOR_SIZE; v++)
        {
            sum[v] = 0;
            taps[v] = &state[i + v];
        }

#pragma GCC unroll 8
        for (int j = 0; j < fir_size; j++)
        {
#pragma GCC ivdep
            for (int v = 0; v < FIR_OPT_VECTOR_SIZE; v++)
            {
                sum[v] += coeff[j] * (*taps[v]++);
            }
        }

#pragma GCC ivdep
        for (int v = 0; v < FIR_OPT_VECTOR_SIZE; v++)
        {
            *output++ = sum[v];
        }
    }
    memmove(&state[0], &state[buffer_size], (fir_size - 1) * sizeof(float));
}

#endif // FIR_OPT_H_

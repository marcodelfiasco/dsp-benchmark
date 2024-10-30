// Basic FIR implementation

#ifndef FIR_CIRCULAR_H_
#define FIR_CIRCULAR_H_

#include "fir_common.h"
#include "mem.h"
#include "utils.h"

struct fir_circular_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size' floats)
    int size; // FIR size (i.e. number of coefficients)
    int pos; // State memory position
};

static void fir_circular_init_ddr(struct fir_circular_t *fir,
                                  const float *coeff, int fir_size,
                                  int buffer_size)
{
    fir->coeff = mem_alloc(DDR, fir_size * sizeof(float));
    fir->state = mem_alloc(DDR, fir_size * sizeof(float));
    fir->size = fir_size;
    fir->pos = 0;

    reverse_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size);
}

static void fir_circular_init_tcm(struct fir_circular_t *fir,
                                  const float *coeff, int fir_size,
                                  int buffer_size)
{
    fir->coeff = mem_alloc(TCM, fir_size * sizeof(float));
    fir->state = mem_alloc(TCM, fir_size * sizeof(float));
    fir->size = fir_size;
    fir->pos = 0;

    reverse_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size);
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

static void fir_circular_run_restrict(struct fir_circular_t *fir,
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

static void fir_circular_run_unroll(struct fir_circular_t *fir,
                                    const float *restrict input,
                                    float *restrict output, int buffer_size)
{
    float *restrict coeff = fir->coeff;
    float *restrict state = fir->state;
    int fir_size = fir->size;
    int pos = fir->pos;

#pragma GCC unroll 8
    for (int i = 0; i < buffer_size; i++)
    {
        state[pos++] = *input++;
        pos = pos % fir_size;

        float sum = 0;
        int taps_pos = pos;

#pragma GCC unroll 16
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

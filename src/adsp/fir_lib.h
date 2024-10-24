// Sharc library FIR implementation

#ifndef FIR_LIB_H_
#define FIR_LIB_H_

#include <filter.h>
#include "mem.h"
#include "utils.h"

struct fir_lib_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size' floats)
    int size; // FIR size (i.e. number of coefficients)
};

static void fir_lib_init(struct fir_lib_t *fir, const float *coeff,
                         int fir_size, int buffer_size)
{
    fir->coeff = mem_alloc(L1_PM, fir_size * sizeof(float));
    fir->state = mem_alloc(L1_DM, (fir_size + 1) * sizeof(float));
    fir->size = fir_size;

    reverse_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, fir_size + 1);
}

static void fir_lib_run(struct fir_lib_t *fir, const float *input,
                        float *output, int buffer_size)
{
    firf(input, output, (const pm float *)fir->coeff, (dm float *)fir->state,
         buffer_size, fir->size);
}

#endif // FIR_LIB_H_

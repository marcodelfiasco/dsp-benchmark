// CMSIS FIR implementation

#ifndef FIR_CMSIS_H_
#define FIR_CMSIS_H_

#include "arm_math.h"
#include "fir_common.h"
#include "mem.h"
#include "utils.h"

struct fir_cmsis_t
{
    float *coeff; // FIR coefficients (length is 'size' floats)
    float *state; // FIR state (length is 'size' floats)
    arm_fir_instance_f32 inst;
};

static void fir_cmsis_init_ddr(struct fir_cmsis_t *fir, const float *coeff,
                               int fir_size, int buffer_size)
{
    fir->coeff = mem_alloc(DDR, fir_size * sizeof(float));
    fir->state = mem_alloc(DDR, (fir_size + buffer_size - 1) * sizeof(float));

    copy_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, (fir_size + buffer_size - 1));

    arm_fir_init_f32(&fir->inst, fir_size, fir->coeff, fir->state, buffer_size);
}

static void fir_cmsis_init_tcm(struct fir_cmsis_t *fir, const float *coeff,
                               int fir_size, int buffer_size)
{
    fir->coeff = mem_alloc(TCM, fir_size * sizeof(float));
    fir->state = mem_alloc(TCM, (fir_size + buffer_size - 1) * sizeof(float));

    copy_float_buffer(coeff, fir->coeff, fir_size);
    clear_float_buffer(fir->state, (fir_size + buffer_size - 1));

    arm_fir_init_f32(&fir->inst, fir_size, fir->coeff, fir->state, buffer_size);
}

static void fir_cmsis_run(struct fir_cmsis_t *fir, const float *input,
                          float *output, int buffer_size)
{
    arm_fir_f32(&fir->inst, input, output, buffer_size);
}

#endif // FIR_CMSIS_H_

// Main

#include "log.h"
#include "macro.h"
#include "measure.h"
#include "tp.h"
#include "test_vectors.h"
#include "fir_basic.h"
#include "fir_circular.h"

#define MAX_INPUT_LEN 8192
#define MAX_OUTPUT_LEN 8192
#define MAX_COEFF_LEN 4096
#define MAX_STATE_LEN (4096 + 128)
static float _test_input_buffer[MAX_INPUT_LEN];
static float _test_output_buffer[MAX_OUTPUT_LEN];
static float _fir_coeffs[MAX_COEFF_LEN];
static float _fir_state[MAX_STATE_LEN];
static int _test_buffer_size[] = { 8, 16, 32, 64 };
static const float _error_threshold = 6.5E-6;
static struct
{
    char name[64];
    int fir_size;
    int buffer_size;
    struct measure_t meas;
    float max_error;
    bool failed;
} _test_data;

static void _print_test_result_header(void)
{
    log_msg("name,fir_size,buffer_size,result,max_error_e6,count,"
            "min_cycles,max_cycles,avg_cycles,"
            "min_nsec,max_nsec,avg_nsec,mac,mac_per_cycle,mac_per_nanosec\n");
}

static void _print_test_results(void)
{
#define ZERO_IF_FAILED(expr) _test_data.failed ? 0 : (expr)

    log_msg("%s,", _test_data.name);
    log_msg("%d,", _test_data.fir_size);
    log_msg("%d,", _test_data.buffer_size);
    log_msg("%s,", _test_data.failed ? "fail" : "OK");
    log_msg("%fe-6,", _test_data.max_error * 1E6f);
    log_msg("%u,",
            ZERO_IF_FAILED((unsigned int)measure_get_count(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_min_cycles(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_max_cycles(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_avg_cycles(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_min_nsec(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_max_nsec(&_test_data.meas)));
    log_msg("%u,", ZERO_IF_FAILED(measure_get_avg_nsec(&_test_data.meas)));

    unsigned int mac = _test_data.fir_size * _test_data.buffer_size;
    log_msg("%u,", ZERO_IF_FAILED(mac));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / (float)measure_get_avg_cycles(
                                                   &_test_data.meas)));
    log_msg("%f\n", ZERO_IF_FAILED((float)mac / (float)measure_get_avg_nsec(
                                                    &_test_data.meas)));

#undef ZERO_IF_FAILED
}

static void TEST_BEGIN(const char *name, int fir_size, int buffer_size)
{
    snprintf(_test_data.name, sizeof(_test_data.name), "%s", name);
    _test_data.failed = false;
    _test_data.max_error = 0.0f;
    measure_reset(&_test_data.meas);
    _test_data.fir_size = fir_size;
    _test_data.buffer_size = buffer_size;

    clear_float_buffer(_test_input_buffer, MAX_INPUT_LEN);
    clear_float_buffer(_test_output_buffer, MAX_OUTPUT_LEN);
    ASSERT(get_input_length() <= MAX_INPUT_LEN);
    copy_float_buffer(get_input(), _test_input_buffer, get_input_length());
}

static void TEST_END()
{
    _print_test_results();
}

static void TEST_FAILED()
{
    _test_data.failed = true;
}

void test_init(void)
{
    _print_test_result_header();
}

void test_nop_100(void)
{
    TEST_BEGIN("100 NOP", 0, 0);

    tp0_set();
    for (int loop = 0; loop < 1000; loop++)
    {
        tp1_set();
        measure_start(&_test_data.meas);
        NOP_100();
        measure_stop(&_test_data.meas);
        tp1_clr();
    }
    tp0_clr();

    TEST_END();
}

void test_nop_1000(void)
{
    TEST_BEGIN("1000 NOP", 0, 0);

    tp0_set();
    for (int loop = 0; loop < 1000; loop++)
    {
        tp1_set();
        measure_start(&_test_data.meas);
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        NOP_100();
        measure_stop(&_test_data.meas);
        tp1_clr();
    }
    tp0_clr();

    TEST_END();
}

#define FIR_RUNNER(name, fir_data_t, init_func, set_coeff_func, run_func)   \
    {                                                                       \
        int buffer_size = _test_buffer_size[bsize_idx];                     \
        const float *input = _test_input_buffer;                            \
        float *output = _test_output_buffer;                                \
        fir_data_t fir_data;                                                \
                                                                            \
        TEST_BEGIN(name, get_fir_length(fir_idx), buffer_size);             \
        ASSERT(get_fir_length(fir_idx) <= MAX_COEFF_LEN);                   \
                                                                            \
        init_func(&fir_data, _fir_coeffs, _fir_state,                       \
                  get_fir_length(fir_idx));                                 \
        set_coeff_func(&fir_data, get_fir_coeffs(fir_idx));                 \
        for (int processed = 0; processed < get_input_length();             \
             processed += buffer_size)                                      \
        {                                                                   \
            measure_start(&_test_data.meas);                                \
            run_func(&fir_data, input, output, buffer_size);                \
            measure_stop(&_test_data.meas);                                 \
            input += buffer_size;                                           \
            output += buffer_size;                                          \
        }                                                                   \
                                                                            \
        if (!compare_float_buffer(_test_output_buffer, get_output(fir_idx), \
                                  get_output_length(fir_idx),               \
                                  _error_threshold, &_test_data.max_error)) \
        {                                                                   \
            TEST_FAILED();                                                  \
        }                                                                   \
        TEST_END();                                                         \
    }

void test_fir_basic(void)
{
    int fir_idx, bsize_idx;

    for (fir_idx = 0; fir_idx < get_num_fir(); fir_idx++)
    {
        for (bsize_idx = 0; bsize_idx < ARRAYSIZE(_test_buffer_size);
             bsize_idx++)
        {
            FIR_RUNNER("FIR.b", struct fir_basic_t, fir_basic_init,
                       fir_basic_set_coeff, fir_basic_run);
            FIR_RUNNER("FIR.br", struct fir_basic_t, fir_basic_init,
                       fir_basic_set_coeff, fir_basic_restrict_run);
        }
    }
}

void test_fir_circular(void)
{
    int fir_idx, bsize_idx;

    for (fir_idx = 0; fir_idx < get_num_fir(); fir_idx++)
    {
        for (bsize_idx = 0; bsize_idx < ARRAYSIZE(_test_buffer_size);
             bsize_idx++)
        {
            FIR_RUNNER("FIR.c", struct fir_circular_t, fir_circular_init,
                       fir_circular_set_coeff, fir_circular_run);
            FIR_RUNNER("FIR.cr", struct fir_circular_t, fir_circular_init,
                       fir_circular_set_coeff, fir_circular_restrict_run);
        }
    }
}

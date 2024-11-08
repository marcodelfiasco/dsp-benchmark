// Main

#include "log.h"
#include "macro.h"
#include "measure.h"
#include "mem.h"
#include "tp.h"
#include "utils.h"
#include "test_vectors.h"
#if defined(__ADSPSHARC__)
    #include "adsp/fir_basic.h"
    #include "adsp/fir_circular.h"
    #include "adsp/fir_lib.h"
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    #include "imxrt/fir_basic.h"
    #include "imxrt/fir_circular.h"
    #include "imxrt/fir_opt.h"
    #include "imxrt/fir_cmsis.h"
#else
    #include "rpi/fir_basic.h"
    #include "rpi/fir_circular.h"
    #include "rpi/fir_opt.h"
    #include "rpi/fir_cmsis.h"
#endif

extern void test_sleep(void);

#define FIR_TEST_TOTAL_LEN 48000 // the total length of each test

#define MAX_INPUT_LEN 20480 // same as test_vectors.c::INPUT_VECTOR_LEN
#define MAX_OUTPUT_LEN 20480 // same as test_vectors.c::INPUT_VECTOR_LEN
static AUDIO_BUFFER_SECTION float _test_input_buffer[MAX_INPUT_LEN];
static AUDIO_BUFFER_SECTION float _test_output_buffer[MAX_OUTPUT_LEN];
static int _test_buffer_size[] = { 8, 16, 32, 64, 128 };
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
    log_msg("name,fir_size,buffer_size,mac,result,max_error_e6,count,"
            "min_cycles,max_cycles,avg_cycles,"
            "min_nsec,max_nsec,avg_nsec,"
            "min_mac_per_cycle,max_mac_per_cycle,avg_mac_per_cycle,"
            "min_mac_per_nsec,max_mac_per_nsec,avg_mac_per_nsec\n");
}

static void _print_test_results(void)
{
#define ZERO_IF_FAILED(expr) _test_data.failed ? 0 : (expr)

    unsigned int mac = _test_data.fir_size * _test_data.buffer_size;

    float min_cycles = (float)measure_get_min_cycles(&_test_data.meas);
    float max_cycles = (float)measure_get_max_cycles(&_test_data.meas);
    float avg_cycles = (float)measure_get_avg_cycles(&_test_data.meas);
    float min_nsec = (float)measure_get_min_nsec(&_test_data.meas);
    float max_nsec = (float)measure_get_max_nsec(&_test_data.meas);
    float avg_nsec = (float)measure_get_avg_nsec(&_test_data.meas);

    log_msg("%s,", _test_data.name);
    log_msg("%d,", _test_data.fir_size);
    log_msg("%d,", _test_data.buffer_size);
    log_msg("%u,", mac);
    log_msg("%s,", _test_data.failed ? "fail" : "OK");
    log_msg("%fe-6,", _test_data.max_error * 1E6f);
    log_msg("%f,", ZERO_IF_FAILED((float)measure_get_count(&_test_data.meas)));
    log_msg("%f,", ZERO_IF_FAILED(min_cycles));
    log_msg("%f,", ZERO_IF_FAILED(max_cycles));
    log_msg("%f,", ZERO_IF_FAILED(avg_cycles));
    log_msg("%f,", ZERO_IF_FAILED(min_nsec));
    log_msg("%f,", ZERO_IF_FAILED(max_nsec));
    log_msg("%f,", ZERO_IF_FAILED(avg_nsec));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / max_cycles));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / min_cycles));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / avg_cycles));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / max_nsec));
    log_msg("%f,", ZERO_IF_FAILED((float)mac / min_nsec));
    log_msg("%f\n", ZERO_IF_FAILED((float)mac / avg_nsec));
#undef ZERO_IF_FAILED
}

static void _test_begin(const char *name, int fir_size, int buffer_size)
{
    snprintf(_test_data.name, sizeof(_test_data.name), "%s", name);
    _test_data.failed = false;
    _test_data.max_error = 0.0f;
    measure_reset(&_test_data.meas);
    _test_data.fir_size = fir_size;
    _test_data.buffer_size = buffer_size;

    clear_float_buffer(_test_input_buffer, MAX_INPUT_LEN);
    clear_float_buffer(_test_output_buffer, MAX_OUTPUT_LEN);
    REQUIRE(get_input_length() <= MAX_INPUT_LEN);
    copy_float_buffer(get_input(), _test_input_buffer, get_input_length());

    mem_free_all();
}

static void _test_end()
{
    _print_test_results();
}

static void _test_failed()
{
    _test_data.failed = true;
}

static void _test_tp_pulses(void)
{
    // Generate 10 pulses of 100000 NOPs each (each pulse is ~100uS @ 1GHz)
    for (int pulse = 0; pulse < 10; pulse++)
    {
        tp0_set();
        tp1_clr();
        for (int loop = 0; loop < 1000; loop++)
        {
            NOP_100();
        }
        tp0_clr();
        tp1_set();
        for (int loop = 0; loop < 1000; loop++)
        {
            NOP_100();
        }
    }
}

static void _test_measure(void)
{
    const float max_error_percent = 10.0f;

    for (int repeat = 0; repeat < 100; repeat++)
    {
        struct measure_t meas;

        measure_reset(&meas);
        for (int loop = 0; loop < 1000; loop++)
        {
            measure_start(&meas);
            NOP_100();
            NOP_100();
            NOP_100();
            NOP_100();
            NOP_100();
            measure_stop(&meas);
        }

        float error_percent =
            ((float)measure_get_avg_cycles(&meas) - 500.0f) / 500.0f * 100.0f;
        if (error_percent < 0)
        {
            error_percent = -error_percent;
        }
        //log_msg("measure error: +-%f%% (%f)\n", error_percent, (float)measure_get_avg_cycles(&meas));
        REQUIRE(error_percent <= max_error_percent);
    }
}

static void _test_nop_100(void)
{
    _test_begin("100 NOP", 0, 0);

    for (int loop = 0; loop < 10000; loop++)
    {
        measure_start(&_test_data.meas);
        NOP_100();
        measure_stop(&_test_data.meas);
    }

    _test_end();
}

static void _test_nop_1000(void)
{
    _test_begin("1000 NOP", 0, 0);

    for (int loop = 0; loop < 10000; loop++)
    {
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
    }

    _test_end();
}

#define FIR_RUNNER(name, fir_data_t, init_func, run_func, cache_thrash_lines)  \
    {                                                                          \
        int buffer_size = _test_buffer_size[bsize_idx];                        \
        fir_data_t fir_data;                                                   \
        int processed_total;                                                   \
        int processed_inner_loop;                                              \
                                                                               \
        _test_begin(name, get_fir_length(fir_idx), buffer_size);               \
        init_func(&fir_data, get_fir_coeffs(fir_idx), get_fir_length(fir_idx), \
                  buffer_size);                                                \
        for (processed_total = 0; processed_total < FIR_TEST_TOTAL_LEN;        \
             processed_total += processed_inner_loop)                          \
        {                                                                      \
            const float *input = _test_input_buffer;                           \
            float *output = _test_output_buffer;                               \
            for (processed_inner_loop = 0;                                     \
                 (processed_inner_loop + buffer_size) < get_input_length();    \
                 processed_inner_loop += buffer_size)                          \
            {                                                                  \
                test_sleep();                                                  \
                mem_cache_thrash(cache_thrash_lines);                          \
                measure_start(&_test_data.meas);                               \
                run_func(&fir_data, input, output, buffer_size);               \
                measure_stop(&_test_data.meas);                                \
                input += buffer_size;                                          \
                output += buffer_size;                                         \
            }                                                                  \
        }                                                                      \
                                                                               \
        if (!compare_float_buffer(_test_output_buffer, get_output(fir_idx),    \
                                  get_output_length(fir_idx),                  \
                                  _error_threshold, &_test_data.max_error))    \
        {                                                                      \
            _test_failed();                                                    \
        }                                                                      \
        _test_end();                                                           \
    }

static void _test_fir(void)
{
    int fir_idx;
    int bsize_idx;

    // clang-format off
    for (fir_idx = 0; fir_idx < get_num_fir(); fir_idx++)
    for (bsize_idx = 0; bsize_idx < ARRAYSIZE(_test_buffer_size); bsize_idx++)
    {
#if defined(__ADSPSHARC__)
    FIR_RUNNER("FIR.basic", struct fir_basic_t,
                fir_basic_init,
                fir_basic_run,
                0);
    FIR_RUNNER("FIR.basic_restrict", struct fir_basic_t,
                fir_basic_init,
                fir_basic_run_restrict,
                0);
    FIR_RUNNER("FIR.basic_dual_bank", struct fir_basic_t,
                fir_basic_init_dual_bank,
                fir_basic_run_dual_bank,
                0);
    FIR_RUNNER("FIR.basic_aligned", struct fir_basic_t,
                fir_basic_init_dual_bank,
                fir_basic_run_dual_bank_aligned,
                0);
    FIR_RUNNER("FIR.basic_loop_cnt", struct fir_basic_t,
                fir_basic_init_dual_bank,
                fir_basic_run_dual_bank_aligned_loop_count,
                0);

    FIR_RUNNER("FIR.circular", struct fir_circular_t,
                fir_circular_init,
                fir_circular_run,
                0);
    FIR_RUNNER("FIR.circular_opt", struct fir_circular_t,
                fir_circular_init_dual_bank,
                fir_circular_run_optimized,
                0);

    FIR_RUNNER("FIR.lib", struct fir_lib_t,
                fir_lib_init,
                fir_lib_run,
                0);
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    FIR_RUNNER("FIR.basic_ddr", struct fir_basic_t,
                fir_basic_init_ddr,
                fir_basic_run,
                0);
    FIR_RUNNER("FIR.basic_ddr_restrict", struct fir_basic_t,
                fir_basic_init_ddr,
                fir_basic_run_restrict,
                0);
    FIR_RUNNER("FIR.basic_tcm", struct fir_basic_t,
                fir_basic_init_tcm,
                fir_basic_run,
                0);
    FIR_RUNNER("FIR.basic_tcm_restrict", struct fir_basic_t,
                fir_basic_init_tcm,
                fir_basic_run_restrict,
                0);

    FIR_RUNNER("FIR.circular_ddr", struct fir_circular_t,
                fir_circular_init_ddr,
                fir_circular_run,
                0);
    FIR_RUNNER("FIR.circular_ddr_restrict", struct fir_circular_t,
                fir_circular_init_ddr,
                fir_circular_run_restrict,
                0);
    FIR_RUNNER("FIR.circular_ddr_unroll", struct fir_circular_t,
                fir_circular_init_ddr,
                fir_circular_run_unroll,
                0);
    FIR_RUNNER("FIR.circular_tcm", struct fir_circular_t,
                fir_circular_init_tcm,
                fir_circular_run,
                0);
    FIR_RUNNER("FIR.circular_tcm_restrict", struct fir_circular_t,
                fir_circular_init_tcm,
                fir_circular_run_restrict,
                0);
    FIR_RUNNER("FIR.circular_tcm_unroll", struct fir_circular_t,
                fir_circular_init_tcm,
                fir_circular_run_unroll,
                0);

    FIR_RUNNER("FIR.opt_ddr", struct fir_opt_t,
                fir_opt_init_ddr,
                fir_opt_run,
                0);
    FIR_RUNNER("FIR.opt_tcm", struct fir_opt_t,
                fir_opt_init_tcm,
                fir_opt_run,
                0);

    FIR_RUNNER("FIR.cmsis_ddr", struct fir_cmsis_t,
                fir_cmsis_init_ddr,
                fir_cmsis_run,
                0);
    FIR_RUNNER("FIR.cmsis_tcm", struct fir_cmsis_t,
                fir_cmsis_init_tcm,
                fir_cmsis_run,
                0);

    FIR_RUNNER("FIR.basic_ddr_cache_thrash", struct fir_basic_t,
                fir_basic_init_ddr,
                fir_basic_run,
                256);
    FIR_RUNNER("FIR.cmsis_ddr_cache_thrash", struct fir_cmsis_t,
                fir_cmsis_init_ddr,
                fir_cmsis_run,
                256);
#else
    FIR_RUNNER("FIR.basic", struct fir_basic_t,
                fir_basic_init,
                fir_basic_run,
                0);
    FIR_RUNNER("FIR.basic_restrict", struct fir_basic_t,
                fir_basic_init,
                fir_basic_run_restrict,
                0);

    FIR_RUNNER("FIR.circular", struct fir_circular_t,
                fir_circular_init,
                fir_circular_run,
                0);
    FIR_RUNNER("FIR.circular_restrict", struct fir_circular_t,
                fir_circular_init,
                fir_circular_run_restrict,
                0);

    FIR_RUNNER("FIR.opt", struct fir_opt_t,
                fir_opt_init_ddr,
                fir_opt_run,
                0);

    FIR_RUNNER("FIR.cmsis", struct fir_cmsis_t,
                fir_cmsis_init,
                fir_cmsis_run,
                0);

    FIR_RUNNER("FIR.basic_cache_thrash", struct fir_basic_t,
                fir_basic_init,
                fir_basic_run,
                256);
    FIR_RUNNER("FIR.opt_cache_thrash", struct fir_opt_t,
                fir_opt_init_ddr,
                fir_opt_run,
                256);
#endif
    }
    // clang-format on
}

void test_run(void)
{
    _print_test_result_header();

    _test_tp_pulses();
    _test_measure();
    _test_nop_100();
    _test_nop_1000();

    _test_fir();
}

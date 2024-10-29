// Target initialization

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"
#include "macro.h"

extern void tp_init(void);

void target_init(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    tp_init();

    uint32_t core_clock_freq = CLOCK_GetFreq(kCLOCK_CpuClk);
    REQUIRE(core_clock_freq == 996000000); // We should be running at 996MHz
    (void)core_clock_freq;
}

uint64_t get_timestamp(void)
{
    return DWT->CYCCNT;
}

uint64_t timestamp_to_nsec(uint64_t timestamp)
{
    return (uint64_t)(0.5 +
                      timestamp * 1.0E9 / (double)CLOCK_GetFreq(kCLOCK_CpuClk));
}

uint64_t timestamp_to_cycles(uint64_t timestamp)
{
    // Timestamp is already in cycles
    return timestamp;
}
// Target initialization

#include <stdint.h>
#include <builtins.h>
#include <sys/platform.h>
#include <services/int/adi_sec.h>
#include <services/pwr/adi_pwr.h>
#include "macro.h"
#include "log.h"

extern void tp_init(void);

void target_init(void)
{
    adi_sec_Init();
    tp_init();

    ADI_PWR_RESULT pwr_result = adi_pwr_Init(0, 25000000);
    ASSERT(pwr_result == ADI_PWR_SUCCESS);

    uint32_t core_clock_freq;
    pwr_result = adi_pwr_GetCoreClkFreq(0u, &core_clock_freq);
    ASSERT(pwr_result == ADI_PWR_SUCCESS);
    ASSERT(core_clock_freq == 1000000000);  // We should be running at 1GHz
}

uint64_t get_timestamp(void)
{
    return __builtin_emuclk();
}

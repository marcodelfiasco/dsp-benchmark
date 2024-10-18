// Simple memory allocator

#include <stddef.h>
#include <stdint.h>
#include "log.h"
#include "macro.h"
#include "mem.h"

#define MEM_POOL_SIZE (48 * 1024)

#if defined(__ADSPSHARC__)
#pragma alignment_region(8)
static dm uint8_t _dm_pool[MEM_POOL_SIZE];
static pm uint8_t _pm_pool[MEM_POOL_SIZE];
#pragma alignment_region_end
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
static __attribute__((section(".data"))) __attribute__((aligned(8)))
uint8_t _ddr_pool[MEM_POOL_SIZE];
static __attribute__((section("DataQuickAccess"))) __attribute__((aligned(8)))
uint8_t _tcm_pool[MEM_POOL_SIZE];
#else
static __attribute__((aligned(8))) uint8_t _ddr_pool[MEM_POOL_SIZE];
#endif

struct mem_pool_desc_t
{
    uint8_t *base;
    unsigned int total_size;
    unsigned int allocated_size;
};

struct mem_pool_desc_t _mem_desc[MEM_TYPE_NUM] = {
#if defined(__ADSPSHARC__)
    { _dm_pool, sizeof(_dm_pool), 0 }, // L1_DM
    { _pm_pool, sizeof(_pm_pool), 0 }, // L1_PM
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    { _ddr_pool, sizeof(_ddr_pool), 0 }, // DDR
    { _tcm_pool, sizeof(_tcm_pool), 0 }, // TCM
#else
    { _ddr_pool, sizeof(_ddr_pool), 0 }, // DDR
#endif
};

void *mem_alloc(enum mem_pool_type mem_type, unsigned int size)
{
    uint8_t *ptr = NULL;

    if (mem_type < MEM_TYPE_NUM)
    {
        if ((_mem_desc[mem_type].allocated_size + size) <=
            _mem_desc[mem_type].total_size)
        {
            ptr = _mem_desc[mem_type].base + _mem_desc[mem_type].allocated_size;
            _mem_desc[mem_type].allocated_size += size;
        }
    }

    ASSERT(ptr != NULL);
    return ptr;
}

void mem_free_all(void)
{
    int i;

    for (i = 0; i < MEM_TYPE_NUM; i++)
    {
        _mem_desc[i].allocated_size = 0;
    }
}

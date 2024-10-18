// Simple memory allocator

#ifndef MEM_H_
#define MEM_H_

enum mem_pool_type
{
#if defined(__ADSPSHARC__)
    L1_DM,
    L1_PM,
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
    DDR,
    TCM,
#else
    DDR,
#endif
    MEM_TYPE_NUM
};

void *mem_alloc(enum mem_pool_type mem_type, unsigned int size);
void mem_free_all(void);

#endif // MEM_H_

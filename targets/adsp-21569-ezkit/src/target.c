// Target initialization

#include <stdint.h>
#include <sys/platform.h>
#include <services/int/adi_sec.h>

extern void tp_init(void);

void target_init(void)
{
	adi_sec_Init();
	tp_init();
}

uint64_t get_timestamp(void)
{
	// TODO
	return 0;
}

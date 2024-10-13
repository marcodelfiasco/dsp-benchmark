#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <stdint.h>

#ifdef MCUXPRESSO_SDK
#include "fsl_debug_console.h"
#endif

#ifndef PRINTF
#define PRINTF printf
#endif

#define log_msg(fmt...) PRINTF(fmt)

#endif // LOG_H_

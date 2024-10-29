// Target initialization

#define _GNU_SOURCE
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <evl/clock.h>
#include <evl/thread.h>
#include "log.h"
#include "macro.h"

#define CPU_FREQ 1500000000
#define CPU_NUMBER 3 // We're pinning to an isolated CPU

static uint32_t _get_cpu_freq(void)
{
    uint32_t cpu_freq = 0;
    char filename[128];
    FILE *fp;

    sprintf(filename, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq",
            CPU_NUMBER);
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        log_msg("Failed to open %s\n", filename);
        exit(1);
    }
    if (fscanf(fp, "%u", &cpu_freq) != 1)
    {
        log_msg("Failed to read CPU frequency\n");
        exit(1);
    }
    fclose(fp);

    return cpu_freq * 1000;
}

void target_init(void)
{
    cpu_set_t set;
    struct sched_param param;
    int ret;
    int evl_fd;

    // Check CPU frequency
    if (_get_cpu_freq() != CPU_FREQ)
    {
        log_msg("CPU frequency mismatch\n");
        exit(1);
    }

    // Set CPU affinity
    CPU_ZERO(&set);
    CPU_SET(CPU_NUMBER, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
    {
        log_msg("sched_setaffinity failed\n");
        exit(1);
    }

    // Lock the whole address space
    ret = mlockall(MCL_CURRENT | MCL_FUTURE);
    if (ret < 0)
    {
        log_msg("mlockall failed\n");
        exit(1);
    }

    // Set sched FIFO with maximum priority
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    if (ret != 0)
    {
        log_msg("pthread_setschedparam failed\n");
        exit(1);
    }

    evl_fd = evl_attach_self("evl-thread:%d", getpid());

    REQUIRE(evl_is_inband() == false);
}

uint64_t get_timestamp(void)
{
    struct timespec time;
    evl_read_clock(EVL_CLOCK_MONOTONIC, &time);
    return (uint64_t)time.tv_sec * (uint64_t)1000000000 + time.tv_nsec;
}

uint64_t timestamp_to_nsec(uint64_t timestamp)
{
    // Timestamp is already in nsec
    return timestamp;
}

uint64_t timestamp_to_cycles(uint64_t timestamp)
{
    return (uint64_t)(0.5 + timestamp * (double)CPU_FREQ / 1.0E9);
}
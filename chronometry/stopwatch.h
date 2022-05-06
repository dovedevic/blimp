#ifndef __STOPWATCH__
#define __STOPWATCH__

#include <time.h>
#include <stdio.h>
#define CHRONO_COUNT 100

extern clock_t CHRONOS[CHRONO_COUNT + 1];
extern unsigned long int SPLIT_TIMES[CHRONO_COUNT + 1];
extern unsigned long int INVOKE_TIMES[CHRONO_COUNT + 1];
extern unsigned long int INVOKE_DATA_SIZE[CHRONO_COUNT + 1];
extern FILE *CHRONO_RESULT;

extern void START_PROGRAM();
extern void STOP_PROGRAM();
extern void START_CHRONO(int clkIdx);
extern void START_CHRONO_LOG_SIZE(int clkIdx, int n);
extern void STOP_CHRONO(int clkIdx);

#endif

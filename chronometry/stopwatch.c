#include "stopwatch.h"
#include <unistd.h>

clock_t CHRONOS[CHRONO_COUNT + 1];
unsigned long int SPLIT_TIMES[CHRONO_COUNT + 1];
unsigned long int INVOKE_TIMES[CHRONO_COUNT + 1];
unsigned long int INVOKE_DATA_SIZE[CHRONO_COUNT + 1];
FILE *CHRONO_RESULT;

void START_PROGRAM() {
    CHRONOS[0] = clock();
    int i;
    for (i = 0; i < CHRONO_COUNT + 1; i++) {
        SPLIT_TIMES[i] = 0;
        INVOKE_TIMES[i] = 0;
        INVOKE_DATA_SIZE[i] = 0;
    }
    CHRONO_RESULT = fopen("chrono_result.out", "w");
}

void STOP_PROGRAM() {
    SPLIT_TIMES[0] = clock() - CHRONOS[0];
    fprintf(CHRONO_RESULT, "Program Clock Step Total Time:\t%ld\n", SPLIT_TIMES[0]);
    int i;
    for (i = 1; i < CHRONO_COUNT + 1; i++) {
        fprintf(CHRONO_RESULT, "C%d\t%ld\t%ld\t%ld\n", i-1, SPLIT_TIMES[i], INVOKE_TIMES[i], INVOKE_DATA_SIZE[i]);
    }
    fclose(CHRONO_RESULT);
}

void START_CHRONO(int clkIdx) {
    CHRONOS[clkIdx+1] = clock();
    INVOKE_TIMES[clkIdx+1] += 1;
}

void START_CHRONO_LOG_SIZE(int clkIdx, int n){
    CHRONOS[clkIdx+1] = clock();
    INVOKE_TIMES[clkIdx+1] += 1;
    INVOKE_DATA_SIZE[clkIdx+1] = n > INVOKE_DATA_SIZE[clkIdx+1] ? n : INVOKE_DATA_SIZE[clkIdx+1];
}

void STOP_CHRONO(int clkIdx) {
    SPLIT_TIMES[clkIdx+1] += clock() - CHRONOS[clkIdx+1];
}

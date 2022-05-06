# Chronometry

The following source defines the chronometry, or chrono, used for intra-application region timing in both the SPEC and Phoenix benchmarks. It uses C/C++ STD-99 compatible source and uses the `time` library. The package offers `CHRONO_COUNT` (default 100) clocks to use. If your application requires more clocks, edit the header and source and recompile. 

There are 5 main functions that the package provides;
* `START_PROGRAM()` : This function shall be placed right at the entry of the `main` function in the application, just after the opening `{` in the function definition. This function begins the overall wall clock used to calculate relative clock timings later.
* `STOP_PROGRAM()` : This function shall be placed right before the exit or return of the `main` function. This function ends the wall clock started by the previous method and stops all existing clocks. The function will then dump all clock data to the current working directory as a file named `chrono_result.out`. *Note: it will overwrite existing chrono results so be sure to save or backup results between runs*. The output file is formatted as follows:
  * Line 1: Total wall clock steps
  * Lines 2-...: TSV (tab separated) clock results;
    * Clock Index - (formatted as `C##`)
    * Total Clock Index Steps - Aggregate number of steps this clock was run for
    * Total Clock Index Invocations/Starts - Number of times this clock was started (regardless if it was not stopped beforehand)
    * Largest Data Payload Seen - Largest value passed into the START_LOG function as defined below
* `START_CHRONO(int clkIdx)` : Given a clock index (`clkIdx`), start the chrono to the current wall clock step (if this clock was already running, overwrite the start time to now)
* `START_CHRONO_LOG_SIZE(int clkIdx, int n)` : Given a clock index, perform `START_CHRONO` and log an integer payload. This payload is intended to be used as a measure of data sizes for debugging and profiling. **Only the largest seen payload integer is saved.**
* `STOP_CHRONO(int clkIdx)` : Given a clock index, end the started chrono at the current wall clock step, calculate the step difference, and add the difference to the aggregated clock index wall clock count.


## Compiling

Compile the sources using any C/C++ compiler with no optimizations enabled. 

For C sources, use `gcc`, for C++ use `g++`:

```sh
$ gxx -c -o stopwatch.o stopwatch.c
```

This object file and header shall then be used in the linker when compiling the source you wish to benchmark, as defined below.

## Using

Take the following application as an example:

```C
// sample.c
int main(int argc, char* argv[]) {
    
    int foo = 0;
    for (int i = 0; i <= 100000000; i++) {
        foo /= 2;
        
        for (int j = 0; j <= 3500; j++) {
            foo += i % 7 + 2;
        }
    }
    
    return 0;
}
```

Suppose we wish to profile the timing of the two for loops; we can instrument this application as follows:

```C
// sample.c
#include "stopwatch.h"

int main(int argc, char* argv[]) { START_PROGRAM();
    
    int foo = 0;
    START_CHRONO(0); for (int i = 0; i <= 1000000; i++) {
        foo /= 2;
        
        START_CHRONO_LOG_SIZE(1, foo); for (int j = 0; j <= 3500; j++) {
            foo += i % 7 + 2;
        }STOP_CHRONO(1); 
    }STOP_CHRONO(0); 
    
    STOP_PROGRAM(); return 0;
}
```

With the instrumented application, we compile it with the chronometry package as follows:

As a stand-alone:

```sh
$ gxx -Ipath/to/chronometry/c_h_and_o_files /path/to/stopwatch.o -o sample sample.c
```

or, as part of a build process:
```sh
$ gxx -c -Ipath/to/chronometry/c_h_and_o_files -o sample.o sample.c
$ gxx /path/to/stopwatch.o sample.o -o sample
```

After running this application, `sample`, the `chrono_result.out` file appears in the current working directory. The contents of which (for my system) is as follows:

```sh
$ cat chrono_result.out
Program Clock Step Total Time:  8689916
C0      8689773 1       0
C1      8224643 1000001 24699
C2      0       0       0
C3      0       0       0
C4      0       0       0
C5      0       0       0
C6      0       0       0

...

C99     0       0       0
```

From this result, we can see that the outer loop, clocked by clock index 0, or `C0`, ran for 8689773 of 8689916 steps, or 99.998% of the runtime, while `C1` ran for 8224643 of 8689916 steps, or 94.645%. If we run the application as follows, `/usr/bin/time -v sample`, we can also deduce accurate elapsed time in seconds. Lastly, we see that the largest value of `foo`, as tracked by `C1`, was 24699.

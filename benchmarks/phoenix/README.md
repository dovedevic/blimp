# Phoenix Benchmarking

The Phoenix suite/repository contains 8 sample map-reduce applications for which one could investigate offloadability to PIM. We use the [Chronometry package](https://github.com/dovedevic/blimp/tree/main/chronometry) for instrumentation and the linux system `time` application for timing data. One can use an automatic instrumentation routine (such as that used by a compiler we define in the paper) or done manually. The benchmarks to be instrumented are in `phoenix/sample_apps/{benchmark}`.

# Building an Instrumented Benchmark

For any sample application/benchmark Phoenix provides, the `Makefile` must be edited to include the [Chronometry package](https://github.com/dovedevic/blimp/tree/main/chronometry). This can be easily done by appending the following to the line defining the `CFLAGS` in the `Makefile`:

```
-Ipath/to/chronometry/c_h_and_o_files /path/to/stopwatch.o
```

For example, in the Histogram `Makefile`, we append the above to [line 57](https://github.com/kozyraki/phoenix/blob/master/sample_apps/histogram/Makefile#L57):

```
L57:  CFLAGS = -Wall $(ARCH) -O3 -Ipath/to/chronometry/c_h_and_o_files /path/to/stopwatch.o 
```

# Running an Instrumented Benchmark

To run an instrumented benchmark, we use the following command to combine timing data and chronometry data into the same file, as well as any benchmark output:

```sh
{ { /usr/bin/time -v ./{benchmark}.exe [parameters] ; } 2>&1; } | tee result_{benchmark}_{params}.out && cat chrono_result.out >> result_{benchmark}_{params}.out
```

For example, we benchmark the Histogram-pthread benchmark:

```sh
{ { /usr/bin/time -v ./hist-pthread.exe histogram_datafiles/large.bmp ; } 2>&1; } | tee result_pthread_large.out && cat chrono_result.out >> result_pthread_large.out
```

where the datafiles are provided by the Phoenix author, [here](https://github.com/kozyraki/phoenix/blob/master/README.md).
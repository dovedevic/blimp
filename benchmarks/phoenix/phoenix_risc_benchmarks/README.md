# RISC-V Phoenix Benchmarks

The compiler and simulator we use in the paper does not provide a notion of threading or an operating system. As such, certain libraries, such as `mmap` and `pthreads` are not available. Regardless, in the paper we performed bank-level simulation on a single threaded version *acting as if* it were part of a multithreaded/multidispatched offload routine. 

The sources provided here are the modifications made to the Phoenix benchmarks to support RISC-V compilation and execution on our simulation framework. 

# Building a RISC-V-Compatible Benchmark

Using the [compiler linked and defined herein](https://github.com/dovedevic/blimp), one can compile each of the applications listed here as follows:

```sh
/path/to/riscv64-unknown-elf-gcc -std=c99 -march=rv64gc -g -O3 -Ipath/to/chronometry/c_h_and_o_files /path/to/stopwatch.o -lm -o risc-{benchmark}-pthread.elf risc-{benchmark}-pthread.c
```

# Running the ELF

Go to the [simulator folder](https://github.com/dovedevic/blimp) of this repository to see how to profile and run ELFs in our simulation framework.

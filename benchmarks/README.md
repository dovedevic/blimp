# Benchmarks

We investigate two benchmarks in the BLIMP paper; [SPEC CPU 2017](https://www.spec.org/cpu2017/) and the sample applications provided in the [Phoenix suite](https://github.com/kozyraki/phoenix). Each benchmark and the instructions to instrument, build, and run are provided herein. Since RISCV GCC/G++ does not yet support automatic vectorized code generation (at the time of this study), the kernels defined herein were used in place of those vectorizable regions in these larger applications using the timing data provided by the simulation and runtime environments.

Information on what to do with instrumented output is provided [here](https://github.com/dovedevic/blimp).

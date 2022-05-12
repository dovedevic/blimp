# RISC-V Vector Compilation

The provided GNU toolchain is provided for RISCV vector compilation (using GCC and G++). Follow the instructions there for installation.

When compiling a kernel for riscvOVPsim simulation, we use the following generalized compilation command:

```sh
/path/to/riscv64-unknown-elf-gcc -std=c99 -march=rv64gc -g -O3 -Ipath/to/chronometry/c_h_and_o_files /path/to/stopwatch.o -lm -o {benchmark}.elf {benchmark}.c
```

or

```sh
/path/to/riscv64-unknown-elf-gcc -std=c99 -march=rv64gcv -nostdlib -o {benchmark}.elf {benchmark}.sx
```

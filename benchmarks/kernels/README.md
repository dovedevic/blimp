# Kernel Benchmarking

To avoid rewriting entire benchmarks in RISC-V/V assembly, we use the following kernels with varying datasizes to simulate and extrapolate BLIMP-V runtimes in real-word applications. The provided benchmarks (with exception to `hello_world.sx`) require the `build_bank.py` script output, which will be detailed below. Additionally, all vector benchmarks run using the **0.9.x RISC-V Vector extension**. If you use an older or newer version, the `VS` bit in the `mstatus` register may change and require updating. 

# Directives

For each kernel benchmark, a number of preprocessor directives are included to easily define the size of a memory region, the number of trails to perform, and other specific kernel behavior. 

The following directives are provided *at compile time* using the `-D{directive}` switches.

## Trials
Trials are specified by the base 10 magnitude of trials to perform for ease of remembering:
 * (omitted) - Default, 1 trial
 * `T_10` - 32 trials
 * `T_100` - 512 trials
 * `T_1000` - 1,024 trials
 * `T_10000` - 16,384 trials
 * `T_100000` - 131,072 trials
 * `T_1000000` - 1,048,576 trials
 * `T_10000000` - 16,777,216 trials

## Operating Memory Size
The region size is specified by an absolute fraction of the bank size (32MB):
 * (omitted) - Default, 1 element, 4 bytes.
 * `S_1KB` - 256 elements, or 1KB
 * `S_1MB` - 262,144 elements, or 1MB
 * `S_4MB` - 1,048,576 elements, or 4MB
 * `S_8MB` - 2,097,152 elements, or 8MB
 * `S_16MB` - 4,194,304 elements, or 16MB
 * `S_32MB` - 8,388,608 elements, or 32MB

# Micro-benchmarks / Kernels

The following are descriptions on the micro-benchmarks:

## hello_world.sx

A dummy hello world application to test your compiler and simulator workflow.

## vmemset32.asm

32bit Vector Memory Set, or `vmemset32`, is a benchmark that sets all 32b elements in a specified region to a specific value. This value is defined by [line 64](https://github.com/dovedevic/blimp/blob/main/benchmarks/kernels/vmemset32.asm#L64), and is presently `0x0` (akin to a `memclear`). The processor will take a row-buffer-sized vector of elements from the bank, set them each to the value, and then store this new row back into the bank.

## vscan32.asm

32bit Vector Memory Scan, or `vscan32`, is a benchmark that searches for a specific 32b value within the memory region and stores the address of the hit in the last element of the array. This value is defined by [line 67](https://github.com/dovedevic/blimp/blob/main/benchmarks/kernels/vscan32.asm#L67), and is presently `0x2A` (akin to all of us at some point). The processor will take a row-buffer-sized vector of elements from the bank and search all against the value. If an element in the vector matches, the first occurrence of said element's address is stored in the last element of the memory array. If all elements were searched and the value was not found, an illegal address of `0xFFFF` is stored instead.

## vadd32.asm

32bit Vector Array Addition, or `vadd32`, is a benchmark that performs addition on a memory region. Depending on the operating memory size directive, `vadd32` has differing behavior. 

If the directive is `S_32MB`, only one memory region will fit in the bank. Because of this, `vadd32` will perform a destructive, per-element, self-addition routine. The result of this routine is akin to `for i in x; x[i] = x[i] + x[i];`.

If the directive is `S_16MB`, only two memory regions will fit in the bank. Because of this, `vadd32` will perform a result-destructive, per-element, addition routine. The result of this routine is akin to `for i in x, y; y[i] = x[i] + y[i];`.

If the directive is anything lower, three memory regions will fit in the bank. Because of this, `vadd32` will perform a per-element addition routine. The result of this routine is akin to `for i in x, y, z; z[i] = x[i] + y[i];`.

## vmatmul32.asm

32bit Vector Matrix Multiplication, or `vmatmul32`, is a benchmark that performs matrix multiplication on a memory region representing a square matrix that is laid out in row-then-column major order, padded with zeros. (The first row buffer row is filled with the first matrix row of values, when full continue to the next row buffer, when all matrix row values are consumed, pad the remaining row buffer with zeros. After all rows are placed in this manner, place all matrix columns, from top left to bottom, in the same manner.)

The memory region directives for this benchmark are slightly different to the standard definition as defined above.

### Trials

These are identical to what was defined before.

### Operating Memory Size
The region size is specified by an absolute fraction of the bank size (32MB) relative to a square matrix:
 * (omitted) - Default, 1 element, 4 bytes, 1x1 matrix
 * `S_256B` - 64 elements, 256B, 8x8 matrix
 * `S_1KB` - 256 elements, 1KB, 16x16 matrix
 * `S_64KB` - 16,384 elements, 64KB, 128x128 matrix
 * `S_256KB` - 65,536 elements, 256KB, 256x256 matrix.
 * `S_1MB` - 262,144 elements, 1MB, 512x512 matrix.
 * `S_4MB` - 1,048,576 elements, 4MB, 1024x1024 matrix.
 * `S_16MB` - 4,194,304 elements, 16MB, 2048x2048 matrix.
 
### Row Buffer Size
The row buffer size that the onboard bank processor is utilizing; i.e. the vector width:
 * (omitted) - Default, 1024B/1KB row buffer (8192b)
 * `R_512B` - 512B row buffer (4096b)
 * `R_2048B` - 2048B/2KB row buffer (16384b)

The benchmark will take one row-region and column-region row buffer row at a time, multiply them element wise, and aggregate them. This value will then be stored after the matrix-column rows region (or *destructively* in the row-column rows region if `S_16MB` is specified).

# Pre-compilation Source Generation

For all kernels that end in `*.asm`, a pre-compilation step is required to satisfy the compiler which pre-generates data assuming a relayout routine has already occurred. This pre-generation routine is handled by the `build_bank.py` script. 

This script takes the following positional arguments; `row_buffer_size` (in bytes), `number_of_regions` (integer), `region_size` (in bytes), and optionally a list of initial region values. The output of this program is an assembly data region which the benchmarks use.

For example, to generate a memory region which operates on an 8B row buffer, has 3 regions, each of which 16B in size, with the first region having bytes `0x1` and the second `0x2`, one would run: 

```sh
$ python3 build_bank.py 8 3 16 0x01 0x02
data_a:
    .byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
    .byte 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
data_b:
    .byte 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
    .byte 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
data_c:
    .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

```

The following commands are useful for common bank configurations:

 * One 32MB region for a 1KB-RB Bank:
   * `$ python3 build_bank.py 1024 1 33554432 > 32MB_1024_0x00.bank`
 * Two 16MB regions (total 32MB) for a 1KB-RB Bank with the first region having `0x01`:
   * `$ python3 build_bank.py 1024 2 16777216 0x01 > 32MB_16MB_16MB_1024_0x01_0x00.bank`
 * Three 8MB regions (total 24MB) for a 1KB-RB Bank with the first region having `0x01`, second region having `0x02`:
   * `$ python3 build_bank.py 1024 3 8388608 0x01 0x02 > 24MB_8MB_8MB_8MB_1024_0x01_0x02_0x00.bank`

Once a bank data configuration region is generated by this script; simply run the following command *prior* to compilation (defined below):

```sh
cat {benchmark}.asm {bank_configuration}.bank > {benchmark}.sx
```

**It is important that the resulting filename ends in `.sx` so GCC understands the file it is dealing with.**

# Building / Compiling a Kernel

After a benchmark is transformed from its `.asm` format to `.sx`, use the [compiler linked and defined herein](https://github.com/dovedevic/blimp) and run the following to compile the kernel to an ELF:

```sh
/path/to/riscv64-unknown-elf-gcc -std=c99 -march=rv64gcv -nostdlib -DT_{trails} -DS_{size} -o {benchmark}.elf {benchmark}.sx
```

\*\*If after compiling and running the ELF you get an error along the lines of "Misaligned 4-byte read", edit the `code_padding` region at the bottom of every benchmark `.asm` by adding or removing 'x's until you are aligned.


# Running the ELF

Go to the [simulator folder](https://github.com/dovedevic/blimp) of this repository to see how to profile and run ELFs in our simulation framework.

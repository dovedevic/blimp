# Relayout

The following source files emulate sending/fetching a word of data, byte by byte, to/from a particular bank assuming the DRAM architecture defined in the paper in a format compatible to BLIMP/CPU compute, respectively.

## Compiling

Compile the sources using any C/C++ compiler with no optimizations enabled. A simple compile command is:

```sh
$ g++ -o fast_to_bank fast_to_bank.cpp
$ g++ -o fast_from_bank fast_from_bank.cpp
```

## Running

The relayout application takes arguments as follows:

```
$ fast_to_bank [region_size_bytes] [trials]
$ fast_from_bank [region_size_bytes] [trials]
```

where `region_size_bytes` is the amount of data you wish to simulate offloading, and `trials` is the number of times you wish to perform this relayout. For small relayout procedures, such as those under 1MB, several hundred or thousand trials may be required to gather meaningful timing data.

Use the system time profiler to execute the application as follows:

```
$ /usr/bin/time -v fast_to_bank [region_size_bytes] [trials]
$ /usr/bin/time -v fast_from_bank [region_size_bytes] [trials]
```

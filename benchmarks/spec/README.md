# SPEC CPU 2017 Benchmarking

The SPEC CPU suite defines many benchmarks for which one could investigate offloadability to PIM. We use the [Chronometry package](https://github.com/dovedevic/blimp/tree/main/chronometry) for instrumentation and the SPEC `runcpu` orchestrator for timing data. One can use an automatic instrumentation routine (such as that used by a compiler we define in the paper) or done manually. The benchmarks to be instrumented are in `{spec_base}/spec_2k17/benchspec/CPU/{benchmark}/src`.

# Building/Running an Instrumented Benchmark

When a SPEC benchmark is instrumented, one can build as follows (example used is `505.mcf_r`):

```sh
$ {spec_base}/spec_2k17/bin/runcpu --action build --rebuild --config stopwatch-testing-configuration 505.mcf_r
```

The configuration used here is the one provided in [the configurations directory](https://github.com/dovedevic/blimp/blob/main/benchmarks/spec/configurations/stopwatch-testing-configuration.cfg).

To run a benchmark, use:

```sh
$ {spec_base}/spec_2k17/bin/runcpu --action run --rebuild --config stopwatch-testing-configuration 505.mcf_r
```

When the run is complete, the [Chronometry package](https://github.com/dovedevic/blimp/tree/main/chronometry) will place the output file in the `{spec_base}/spec_2k17/benchspec/CPU/{benchmark}/run/{run_version}/` directory.

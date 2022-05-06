# SPEC Build Configuration

The configuration in this directory should be placed in the `{spec_base}/spec_2k17/config/` directory. This configuration builds SPEC benchmarks with the [Chronometry package](https://github.com/dovedevic/blimp/tree/main/chronometry), assuming it has been built already with the compiler for which the benchmark will be compiled with. For example, if the SPEC benchmark to be compiled is with `gcc`, then ensure the Chronometry package is rebuilt with `gcc`; likewise for C++/`g++`. 

When using this configuration **make sure you edit [line 41](https://github.com/dovedevic/blimp/blob/main/benchmarks/spec/configurations/stopwatch-testing-configuration.cfg#L41)** to the path where your package is built. If you plan to submit SPEC results, ensure you update [lines 283-287](https://github.com/dovedevic/blimp/blob/main/benchmarks/spec/configurations/stopwatch-testing-configuration.cfg#L283-L287)

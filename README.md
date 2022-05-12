# Bank Level In Memory Processing - BLIMP

This repository serves as the point-of-entry to our work, ***To PIM or Not for Emerging General Purpose Processing in DDR Memory Systems***. Within, we include all benchmarks studied in the paper, instructions for instrumentation, compilation, execution, simulation, and evaluation. The full paper can be found in the proceedings of *The 49th Annual International Symposium on Computer Architecture (ISCA '22)*.

# Abstract

As Processing-In-Memory (PIM) hardware matures and starts making its way into normal compute platforms, software has an important role to play in determining what to perform where, and when, on such heterogeneous systems. Taking an emerging class of PIM hardware which provisions a general purpose (RISC-V) processor at each memory bank, this paper takes on this challenging problem by developing a software compilation framework. This framework analyzes several application characteristics -- parallelizability, vectorizability, data set sizes, and offload costs -- to determine what, whether, when and how to offload computations to the PIM engines. In the process, it also proposes a vector engine extension to the bank-level RISC-V cores. Using several off-the-shelf C/C++ applications, we demonstrate that PIM is not always a panacea, and a framework such as ours is essential in carefully selecting what needs to be performed where, when and how. The choice of hardware platforms -- number of memory banks, relative speeds and capabilities of host CPU and PIM cores, can further impact the ``to PIM or not'' question. 

# Repository Layout

## /benchmarks

This folder contains the two main benchmark suites we use; namely SPEC 2017 and the sample applications from Phoenix. We also include RISC-V/V assembly microkernels for vectorized regions. 

## /chronometry

This folder contains the region and application timing package which instruments inspected benchmarks.

## /compilation

This folder contains all compilers and methods needed to compile applications to be run in the simulation suite.

## /relayout

This folder contains simple relayout (for offload preambles and onload postambles) routines for benchmarking on real-world host systems.

## /simulation

This folder contains the simulation framework and supplementary scripts needed to evaluate benchmarks.

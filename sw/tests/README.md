Syntacore RISC-V SCR1 FPGA tests
========================================

About
--------------

Tests for SCR1 FPGA

Prerequisites
--------------

    riscv64 multilib toolchain
or

    riscv32 toolchain

Supported target platforms
--------------

    arty
    de10lite

Build SCR1 FPGA tests
--------------

We assume that the RISC-V environment variable is set to the RISC-V tools install path, and that the riscv-gnu-toolchain package is installed.

    i.e. export PATH=/opt/riscv-tools/bin:$PATH

To build tests execute:

    $ make

After the build process complets the target tests files are created in scr-sdk/images/arty, scr-sdk/images/de10lite.
Note that tests build for TCM memory, and must be load at adress 0xf0000000, and then run from 0xf0000200.

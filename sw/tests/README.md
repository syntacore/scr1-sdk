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

Target name | Description | Clock, MHz | TCM | RAM
------ | ----------- | --------- | ----- | -----
arty_scr1     | Digilent Arty A7  | 25 | 64K | -
de10lite_scr1 | Terasic DE10-lite | 20 | 64K | 64M
a5_scr1       | Intel Arria V GX FPGA Starter Kit | 30 | 128K | 256M

Build SCR1 FPGA tests
--------------

We assume that the RISC-V environment variable is set to the RISC-V tools install path, and that the riscv-gnu-toolchain package is installed.

    i.e. export PATH=/opt/riscv-tools/bin:$PATH

To build tests execute:

    $ make

After the build process complets the target tests files are created in scr-sdk/images/arty/scr1, scr-sdk/images/de10lite/scr1, scr-sdk/images/a5/scr1.
Note that tests build for TCM memory, and must be load at adress 0xf0000000, and then run from 0xf0000200.

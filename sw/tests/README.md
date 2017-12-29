Syntacore RISC-V SCR1 FPGA tests
========================================

About
--------------

Tests for SCR1 FPGA

Prerequisites
--------------

RISC-V GCC toolchain is required to compile the software. You can use pre-built binaries or build the toolchain from scratch.

#### Using pre-built binary tools

Pre-built RISC-V GCC toolchain and OpenOCD binaries are available to download from http://syntacore.com/page/products/sw-tools. Download the archive (*.tar.gz* for Linux, *.zip* for Windows) for your platform, extract the archive to your preferred directory and update the PATH environment variable as described in **Set environment variables** section.

#### Building tools from source

You can build the RISC-V toolchain from sources.

Build procedure is verified at the Ubuntu 14.04 LTS and Ubuntu 16.04 LTS distributions.

    sudo apt-get install autoconf automake libmpc-dev libmpfr-dev libgmp-dev gawk bison flex texinfo libtool make g++ pkg-config libexpat1-dev zlib1g-dev
    git clone https://github.com/riscv/riscv-gnu-toolchain.git
    cd riscv-gnu-toolchain
    git checkout a71fc539850f8dacf232fc580743b946c376014b
    git submodule update --init --recursive
    ./configure --prefix=<YOUR_INSTALL_PATH> --enable-multilib
    make

More detailed instructions on how to prepare and build the toolchain can be found in https://github.com/riscv/riscv-tools/blob/master/README.md.

#### Set environment variables

Add the <YOUR_INSTALL_PATH>/bin folder to the PATH environment variable:

    export PATH=$PATH:<YOUR_INSTALL_PATH>/bin

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

To build tests execute:

    $ make

After the build process complets the target tests files are created in scr-sdk/images/arty/scr1, scr-sdk/images/de10lite/scr1, scr-sdk/images/a5/scr1.
Note that tests build for TCM memory, and must be load at adress 0xf0000000, and then run from 0xf0000200.

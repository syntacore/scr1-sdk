# 'Hello' example project

## How to build a binary image of application using command-line tools

1. Check, and, if needed set the $(CROSS_PATH) environment variable to point to the location of where the RISC-V toolchain is installed:

```
$ export CROSS_PATH=<toolchain_path>
```

2. Build the application:


```
$ make [ARG=<value> ...] clean all
```

#### Arguments and values

Make can process the following optional arguments:

Argument | Description | Values
------ | ----------- | ---------
PLATFORM  | target platform     | **a5_scr1** **de10lite_scr1** **arty_scr1** **nexys4ddr_scr1**
OPT       | optimization preset | **0** (-O0), **2** (-O2), **3** (-O3), **g** (-Og -g3)

By default, PLATFORM=arty_scr1 and OPT=2 argument values are used

3. After the build process completes succesfully, the output files can be found in the subdirectory 'build.\*'.

4. By default, application is linked to run from the TCM address 0xF0000000, and can be directly loaded by the bootloader in the SCR1-SDK board. Please, refer to the tcm.ld file for additional details.

#------------------------------------------------------------------------------
# Makefile for SCR1 FPGA tests
#------------------------------------------------------------------------------

# Parameters
export ARCH ?= imfc

# Paths
export root_dir := $(shell pwd)/../../scr1
export inc_dir := $(root_dir)/tests/common
export bld_dir := $(root_dir)/build

test_results := $(bld_dir)/test_results.txt
test_info := $(bld_dir)/test_info

# Environment
export CROSS_PREFIX ?= riscv32-unknown-elf-

# Targets
.PHONY: fpga_sw

fpga_sw: dhrystone21_fpga_arty dhrystone21_fpga_de10lite

dhrystone21_fpga_arty:
	$(eval export SYS_CLK := 25000000)
	$(info arty_sys_clk=$(SYS_CLK))
	$(MAKE) dst_dir=$(root_dir)/../images/arty/scr1 OPT=3lto -f common.mk clean all 
	$(MAKE) dst_dir=$(root_dir)/../images/arty/scr1 OPT=2 -f common.mk clean all 

dhrystone21_fpga_de10lite:
	$(eval export SYS_CLK := 20000000)
	$(info de10lite_sys_clk=$(SYS_CLK))
	$(MAKE) dst_dir=$(root_dir)/../images/de10lite/scr1 OPT=3lto -f common.mk clean all
	$(MAKE) dst_dir=$(root_dir)/../images/de10lite/scr1 OPT=2 -f common.mk clean all 

clean:
	$(MAKE) -f common.mk clean

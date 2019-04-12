#------------------------------------------------------------------------------
# Makefile for SCR1 FPGA tests
#------------------------------------------------------------------------------

# Parameters
export ARCH ?= imfc

# Paths
export root_dir := $(CURDIR)
export inc_dir := $(root_dir)/tests_common
export bld_dir := $(root_dir)/build

test_results := $(bld_dir)/test_results.txt
test_info := $(bld_dir)/test_info

# Environment
export CROSS_PREFIX ?= riscv64-unknown-elf-

# Targets
.PHONY: fpga_sw

fpga_sw: dhrystone21_fpga_arty dhrystone21_fpga_de10lite dhrystone21_fpga_a5 dhrystone21_fpga_nexys

dhrystone21_fpga_arty:
	$(eval export SYS_CLK := 25000000)
	$(info arty_sys_clk=$(SYS_CLK))
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/arty/scr1 OPT=3lto clean all
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/arty/scr1 OPT=2 clean all

dhrystone21_fpga_de10lite:
	$(eval export SYS_CLK := 20000000)
	$(info de10lite_sys_clk=$(SYS_CLK))
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/de10lite/scr1 OPT=3lto clean all
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/de10lite/scr1 OPT=2 clean all

dhrystone21_fpga_a5:
	$(eval export SYS_CLK := 30000000)
	$(info a5_sys_clk=$(SYS_CLK))
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/a5/scr1 OPT=3lto clean all
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/a5/scr1 OPT=2 clean all

dhrystone21_fpga_nexys:
	$(eval export SYS_CLK := 30000000)
	$(info nexys_sys_clk=$(SYS_CLK))
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/nexys4ddr/scr1 OPT=3lto clean all
	$(MAKE) -C ./dhry21 ITERATIONS=500 dst_dir=$(root_dir)/../../images/nexys4ddr/scr1 OPT=2 clean all

clean:
	$(MAKE) -C ./dhry21 dst_dir=$(root_dir)/../../images/arty/scr1 clean
	$(MAKE) -C ./dhry21 dst_dir=$(root_dir)/../../images/a5/scr1 clean
	$(MAKE) -C ./dhry21 dst_dir=$(root_dir)/../../images/de10lite/scr1 clean
	$(MAKE) -C ./dhry21 dst_dir=$(root_dir)/../../images/nexys4ddr/scr1 clean

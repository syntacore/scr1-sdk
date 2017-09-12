# Makefile

.PHONY: all clean riscv
all: riscv

bmarks = dhry21

dst_dir          ?= $(CURDIR)
src_dir          := $(root_dir)/dhry21/
tests_common_dir := ../tests_common/

include $(tests_common_dir)tests_common.mk

$(info src_dir=$(src_dir))

VPATH += $(src_dir) $(tests_common_dir)

FLAGS_ = $(PORT_CFLAGS)
FLAGS_ += $(XLFLAGS)
FLAGS_ += $(XCFLAGS)

FLAGS_STR = "$(FLAGS_)"

CFLAGS = -static -Wa,-march=$(MARCH) -march=$(MARCH) -mabi=$(MABI) -std=gnu99 $(PORT_CFLAGS) $(XLFLAGS) -fno-common -fno-builtin-printf -DFLAGS_STR=\"$(FLAGS_STR)\"
LFLAGS = -march=$(MARCH) -mabi=$(MABI) -nostartfiles -nostdlib $(XLFLAGS) -lc -lgcc

CFLAGS += -DITERATIONS=$(ITERATIONS)

bmarks_defs += -DSELF_TIMED=1

build_dir ?= $(dst_dir)/

incs  += -I. -I$(src_dir) -I$(tests_common_dir)
objs  :=

dhry21_c_src = dhry_1.c dhry_2.c
dhry21_asm_src =

dhry21_c_objs   = $(patsubst %.c, $(build_dir)%.o, $(dhry21_c_src) $(common_c_src))
dhry21_asm_objs = $(patsubst %.S, $(build_dir)%.o, $(dhry21_asm_src) $(common_asm_src))

dhry21_elf = $(build_dir)dhry21$(build_siffix).elf
dhry21_bin = $(build_dir)dhry21$(build_siffix).bin
dhry21_map = $(build_dir)dhry21$(build_siffix).map

$(dhry21_elf): $(ld_script) $(dhry21_asm_objs) $(dhry21_c_objs)
	$(LD) $(incs) -o $@ -Wl,-Map=$(dhry21_map) -T $^ $(LFLAGS)
	$(OBJCOPY) -Obinary -S $@ $(dhry21_bin)
	$(RM) $(build_dir)*.o
	$(RM) $(dhry21_elf)
	$(RM) $(dhry21_map)

bmarks_riscv_elf = $(dhry21_elf)
bmarks_riscv_dump =$(build_dir)dhry21.$(build_siffix).dump

$(bmarks_riscv_dump): $(bmarks_riscv_elf)

%.dump: %.elf
	$(OBJDUMP) -w -x -d -s -z -S $< > $@

$(build_dir)%.o: $(src_dir)%.c | $(build_dir)
	$(CC) $(CFLAGS) $(bmarks_defs) -c $(incs) $< -o $@

$(build_dir)%.o: $(tests_common_dir)%.c | $(build_dir)
	$(CC) $(CFLAGS) $(bmarks_defs) -c $(incs) $< -o $@

$(build_dir)%.o: $(tests_common_dir)%.S | $(build_dir)
	$(CC) $(CFLAGS) $(bmarks_defs) -D__ASSEMBLY__=1 -c $(incs) $< -o $@

riscv: $(bmarks_riscv_elf) $(bmarks_riscv_dump)

$(build_dir):
	mkdir -p $(build_dir)

clean:
	rm -rf $(dhry21_elf) $(dhry21_bin) $(dhry21_map)

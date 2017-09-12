/*
*  Syntacore SCR* framework
*  @brief Bare metal tests/benchmarks linker script
*  @author mn-sc
*
* Copyright by Syntacore © 2017. ALL RIGHTS RESERVED.
*
*/

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
  TCM (rwx) : ORIGIN = 0xF0000000, LENGTH = 64K
}

STACK_SIZE = 2048;

SECTIONS {

  /*.text.crt ORIGIN(TCM) + 0x100 : {*/
  .text.crt ORIGIN(TCM) : {
    FILL(0);
    . = 0x100;
    *(.text.crt*)
  } >TCM

  . = ALIGN(4);
  /* __reloc_start = .; */

  .text : {
    PROVIDE(__TEXT_START__ = .);
    *(.text .text.*)
     PROVIDE(__TEXT_END__ = .);
  } >TCM

  .rodata : {
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
  } >TCM

  /* data segment */
  .data : {
    PROVIDE(__DATA_START__ = .);
    *(.data .data.*)
  } >TCM

  .sdata : {
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    . = ALIGN(4);
    PROVIDE(__DATA_END__ = .);
  } >TCM

  /* thread-local data segment */
  .tdata : {
    PROVIDE(_tls_data = .);
    PROVIDE(_tdata_begin = .);
    *(.tdata .tdata.*)
    . = ALIGN(4);
    PROVIDE(_tdata_end = .);
  } >TCM

  .tbss : {
    PROVIDE(_tbss_begin = .);
    *(.tbss .tbss.*)
    . = ALIGN(4);
    PROVIDE(_tbss_end = .);
  } >TCM

  /* bss segment */
  .sbss : {
    PROVIDE(__BSS_START__ = .);
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } >TCM

  .bss : {
    *(.bss .bss.*)
    . = ALIGN(4);
    PROVIDE(__BSS_END__ = .);
  } >TCM

  _end = .;
  PROVIDE(__end = .);

  /* End of uninitalized data segement */

  .stack ORIGIN(TCM) + LENGTH(TCM) - STACK_SIZE : {
    PROVIDE(__STACK_START__ = .);
    . += STACK_SIZE;
    PROVIDE(__C_STACK_TOP__ = .);
    PROVIDE(__STACK_END__ = .);
  } >TCM

  /DISCARD/ : {
    *(.eh_frame .eh_frame.*)
  }
}

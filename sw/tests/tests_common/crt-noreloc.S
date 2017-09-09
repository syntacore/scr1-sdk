/// Syntacore SCR* infra
///
/// @copyright (C) Syntacore 2015-2017. All rights reserved.
/// @author mn-sc
///
/// @brief crt

    // some useful macros
    .altmacro

    .macro save_reg_offs reg, offs, save_mem_base=zero
#ifdef __riscv64
    sd   \reg, \offs*8(\save_mem_base)
#else
    sw   \reg, \offs*4(\save_mem_base)
#endif // __riscv64
    .endm

    .macro save_reg regn, save_mem_base=zero
#ifdef __riscv64
    sd   x\regn, \regn*8(\save_mem_base)
#else
    sw   x\regn, \regn*4(\save_mem_base)
#endif // __riscv64
    .endm

    .macro load_reg_offs reg, offs, load_mem_base=zero
#ifdef __riscv64
    ld   \reg, \offs*8(\load_mem_base)
#else
    lw   \reg, \offs*4(\load_mem_base)
#endif // __riscv64
    .endm

    .macro load_reg regn, load_mem_base=zero
#ifdef __riscv64
    ld   x\regn, \regn*8(\load_mem_base)
#else
    lw   x\regn, \regn*4(\load_mem_base)
#endif // __riscv64
    .endm

    .macro save_regs reg_first, reg_last, save_mem_base=zero
    LOCAL regn
    regn = \reg_first
    .rept \reg_last - \reg_first + 1
    save_reg %(regn), \save_mem_base
    regn = regn+1
    .endr
    .endm

    .macro load_regs reg_first, reg_last, load_mem_base=zero
    LOCAL regn
    regn = \reg_first
    .rept \reg_last - \reg_first + 1
    load_reg %(regn), \load_mem_base
    regn = regn+1
    .endr
    .endm

#define TRAP_STACK_SPACE 512
#define TRAP_REGS_SPACE 512

    .globl _start
    .globl main
    .globl handle_trap
    .weak handle_trap

    // .text
    .option norvc
    .section ".text.crt","ax",@progbits
_tmp:
    .org (64*3)
    .align 6;
machine_trap_entry:
    j trap_entry

    .align 6
_start:
    la  t4, _tmp
    save_regs 1, 31, t4
    lui a4,0xff010
    li  a5, 'x'
    sw a5, 4(a4) 
    la  a0, machine_trap_entry
    csrw mtvec, a0

    la    a2, __BSS_START__

    // clear bss
21: la    a1, __BSS_END__
    j     4f
3:  sw    zero, 0(a2)
    add   a2, a2, 4
4:  bne   a1, a2, 3b

    auipc gp, %hi(_gp)
    addi  gp, gp, %lo(_gp)

    // init stack
    la    sp, __C_STACK_TOP__ - TRAP_REGS_SPACE
    csrw  mscratch, sp
    addi  sp, sp, -TRAP_STACK_SPACE
    // init hart0 TLS
    la    a0, _tdata_begin
    la    a2, _tbss_end
    sub   a1, a2, a0
    la    a4, __STACK_START__
    sub   tp, a4, a1
    // init tdata
    mv    a1, tp
    la    a2, _tdata_end
    j     6f
5:  lw    a3, 0(a0)
    sw    a3, 0(a1)
    add   a0, a0, 4
    add   a1, a1, 4
6:  bne   a0, a2, 5b
    // clear tbss
    j     8f
7:  sw    zero, 0(a1)
    add   a1, a1, 4
8:  bne   a1, a4, 7b
    
9:
    // start main
10: auipc t0, %pcrel_hi(main)
    // init argv=0, argc=0
    li a0, 0
    li a1, 0
    jalr  t0, %pcrel_lo(10b)
    la  t4, _tmp
    load_regs 1, 31, t4
    ret

handle_trap:
halt_loop:
sc_exit:
    wfi
    j     halt_loop

trap_entry:
    //csrrw sp, mscratch, sp 
    addi sp, sp, -TRAP_REGS_SPACE

    save_reg 1, sp
    save_regs 3, 31, sp
    addi a0, sp, TRAP_REGS_SPACE 
    save_reg_offs a0, 2, sp // save original sp

    csrr s0, mstatus
    //csrr s1, mepc

    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
1:  auipc t0, %pcrel_hi(handle_trap)
    jalr  t0, %pcrel_lo(1b)

    csrw mstatus, s0
    //csrw mepc, s1

    load_reg 1, sp
    load_regs 3, 31, sp

    //csrrw sp, mscratch, sp // restore original sp
    addi sp, sp, TRAP_REGS_SPACE
    mret

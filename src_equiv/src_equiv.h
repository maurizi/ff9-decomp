/* Hooks for src_equiv code that the port defines differently. The defaults reproduce the PS1 behaviour. */
#ifndef SRC_EQUIV_H
#define SRC_EQUIV_H

/* A pointer that went through the event-script value stack keeps only its low 26 bits (sign-extended);
 * the PS1 code rebuilds a KSEG0 address with | 0x80000000. */
#ifndef SCRIPT_PTR_RESTORE
#define SCRIPT_PTR_RESTORE(v) ((u32)(v) | 0x80000000)
#endif

/* GTE operations the src/ headers don't define. PS1 encodings; a port with a software GTE defines its own. */
#ifndef gte_ldrgbc
#define gte_ldrgbc(v) __asm__ volatile("mtc2\t%0, $6" : : "r"(v))
#endif
#ifndef gte_strgb2
#define gte_strgb2(r) __asm__ volatile("mfc2\t%0, $22" : "=r"(r))
#endif
#ifndef gte_dpcl
#define gte_dpcl() __asm__ volatile(GTE_NOPS ".word\t0x4A680029")
#endif

#ifndef gte_lddp
#define gte_lddp(v) __asm__ volatile("mtc2\t%0, $8" : : "r"(v))
#endif
#ifndef gte_nct
#define gte_nct() __asm__ volatile(GTE_NOPS ".word\t0x4AD80420")
#endif
#ifndef gte_dpcs
#define gte_dpcs() __asm__ volatile(GTE_NOPS ".word\t0x4A780010")
#endif
/* INTPL (sf=1): MAC = IR + (FC - IR) * IR0, i.e. IR moved toward the far colour FC by IR0 / 4096. */
#ifndef gte_intpl
#define gte_intpl() __asm__ volatile(GTE_NOPS ".word\t0x4A980011")
#endif
/* IR1 alone, MAC1 alone, and the red far-colour component RFC (control register 21). */
#ifndef gte_ldir1
#define gte_ldir1(v) __asm__ volatile("mtc2\t%0, $9" : : "r"(v))
#endif
#ifndef gte_stmac1
#define gte_stmac1(r) __asm__ volatile("mfc2\t%0, $25" : "=r"(r))
#endif
#ifndef gte_ldrfc
#define gte_ldrfc(v) __asm__ volatile("ctc2\t%0, $21" : : "r"(v))
#endif
#ifndef gte_strfc
#define gte_strfc(r) __asm__ volatile("cfc2\t%0, $21" : "=r"(r))
#endif
/* NCLIP result (MAC0) to a register. */
#ifndef gte_stopz_r
#define gte_stopz_r(r) __asm__ volatile("mfc2\t%0, $24" : "=r"(r))
#endif

/* RGB FIFO (RGB0-RGB2) to three words at r0. */
#ifndef gte_strgb3
#define gte_strgb3(r0) __asm__ volatile( \
    "swc2\t$20, 0(%0);" \
    "swc2\t$21, 4(%0);" \
    "swc2\t$22, 8(%0)" \
    : : "r"(r0) : "memory")
#endif

/* Register-level GTE access for routines that hand-written asm drives one register at a time (ovl_0b's battle
 * effect emitters keep results in GTE registers between commands). `reg` is a literal register number (data
 * registers 0-31 for MTC2/MFC2/LWC2/SWC2, control registers 0-31 for CTC2/CFC2), `op` the literal 32-bit COP2
 * command word (e.g. 0x4A280030 = RTPT). The values written are the full 32-bit words the original moves, also
 * where the register only keeps 16 bits (lwc2 of a word with padding, say), so that a model of the GTE that
 * hashes the whole register file (tools/equiv.py) sees the same state. PS1 encodings; a port with a software
 * GTE defines its own. */
#ifndef GTE_MTC2
#define GTE_MTC2(reg, v) __asm__ volatile("mtc2\t%0, $" #reg : : "r"((u32)(v)))
#define GTE_CTC2(reg, v) __asm__ volatile("ctc2\t%0, $" #reg : : "r"((u32)(v)))
#define GTE_MFC2(reg) ({ u32 _gte_v; __asm__ volatile("mfc2\t%0, $" #reg ";nop" : "=r"(_gte_v)); _gte_v; })
#define GTE_CFC2(reg) ({ u32 _gte_v; __asm__ volatile("cfc2\t%0, $" #reg ";nop" : "=r"(_gte_v)); _gte_v; })
#define GTE_CMD(op) __asm__ volatile(".word\t0;.word\t0;.word\t" #op)
#endif

/* The 1 KB scratchpad RAM. */
#ifndef SCRATCHPAD
#define SCRATCHPAD ((u8 *)0x1F800000)
#endif

#endif

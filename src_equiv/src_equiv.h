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
/* RGB FIFO (RGB0-RGB2) to three words at r0. */
#ifndef gte_strgb3
#define gte_strgb3(r0) __asm__ volatile( \
    "swc2\t$20, 0(%0);" \
    "swc2\t$21, 4(%0);" \
    "swc2\t$22, 8(%0)" \
    : : "r"(r0) : "memory")
#endif

/* The 1 KB scratchpad RAM. */
#ifndef SCRATCHPAD
#define SCRATCHPAD ((u8 *)0x1F800000)
#endif

#endif

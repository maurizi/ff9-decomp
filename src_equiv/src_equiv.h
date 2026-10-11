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

#endif

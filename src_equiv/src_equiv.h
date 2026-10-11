/* Hooks for src_equiv code that the port defines differently. The defaults reproduce the PS1 behaviour. */
#ifndef SRC_EQUIV_H
#define SRC_EQUIV_H

/* A pointer that went through the event-script value stack keeps only its low 26 bits (sign-extended);
 * the PS1 code rebuilds a KSEG0 address with | 0x80000000. */
#ifndef SCRIPT_PTR_RESTORE
#define SCRIPT_PTR_RESTORE(v) ((u32)(v) | 0x80000000)
#endif

#endif

/* STATUS: near miss, 3 rows. ovl_39 func_801F973C (0x4C, CRC-16 over the save block). With a u16 crc the loop is right except that the crc & 0xFFFF before the shift is scheduled first, and the original masks again at the return (andi v0,v1,0xFFFF) where we emit a move; with an int crc gcc drops the in-loop andi (9 rows). The original seems to carry crc in a wider type whose upper bits gcc could not prove zero. Pass 4 tried (bounded search, build/g39*.py in the agent worktree): crc in u16/s16/s32/u32, init 0xFFFF/-1, 8 spellings of the update, 3 return casts, while/for loops, an uninitialised-on-entry crc, and a static inline update helper with u16/s32 parameters; the best is the inline helper 'static inline u16 upd(u16 crc, s32 b)' with an s32 crc (2 rows: both andi missing). */
#include "ovl_39.h"
extern u16 D_801FC3E0[256];

u16 func_801F973C(u8 *p, s32 n) {
    u16 crc = 0xFFFF;

    while (--n >= 0) {
        crc = (crc >> 8) ^ D_801FC3E0[(crc ^ *p++) & 0xFF];
    }
    return crc;
}

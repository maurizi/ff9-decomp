/* Text bank lookups: text_setBank, text_get, text_getInfo, text_getFrom (main 0x8003575C-0x800357DF).
 *
 * Hand-written asm in the original (addi, no frame); the standard C ABI otherwise. A bank is
 *     u32 count; struct { u16 offset; s16 info; } entry[count]; strings...
 * and offsets are relative to the entry table (bank + 4), not to the bank start.
 */
#include "ff9.h"

void text_setBank(const u8 *bank) {
    g_textBank = (s32)bank;
}

/* The bank's string `id`. */
u8 *text_get(s32 id) {
    u8 *tbl = (u8 *)g_textBank + 4;
    return tbl + (((u32 *)tbl)[id] & 0xFFFF);
}

/* The high half of entry `id`, sign-extended (sra 16). */
s32 text_getInfo(s32 id) {
    u8 *tbl = (u8 *)g_textBank + 4;
    return (s32)((u32 *)tbl)[id] >> 16;
}

/* text_get on an explicit bank. */
s32 text_getFrom(u8 *bank, s32 id) {
    u8 *tbl = bank + 4;
    return (s32)(tbl + (((u32 *)tbl)[id] & 0xFFFF));
}

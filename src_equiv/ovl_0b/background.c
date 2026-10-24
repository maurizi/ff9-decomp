/* ovl_0b battle background drawing (0x800CEED4-0x800CF5A4), hand-written asm in the original.
 *
 * g_btlbgBrightness (0x800CF588) and g_btlbgOtShift (0x800CF5A0) are data words inside .text, between the
 * functions that set them (DS port F33); a port gives them ordinary storage.
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

/* Standard C ABI. */
void btlbg_setBrightness(s32 v) {
    g_btlbgBrightness = v;
}

s32 btlbg_getBrightness(void) {
    return g_btlbgBrightness;
}

void btlbg_setOtShift(s32 v) {
    g_btlbgOtShift = v;
}

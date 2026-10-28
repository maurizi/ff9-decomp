/* mbg_getStatus (main), filed in config/library_funcs.us.txt as an "SN startup helper" but called from game code
 * (ovl_14, ovl_15, ovl_0a, ovl_64: the movie paths) as a status query; tested with & 2. Which library it belongs
 * to is not established. Standard C ABI. */
#include "ff9.h"

extern s8 D_80071E30;
extern s8 g_mbgRgb24;
extern s32 g_mbgFrame;
extern s8 g_mbgPaused;

s32 mbg_getStatus(void) {
    s32 r;

    if (g_mbgState == 1) {
        return 1;
    }
    if (g_mbgState < 2) {
        return 0;
    }
    r = g_mbgFrame > 0 ? 3 : 1;
    if (g_mbgPaused != 0) {
        r |= 0x10;
    }
    if (g_mbgRgb24 != 0) {
        r |= 4;
    }
    if (D_80071E30 == 1) {
        r |= 8;
    }
    return r;
}

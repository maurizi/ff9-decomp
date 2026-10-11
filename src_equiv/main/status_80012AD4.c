/* func_80012AD4 (main), filed in config/library_funcs.us.txt as an "SN startup helper" but called from game code
 * (ovl_14, ovl_15, ovl_0a, ovl_64: the movie paths) as a status query; tested with & 2. Which library it belongs
 * to is not established. Standard C ABI. */
#include "ff9.h"

extern s8 D_80071E30;
extern s8 D_80071E31;
extern s32 D_80071E34;
extern s8 D_80071E38;

s32 func_80012AD4(void) {
    s32 r;

    if (D_80071E32 == 1) {
        return 1;
    }
    if (D_80071E32 < 2) {
        return 0;
    }
    r = D_80071E34 > 0 ? 3 : 1;
    if (D_80071E38 != 0) {
        r |= 0x10;
    }
    if (D_80071E31 != 0) {
        r |= 4;
    }
    if (D_80071E30 == 1) {
        r |= 8;
    }
    return r;
}

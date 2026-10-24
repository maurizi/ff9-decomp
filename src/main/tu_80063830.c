#include "ff9.h"

s32 func_80063830(s32 id) {
    DbEntry12 *e = (DbEntry12 *)func_800633E4(id);

    if (e != NULL) {
        return e->unk7;
    }
    return 0;
}

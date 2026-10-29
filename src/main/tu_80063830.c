#include "ff9.h"

s32 db_getModelTexCount(s32 id) {
    DbEntry12 *e = (DbEntry12 *)db_findModelInfo(id);

    if (e != NULL) {
        return e->unk7;
    }
    return 0;
}

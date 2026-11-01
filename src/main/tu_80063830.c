#include "ff9.h"

s32 db_getModelTexCount(s32 id) {
    DbEntry12 *modelInfo = (DbEntry12 *)db_findModelInfo(id);

    if (modelInfo != NULL) {
        return modelInfo->texCount;
    }
    return 0;
}

#include "ff9.h"

u16 *db_findModelInfo(s32 id) {
    s32 i;
    s32 j;
    u8 *blk;
    DbEntry12 *tab;
    DbEntry12 *entry;

    for (i = g_dbList.count - 1; i >= 0; i--) {
        blk = (u8 *)db_lookup((s32)g_dbList.dbs[i], DB_GET_AT, DB_MODEL_INFO, 0);
        if (blk != (u8 *)-1) {
            j = blk[1] - 1;
            tab = (DbEntry12 *)(blk + 4);
            for (; j >= 0; j--) {
                entry = &tab[j];
                if (entry->id == (u16)id) {
                    return (u16 *)entry;
                }
            }
        }
    }
    return NULL;
}

u8 *db_getModelTexPages(s32 modelId) {
    u16 *info = db_findModelInfo(modelId);
    return (u8 *)info + (((u32 *)info)[1] & 0xFFFFFF);
}

u8 *db_getModelUvOffsets(s32 modelId) {
    u8 *info = (u8 *)db_findModelInfo(modelId);
    return info + (((u32 *)info)[1] & 0xFFFFFF) + info[7] * 4;
}

u8 *db_getModelTexPos(s32 modelId) {
    DbEntry12 *info = (DbEntry12 *)db_findModelInfo(modelId);
    s32 texCount = info->texCount;
    u8 *data = (u8 *)info + info->dataOfs;
    return (long long)texCount * 8 + data;
}

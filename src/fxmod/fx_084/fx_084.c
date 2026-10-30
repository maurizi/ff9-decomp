#include "fxmod/fx_084.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7B90);
    s32 t = frame << 8;
    s32 a;

    ex->btlfx_lerpVec((s16 *)&D_801E7B98, (s16 *)&D_801E7BA0, t, (s16 *)pos);
    a = t >> 1;
    pos->vy -= (ex->math_sin(a) * 0x180) >> 12;
    switch (pt->unk1F) {
    case 1:
        pos->vy -= (ex->math_sin(a) << 9) >> 12;
        break;
    case 3:
        pos->vy += (ex->math_sin(a) << 9) >> 12;
        break;
    }
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= 0x10) {
        return -1;
    }
    return 0;
}

s32 fx_084_op80(s32 mode, Fx084Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E7B90 = env;
        work->unk0 = 0;
        D_801E7BA8 = entry;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        BTLFX_COPY_SVEC(&D_801E7B98, &work->unk4);
    } else {
        frame = entry->frame;
        if (ex->btlfx_isEnemyUnit(0x10) == 0) {
            ex->btlfx_getUnitBodyPos(0x10, 1, (s16 *)&work->unkC);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unkC);
        }
        BTLFX_COPY_SVEC(&D_801E7BA0, &work->unkC);
        switch (work->unk0) {
        case 0:
            work->unk0 = 1;
            entry->frame = -1;
            work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E7B98);
            if (work->unk1C != NULL) {
                work->unk1C->unk22 = 0x10;
            }
            if (work->unk1C != NULL) {
                ((EffMotion *)work->unk1C->file)->unk8 = func_801E7704;
            }
            break;
        case 1:
            if (frame >= 0x10) {
                work->unk0 = 2;
                entry->frame = -1;
                work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unkC);
                if (work->unk20 != NULL) {
                    work->unk20->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x18);
                }
                ex->btlfx_aimAngles((s16 *)&work->unkC, (s16 *)&work->unk4, (s16 *)&work->unk14);
            }
            break;
        case 2:
            if (work->unk20 != NULL) {
                if (work->unk20->state == -1) {
                    work->unk20 = NULL;
                }
                if (work->unk20 != NULL) {
                    work->unk20->unk5C.vx = work->unk14.vx;
                    work->unk20->unk5C.vy = work->unk14.vy;
                    work->unk20->unk5C.vz = work->unk14.vz;
                }
            }
            if (frame == 6) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x18);
                }
            }
            if (frame >= 0x2C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_084/nonmatchings/fx_084", fx_084_headerPtr);

/* Draft of fx_026 op80. Status: 1 instruction short: at the switch dispatch the original copies k (s7, still 0)
 * into s (s3) with 'addu s3,s7,zero' in the delay slot of 'beq v1,v0,case1'; every source form tried lets CSE fold
 * s to 0 instead. */
#include "fxmod/fx_026.h"

s32 fx_026_op80(s32 mode, Fx026Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    Pool6CEntry *q;
    Fx026Item *it;
    s32 frame;
    s32 k;
    s32 s;
    s32 t;

    if (mode == 0) {
        init->unk0 = 0x70;
        init->unk8 = 0x14;
        init->unk14 = (s32)func_801E7A5C;
        init->unk4 = 6;
    } else {
    k = 0;
    if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EACCC = env;
        ex->btlfx_initEffModels(D_801EACDC, 0x20);
        work->unk58.vy = 0x800;
        work->unk4C.vx = 0x1000;
        work->unk4C.vy = 0x1000;
        work->unk4C.vz = 0x1000;
        work->unk58.vx = 0;
        work->unk58.vz = 0;
        work->unk40.vx = 0;
        work->unk40.vy = 0x2000;
        work->unk40.vz = 0;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, 0xB), 2, &p);
        p.vy = 0;
        work->unk10.vx = p.vx;
        work->unk10.vy = p.vy;
        work->unk10.vz = p.vz;
        work->unk64 = 0;
    } else {
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        if (frame == 0) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 40) {
            work->unk0 = 1;
            entry->unk0 = -1;
            ex->Hi_SetSummonMotion(((Fx026Summon *)env->unk4)->unk54->unk180, ((Fx026Summon *)env->unk4)->unk60);
            ex->Hi_SetSummonMotFrame(((Fx026Summon *)env->unk4)->unk60, 0);
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->unk4)->unk60, 0, 0);
        }
        break;
    case 1:
        work->unk40.vx = env->unk2C->unk0[2].vx;
        work->unk40.vy = env->unk2C->unk0[2].vy;
        work->unk40.vz = env->unk2C->unk0[2].vz;
        if (frame >= 2) {
            ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                                   ((Fx026Summon *)env->unk4)->unk60, 0);
            ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0, (u16 *)&work->unk28);
        }
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, 0xB), work->unk4, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->unk2C->unk0[2].vx = p.vx;
        env->unk2C->unk0[2].vy = p.vy;
        env->unk2C->unk0[2].vz = p.vz;
        if (frame == 38) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA2), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 42) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA3), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 66) {
            work->unk0 = 2;
            entry->unk0 = -1;
            D_801EC5DC = work->unk68 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 0);
            work->unk6C = D_801EC5E0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 1);
        }
        break;
    case 2:
        work->unk40.vx = env->unk2C->unk0[2].vx;
        work->unk40.vy = env->unk2C->unk0[2].vy;
        work->unk40.vz = env->unk2C->unk0[2].vz;
        t = (frame << 12) / 50;
        ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                               ((Fx026Summon *)env->unk4)->unk60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0, (u16 *)&work->unk28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0x19, (u16 *)&work->unk30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0x12, (u16 *)&work->unk38);
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, 0xE), frame, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->unk2C->unk0[2].vx = p.vx;
        env->unk2C->unk0[2].vy = p.vy;
        env->unk2C->unk0[2].vz = p.vz;
        if (frame == 0) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA1), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 29) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA6), (s32)&work->unk30);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame == 32) {
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->unk4)->unk60, 1, 0);
        }
        k = t;
        s = (k >> 1) + 0x800;
        if (frame >= 50) {
            work->unk0 = 3;
            entry->unk0 = -1;
            ex->func_800E3C58();
            ex->btlfx_poolClear(entry->unk10);
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA4), (s32)&work->unk28);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        break;
    case 3:
        if (frame == 0) {
            q = (Pool6CEntry *)ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA5), (s32)&work->unk30);
            if (q != NULL) {
                q->unk22 = 0x40;
            }
        }
        if (frame >= 4 && frame < 10) {
            it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (it != NULL) {
                it->unk4 = frame - 4;
                it->unk2 = ex->btlfx_rand();
            }
        }
        work->unk40.vx = env->unk2C->unk0[2].vx;
        work->unk40.vy = env->unk2C->unk0[2].vy;
        work->unk40.vz = env->unk2C->unk0[2].vz;
        ex->Hi_DrawSummonModel(&work->unk58, (VECTOR *)&work->unk40, (s32)&work->unk4C,
                               ((Fx026Summon *)env->unk4)->unk60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0, (u16 *)&work->unk28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0x19, (u16 *)&work->unk30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->unk4)->unk60, 0x12, (u16 *)&work->unk38);
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, 0x11), frame, &p);
        work->unk40.vx = p.vx;
        work->unk40.vy = p.vy;
        work->unk40.vz = p.vz;
        env->unk2C->unk0[2].vx = p.vx;
        k = 0x1000;
        env->unk2C->unk0[2].vy = p.vy;
        env->unk2C->unk0[2].vz = p.vz;
        s = k;
        if (frame >= 29) {
            ex->btlfx_poolClear(entry->unk10);
            return 1;
        }
        break;
    }
    if (k != 0) {
        s = s * 2 / 3;
        if (work->unk4 & 1) {
            s = s * 9 >> 3;
        }
        env->unk28 = 0x40;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA0), &work->unk30, 0, s, 0, 1, k);
        env->unk28 = 0;
    }
    work->unk4++;
    }
    }
    return 0;
}

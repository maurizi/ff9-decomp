/* Draft of fx_188 fx_188_op80. Status: about 26 strict mismatches: the original loads work->unk1C with lw (not
 * narrowed to lhu) for the & 3 and keeps frame in s3. */
#include "fxmod/fx_188.h"

s32 fx_188_op80(s32 mode, Fx188Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx188Elem *e;
    SVECTOR v;
    Pool6CEntry *p;
    s32 frame;
    s32 s;
    s32 t;

    if (mode == 0) {
        arg4->unk0 = 0x20;
        arg4->unk8 = 0x28;
        arg4->unk14 = (s32)func_801E7704;
        arg4->unk4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk1C = 0;
        D_801E7E88 = env;
        BTLFX_COPY_SVEC(&work->unk4, &env->unkC->unk38);
        work->unk4.vy -= 0x200;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        D_801E86CC = 0x1000;
        work->unk14.vy += 0x800;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame & 1) {
                if (work->unk1C < 0xC) {
                    work->unk1C++;
                    e = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (e != NULL) {
                        BTLFX_COPY_SVEC(&e->unkC, &work->unkC);
                        e->unk20.vx = work->unk4.vx;
                        e->unk20.vz = work->unk4.vz;
                        e->unk20.vy = work->unk4.vy;
                        e->unk14.vy = work->unk14.vy - 0x300 + (((u32)work->unk1C & 3) << 9);
                        e->unk14.vz = 0;
                        e->unk14.vx = work->unk14.vx + (work->unk1C >> 2) * 0x180;
                    }
                }
            }
            if (frame >= 0x3E) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = frame << 9;
            if (frame < 8) {
                s = 0x1000;
                D_801E86CC = ex->math_cos(frame << 7);
            } else {
                t = 0x1000;
                s = ex->math_cos((frame - 8) << 7);
                D_801E86CC = 0;
            }
            s = s * 3 + 0x400;
            v.vx = 0x400;
            v.vz = 0;
            v.vy = 0;
            v.pad = 1;
            env->unk28 = 0x20;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk4, &v, s, 0, 1, t);
            env->unk28 = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xF), frame);
            if (frame >= 0x10) {
                work->unk0 = 2;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E7E98;
                    p->unk12 = 0x19;
                    p->unk22 = 0x20;
                    p->unkC = 0x54;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            break;
        case 2:
            if (frame >= 0x20) {
                work->unk0 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

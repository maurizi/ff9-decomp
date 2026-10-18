#include "fxmod/fx_0e6.h"

s32 fx_0e6_op80(s32 mode, Fx0e6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 n;
    Fx0e6Line *line;
    SVECTOR *v;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E7A88 = env;
        ex->btlfx_initEffModels(D_801E7A98, 1);
        work->unk24.vx = 0;
        work->unk24.vy = 0;
        work->unk24.vz = 0;
        work->unk4.vx = 0x1000;
        work->unk4.vy = 0x1000;
        work->unk4.vz = 0x1000;
        work->unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0xF7, 0x3D00, 0, 0);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk2C);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk34);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), 0, 0, -1, 0x1000, 0x1000, mode,
                          (u8 *)&D_801E7B68, 0);
    } else {
        frame = entry->unk0;
        ex->btlfx_lerpVec((s16 *)&work->unk2C, (s16 *)&work->unk34, (frame << 22) >> 12, (s16 *)&pos);
        work->unk14.vx = pos.vx;
        work->unk14.vy = pos.vy;
        work->unk14.vz = pos.vz;
        if (frame < 4) {
            ex->Hi_DrawEffModel(&work->unk24, &work->unk14, (s32)&work->unk4, work->unk0);
        }
        if (frame < 8) {
            n = 25;
            line = &D_801E7B68;
            line->unk2 = 0x18;
            line->unk0 = 0x18;
            line->unk1 = 0;
            v = line->unk4;
            for (i = 0; i < n; i++) {
                ex->btlfx_lerpVec((s16 *)&work->unk2C, (s16 *)&pos, (0x1000000 / n * i) >> 12, (s16 *)v);
                v->pad = 0x20;
                v++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), 0, 0, 0, 0x1000, (8 - frame) << 9, 1,
                              (u8 *)line, 0);
        }
        if (frame == 4) {
            Pool6CEntry *p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk34);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e6/nonmatchings/fx_0e6", fx_0e6_headerPtr);

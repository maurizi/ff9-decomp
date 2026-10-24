#include "fxmod/fx_1ad.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = (frame << 12) / frames;
    s32 row = (e->unk1F - 1) % 6;
    s32 v = t * D_801E7CD8[row][0];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D8C);
    s32 frac = v & 0xFFF;
    s32 i = v / 4096;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;

    ex->func_800DC818(0x10, D_801E7CD8[row][i + 1], (u16 *)&a);
    ex->func_800DC818(0x10, D_801E7CD8[row][i + 2], (u16 *)&b);
    ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, frac, (s16 *)&c);
    pos->vx = c.vx;
    pos->vy = c.vy;
    pos->vz = c.vz;
    e->unk0.vx = c.vx << 12;
    e->unk0.vy = c.vy << 12;
    e->unk0.vz = c.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1ad_op80(s32 mode, Fx1adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    MATRIX m;
    Pool6CEntry *p;
    s32 frame;
    s32 c;
    s32 t;
    s32 k;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E7D8C = env;
        ex->btlfx_initEffModels(D_801E7D9C, 1);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk24 = (s16)ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
    } else {
        frame = entry->unk0;
        ex->func_800DC818(0x10, 0xF, (u16 *)&work->unk8);
        pos.vx = work->unk8.vx;
        pos.vy = work->unk8.vy;
        pos.vz = work->unk8.vz;
        if (work->unk0 == 0) {
            k = 0x40;
            n = work->unk4;
            ex->btlfx_drawScrollMove(n * 4, 0x200, 0x100, 0x20, k, ((n & 3) << 5) + 0x240, ((n * 16) & 0xC0) + 0x100);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 1), work->unk4);
            if (frame == 0) {
                work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk10);
                if (work->unk20 != NULL) {
                    work->unk20->unk14 = (s32)D_801E7E6C;
                    work->unk20->unk12 = 0x48;
                    work->unk20->unkC = 0x3C;
                }
                if (work->unk20 != NULL) {
                    work->unk20->unk22 = k;
                }
                if (work->unk20 != NULL) {
                    ((EffMotion *)work->unk20->unk0)->unk8 = func_801E7704;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E8F4C;
                    p->unk12 = 0x10;
                    p->unkC = 0x6C;
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0xC) {
                t = ((frame - 0xC) << 12) / 57;
                c = ex->math_cos((frame * 0x52 + 0x200) & 0x3FE);
                scale.vz = 0x1800;
                scale.vx = scale.vy = c / 3 + 0x1800;
                ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m);
                ex->func_800CFF40(&m, (s16 *)&rot);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
                ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                         (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80);
            }
            if (frame >= 0x45) {
                work->unk0 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ad/nonmatchings/fx_1ad", fx_1ad_headerPtr);

#include "fxmod/fx_04f.h"

s32 func_801E7704(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 a;
    s32 c;
    s32 s;
    s32 *q;
    s32 c2;

    if (mode == 1) {
        *it = 0;
    } else {
        a = ((frame << 12) / 9) >> 1;
        c = ex->math_sin(a);
        if (frame < 5) {
            c2 = a + 0x200;
            s = ((ex->math_sin((frame << 10) >> 2) * (work->unk4C >> 1)) >> 12) + c2;
        } else {
            c2 = a + 0x200;
            s = (work->unk4C >> 1) + *(q = &c2); /* the address taken changes c2's register (permuter) */
        }
        scale.vy = s;
        scale.vx = s;
        scale.vz = (work->unk50 * 7) >> 3;
        BTLFX_COPY_SVEC(&rot, &work->unk38);
        rot.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&rot, *it, (s16 *)&p);
        pos.vx = p.vx;
        pos.vy = p.vy;
        pos.vz = p.vz;
        *it -= 0x18;
        ex->Hi_DrawEffModel(&work->unk38, &pos, (s32)&scale, work->unk4);
        ex->Hi_ModifyEffModelRGB(work->unk4, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (frame >= 9) {
            return 1;
        }
    }
    return 0;
}

s32 fx_04f_op80(s32 mode, Fx04fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    s32 k;
    s32 t;
    VECTOR scale;
    SVECTOR v;
    s32 q;

    if (mode == 0) {
        init->workSize = 0x54;
        init->elemSize = 4;
        init->elemFn = (s32)func_801E7704;
        init->poolCount = 3;
    } else {
        if (mode == 1) {
        D_801E7F18 = env;
        ex->btlfx_initEffModels(D_801E7F28, 4);
        work->unk4 = (s16)ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB));
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 6), 0x117, 0xFFFF, 0, 0x80);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xC), 0x117, 0xFFFF, 0, 0);
        work->unk0 = 0;
        work->unk48 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk38);
        work->unk40 = ex->btlfx_getUnitRadius(0);
        ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&work->unk38, work->unk40, (s16 *)&work->unk14);
        ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&work->unk38, 0x180, (s16 *)&work->unk24);
        work->unk38.vy += 0x800;
        work->unk2C[0] = work->unk14.vx;
        work->unk2C[1] = work->unk14.vy;
        work->unk2C[2] = work->unk14.vz;
        BTLFX_COPY_SVEC(&work->unk1C, &work->unk14);
    } else {
        r = 0;
        g = 0;
        b = 0;
        k = 0;
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            r = frame << 9;
            b = 0;
            g = 0;
            scale.vx = scale.vy = ((0x2000 - ex->math_sin(r >> 2)) * 0xC00) >> 12;
            scale.vz = 0;
            if (frame >= 8) {
                work->unk0 = 1;
                entry->frame = -1;
                work->unk44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk1C);
                if (work->unk44 != NULL) {
                    work->unk44->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            break;
        case 1:
            q = (frame << 12) / 20;
            if (frame < 10) {
                r = 0x1000;
                b = (frame << 12) / 10;
                g = b;
            } else {
                b = 0x1000;
                g = 0x1000;
                r = 0x1000;
            }
            k = q;
            t = k * 0xC00;
            scale.vy = (t >> 12) + 0xC00;
            scale.vx = (t >> 12) + 0xC00;
            scale.vz = t >> 11;
            if (frame % 5 == 0) {
                ex->btlfx_poolSpawn((s16 *)entry->pool);
            }
            if (work->unk44 != NULL) {
                if (work->unk44->unk30 == -1) {
                    work->unk44 = NULL;
                } else {
                    work->unk38.vy += 0x800;
                    ex->btlfx_pointAhead((s16 *)&work->unk14, (s16 *)&work->unk38, frame * 0x12, (s16 *)&v);
                    work->unk38.vy += 0x800;
                    ex->btlfx_moveEmitterTo((s32)work->unk44, (s32)&v);
                }
            }
            if (frame >= 0x14) {
                work->unk0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            scale.vy = 0x1800;
            scale.vx = 0x1800;
            scale.vz = 0x1800;
            q = (frame << 12) / 12;
            b = 0x1000 - q;
            g = b;
            r = b;
            k = b;
            if (frame >= 0xD) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4C = scale.vx;
        work->unk50 = scale.vz;
        work->unk38.vz = 0;
        ex->Hi_DrawEffModel(&work->unk38, (VECTOR *)work->unk2C, (s32)&scale, work->unkC);
        ex->Hi_ModifyEffModelAbr(work->unkC, 1);
        ex->Hi_ModifyEffModelRGB(work->unkC, (r >> 5) - 0x80, (g >> 5) - 0x80, (b >> 5) - 0x80);
        work->unk38.vz = -work->unk48 * 0x18;
        ex->Hi_DrawEffModel(&work->unk38, (VECTOR *)work->unk2C, (s32)&scale, work->unk10);
        ex->Hi_ModifyEffModelAbr(work->unk10, 1);
        ex->Hi_ModifyEffModelRGB(work->unk10, (k * 2 / 3 >> 5) - 0x80, (k * 2 / 3 >> 5) - 0x80, (k * 2 / 3 >> 5) - 0x80);
        work->unk38.vz = 0;
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), work->unk48);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xD), work->unk48);
        work->unk48++;
    }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04f/nonmatchings/fx_04f", fx_04f_headerPtr);

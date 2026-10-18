/* Draft of fx_07d fx_07d_op80 (first draft, adapted from the fx_0df op80 draft in the partition holding fx_0df; the
 * two are 0.82 similar, fx_07d has an extra case 0 with the screen-ripple call and sxy clamping). Status: 1099
 * aligned mismatches, 477 ignoring registers. Open: the original keeps work in fp, ex in s5, frame in s7 and spills
 * case 0's t to 0x70 (frame 0xA0; ours 0x90, no spill); (t * 0x64) >> 12 is computed before the GTE block; the
 * per-case stack layout (case 3's rot3 at 0x68, case 2 pos 0x48 / scale 0x38 / rot2 0x58 / rot 0x60). */
#include "fxmod/fx_07d.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* ordering-table offset */
    SVECTOR unkC; /* caster body position */
    s32 unk14; /* caster body position, words */
    s32 unk18;
    s32 unk1C;
    s32 unk20; /* models */
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 pad30;
    s32 unk34; /* scroll */
    s32 unk38; /* yaw offset */
    Pool6CEntry *unk3C;
} Fx07dWork; /* 0x40 */

typedef struct {
    s16 unk0;
    s16 unk2; /* size */
    SVECTOR unk4; /* position */
    SVECTOR unkC; /* rotation */
    u16 unk14;
    u16 unk16;
} Fx07dPart;

extern BtlModel D_801E9FC0[];

s32 fx_07d_op80(s32 mode, Fx07dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Fx07dPart *part;
    s32 frame;
    s32 t;
    s32 s;
    s32 c;
    s32 k;

    if (mode == 0) {
        init->unk0 = 0x40;
        init->unk8 = 0x18;
        init->unk14 = (s32)func_801E8108;
        init->unk4 = 6;
    } else if (mode == 1) {
        D_801E9FB0 = env;
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EA534 = entry;
        work->unk8 = ex->func_800E7CC4(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk14 = work->unkC.vx;
        work->unk18 = work->unkC.vy;
        work->unk1C = work->unkC.vz;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = work->unk8 + 0x46;
        }
        D_801EA530 = 0;
        work->unk34 = 0x3E8;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            switch (frame) {
            default: {
                SVECTOR rot;
                SVECTOR sxy;

                D_801EA530 = 0;
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                t = (frame << 12) / 50;
                s = ex->math_sin(t >> 2);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unkC, &rot, t / 3 + 0x400, 0, 1,
                                  s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot,
                                  ex->math_cos(t >> 2) * 5 / 2 + 0x800, 0, 3, s);
                env->unk28 = 0;
                if (frame == 0x27 || frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x40, 0x40, 0x40);
                }
                D_801EA538 = t >> 1;
                gte_SetRotMatrix(env->unk14);
                gte_SetTransMatrix(env->unk14);
                gte_ldv0(&work->unkC);
                gte_rtps();
                gte_stsxy(&sxy);
                if (sxy.vx < 0x78) {
                    sxy.vx = 0x78;
                }
                if (sxy.vx >= 0xC9) {
                    sxy.vx = 0xC8;
                }
                if (sxy.vy < 0x69) {
                    sxy.vy = 0x69;
                }
                if (sxy.vy >= 0x74) {
                    sxy.vy = 0x73;
                }
                func_801E7718(1, sxy.vx, sxy.vy, 0x10, 7, 0x11, (t * 0x64) >> 12, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))func_801E7FF4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x15), work->unk4);
                if (frame == 14) {
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                        p->unk18->unk24 = (s32)func_801E7ECC;
                    }
                }
                if (frame == 0x31) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0, 0x80);
                }
                if (frame >= 0x32) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0xFF, 0xFF);
                    ex->btlfx_initEffModels(D_801E9FC0, 6);
                    work->unk20 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0x139,
                                                             0xFFFF, 0, 0x80);
                    work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0x38,
                                                             0x3E83, 0, 0);
                    work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0x13C,
                                                             0xFFFF, 0x10, 0x80);
                    work->unk2C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF));
                }
                break;
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR sxy;
                SVECTOR rot;

                t = (frame << 12) / 10;
                if (frame < 6) {
                    c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c >> 2);
                }
                D_801EA538 = 0x800;
                gte_SetRotMatrix(env->unk14);
                gte_SetTransMatrix(env->unk14);
                gte_ldv0(&work->unkC);
                gte_rtps();
                gte_stsxy(&sxy);
                if (sxy.vx < 0x78) {
                    sxy.vx = 0x78;
                }
                if (sxy.vx >= 0xC9) {
                    sxy.vx = 0xC8;
                }
                if (sxy.vy < 0x69) {
                    sxy.vy = 0x69;
                }
                if (sxy.vy >= 0x74) {
                    sxy.vy = 0x73;
                }
                func_801E7718(1, sxy.vx, sxy.vy, 0x10, 7, 0x11, (t * 0x58) >> 12, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))func_801E7FF4);
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot, 0x1400 - t, 0, 1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, (0x1000 - t) * 6, 0, 1,
                                  s >> 1);
                rot.vx = 0x200;
                rot.vy = 0x200;
                rot.vz = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, t * 4, 0, 1, s);
                rot.vx = 0x200;
                rot.vy = 0x600;
                rot.vz = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, &rot, t * 4, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 10) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0x80, 0x80);
                    work->unk3C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    work->unk38 = (ex->btlfx_rand() % 3 - 1) * 0x9B;
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                t = (frame << 12) / 14;
                if (frame < 4) {
                    c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c >> 2, c >> 2);
                }
                ex->func_800E6624(work->unk34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->unk34 += 8;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                func_801E80C4(&work->unkC.vx, &rot2.vx);
                rot2.vz = 0;
                rot2.vx = work->unk38 + rot2.vx + 0x400;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->unk24);
                if (frame == 10) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x64, 0x40, 0);
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot, 0x1400 - t, 0, 1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unkC, &rot, t * 2 + 0x400, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 14) {
                    work->unk0 = 3;
                    entry->unk0 = -1;
                }
                break;
            }
            }
            break;
        case 3:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x80, 0x80, 0x80);
                }
                ex->func_800E6624(work->unk34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->unk34 -= 14;
                pos.vx = work->unkC.vx;
                pos.vy = work->unkC.vy;
                pos.vz = work->unkC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                func_801E80C4(&work->unkC.vx, &rot2.vx);
                rot2.vz = 0;
                rot2.vx = work->unk38 + rot2.vx + 0x400;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->unk24);
                rot3.vy = 0;
                rot3.vx = 0;
                rot3.vz = 0;
                rot3.pad = 0;
                s = ex->math_sin((frame << 9) >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &rot3, (frame << 10) | 0x200, 0,
                                  1, s);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unkC, &rot3, (frame << 11) + 0x2400,
                                  0, 1, (s >> 1) + 0x800);
                env->unk28 = 0;
                if (frame >= 8) {
                    work->unk0 = 4;
                    entry->unk0 = -1;
                    ex->func_800E3D54(work->unk3C);
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xAA, 0xAA, 0xAA);
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unkC);
                    if (p != NULL) {
                        p->unk22 = work->unk8 + 0x46;
                    }
                }
                break;
            }
            }
            break;
        case 4:
            switch (frame) {
            default: {
                SVECTOR rot;
                SVECTOR rot2;

                t = (frame << 12) / 50;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0xFF, 0xFF);
                } else if (frame < 7) {
                    c = 0xFF - (frame - 1) * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c);
                }
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x11), work->unk4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x15), work->unk4);
                if (frame < 0x20 && !(frame & 1)) {
                    k = frame * 0x180;
                    part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&rot);
                        if (frame != 0) {
                            ex->func_800CFCA8(&rot, ((frame << 7) >> 3) + 0x200, (s32)&rot2);
                        } else {
                            rot2.vz = 0;
                            rot2.vy = 0;
                            rot2.vx = 0;
                        }
                        part->unk4.vx = work->unkC.vx + rot2.vx;
                        part->unk4.vz = work->unkC.vz + rot2.vy;
                        part->unk2 = 0x1800 - k / 4;
                        part->unk4.vy = work->unkC.vy + rot2.vz;
                        ex->btlfx_randRotation(&part->unkC);
                    }
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_cos(t >> 2);
                env->unk28 = work->unk8 + 0x46;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &work->unkC, &rot,
                                  ex->math_cos(t >> 2) / 2 + 0x400, 0, 1, s >> 1);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x17), work->unk4);
                rot.vx = 0x200;
                rot.vy = 0x590;
                rot.vz = 0x80;
                rot.pad = 1;
                c = t * 3 + 0x800;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), &work->unkC, &rot, c, 0, 1, s);
                rot.vx = 0x600;
                rot.vy = 0x990;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), &work->unkC, &rot, c, 0, 1, s);
                env->unk28 = 0;
                if (frame >= 50) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

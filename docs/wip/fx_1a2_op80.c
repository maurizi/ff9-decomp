/* Draft of fx_1a2 fx_1a2_op80. Status: 7 aligned mismatches (0 outside case 3's frame-0 spawns); equivalent by
 * construction. Open: the original copies &w->unk8 into a second callee-saved register (s0) for the 7/0xF spawns
 * AFTER the getFileData(7) call, while calls 1, 2 and 5 use s1. Here sched1 hoists the copy above the call (into
 * its delay slot). `o = NULL` before the last spawn is a hack that ends CSE's o == &w->unk8 equivalence so the last
 * spawn keeps s1. Keys found: tbl = gfd(); tbl += 4 (two sets raise tbl's priority so &d spills, not &a, in case 0);
 * case 1 RGB inline; case 3/4 `* 32`, case 4 split k computation around the Abr call. */
#include "fxmod/fx_1a2.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1a2Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* unit position */
    SVECTOR unk10; /* body position */
    SVECTOR unk18; /* particle position */
    s16 unk20; /* bouncing rock position */
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s32 unk28; /* bouncing rock velocity */
    s32 unk2C; /* scale */
    s32 unk30[3]; /* models */
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} Fx1a2Work80; /* 0x54 */

extern BtlModel D_801EB2C4[];
extern CVECTOR D_801E97F0;
extern CVECTOR D_801E97F4;
extern CVECTOR D_801E97F8;
extern Fx1a2Seg D_801EC1B4[];
extern Fx1a2Seg D_801EC3B4[];
extern Fx1a2Seg D_801EC5B4[];
extern s32 D_801EBA94[];
extern s32 D_801EBF54[];

void func_801E7718(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx1a2Seg *buf, s32 abr);
void func_801E7B88(s32 model, SVECTOR *center);
void func_801E7D68(void);
s32 func_801E804C(s32 mode, Fx1a2Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx1a2Inst *inst);

s32 fx_1a2_op80(s32 mode, Fx1a2Work80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR a;
    VECTOR scale;
    VECTOR pos;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    SVECTOR e;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m;
    SVECTOR sv;
    Fx1a2Rock *o;
    Pool6CEntry *p;
    SVECTOR *tbl;
    s32 frame;
    s32 t;
    s32 s;
    s32 k;
    s32 i;
    s32 cc;

    if (mode == 0) {
        init->unk0 = 0x54;
        init->unk8 = 0x20;
        init->unk14 = (s32)func_801E804C;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        w->unk0 = 0;
        w->unk4 = 0;
        D_801E9844 = env;
        ex->btlfx_initEffModels(D_801EB2C4, 10);
        w->unk3C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 2), 0xB8, 0x3DC0, 0x80, 0);
        w->unk40 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xB8, 0x3DC0, 0x80, 0);
        w->unk30[0] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0xBA, 0x3E40, 0, 0xE0);
        w->unk30[1] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0xBA, 0x3E40, 0, 0xE0);
        w->unk30[2] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0xBA, 0x3E40, 0, 0xE0);
        w->unk44 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xB8, 0x3DC0, 0x80, 0);
        w->unk4C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0x98, 0x3E00, 0, 0x80);
        w->unk50 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x98, 0x3E00, 0, 0x80);
        rot.vx = rot.vz = 0x97;
        rot.vy = rot.pad = 0x3D40;
        a.vy = a.vx = a.vz = 0;
        a.pad = 0x80;
        w->unk48 = (s16)((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->unk8, 6), &rot, &a);
        ex->btlfx_getUnitPos(0, (s16 *)&w->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&w->unk10);
        ex->func_800E8980(0);
        w->unk2C = 0x8F8;
    } else {
    frame = entry->unk0;
    switch (w->unk0) {
    case 0:
        t = (frame << 12) / 93;
        ex->func_800DC818(0x10, 0x10, (u16 *)&a);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m1);
        ex->func_800CFE88(&m1, 0x10E, (s32)&b);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m2);
        ex->func_800CFE88(&m2, 0x147, (s32)&c);
        ex->func_800DC8AC(0x10, 0x10, (BtlBone *)&m3);
        ex->func_800CFE88(&m3, 0x155, (s32)&d);
        if (frame == 0) {
            func_801E7718(&a, &b, 0x407, &D_801E97F0, &D_801E97F4, 0, D_801EC1B4, 1);
            func_801E7718(&b, &c, 0x407, &D_801E97F4, &D_801E97F8, 0, D_801EC3B4, 1);
            func_801E7718(&c, &d, 0x407, &D_801E97F8, &D_801E97F0, 0, D_801EC5B4, 1);
        }
        func_801E7718(&a, &b, 0x1F, &D_801E97F0, &D_801E97F4, 0x1000, D_801EC1B4, 1);
        func_801E7718(&b, &c, 0x1F, &D_801E97F4, &D_801E97F8, 0x1000, D_801EC3B4, 1);
        func_801E7718(&c, &d, 0x1F, &D_801E97F8, &D_801E97F0, 0x1000, D_801EC5B4, 1);
        env->unk28 = 0x20;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &c, NULL, 0x400, 0, 1, ex->math_sin(t >> 2) >> 1);
        e.pad = 0;
        e.vy = 0;
        e.vx = 0;
        e.vz = frame << 4;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &c, &e, 0x800, 0, 1, ex->math_sin(t >> 2));
        env->unk28 = 0;
        ((s32 *)&w->unk18)[0] = ((s32 *)&c)[0];
        ((s32 *)&w->unk18)[1] = ((s32 *)&c)[1];
        if (frame == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&w->unk18);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
        }
        if (frame >= 0x5D) {
            w->unk0 = 1;
            entry->unk0 = -1;
        }
        break;
    case 1:
        if (frame < 0xB) {
            tbl = (SVECTOR *)ex->btlfx_getFileData(entry->unk8, 0xE);
            tbl = (SVECTOR *)((u8 *)tbl + 4);
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            if (frame == 4) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&w->unk8);
                if (p != NULL) {
                    p->unk22 = -0x40;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&w->unk8);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame < 7) {
                s = 0x1000;
            } else {
                s = 0x1000 - ((frame - 7) << 10);
            }
            scale.vx = scale.vz = (w->unk2C * 13) >> 4;
            scale.vy = w->unk2C * 3;
            for (i = 0; i < 6; i++) {
                if (frame - i >= 0) {
                    rot.vy = ((s16 *)tbl)[(frame - i) * 3] + 0x800;
                    if (frame < 3) {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk30[frame]);
                    } else {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk30[2]);
                        ex->Hi_ModifyEffModelRGB(w->unk30[2], ((s - i * s / 5) >> 5) - 0x80, ((s - i * s / 5) >> 5) - 0x80,
                                                 ((s - i * s / 5) >> 5) - 0x80);
                    }
                }
            }
            gte_SetRotMatrix(env->unk14);
            gte_SetTransMatrix(env->unk14);
            rot.vy = ((s16 *)tbl)[frame * 3] + 0x800;
            ex->func_800316D8(&rot, &m);
            ex->ScaleMatrix(&m, &scale);
            m.t[0] = pos.vx;
            m.t[1] = pos.vy;
            m.t[2] = pos.vz;
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            sv.vx = 0x3E8;
            sv.vy = 0;
            sv.vz = 0;
            gte_ldv0(&sv);
            gte_rtv0tr();
            gte_stlvnl(&pos);
            sv.vx = pos.vx;
            sv.vy = pos.vy;
            sv.vz = pos.vz;
            ((s32 *)&w->unk18)[0] = ((s32 *)&sv)[0];
            ((s32 *)&w->unk18)[1] = ((s32 *)&sv)[1];
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&w->unk18);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            ex->func_800E6624(0, 0x280, 0x1E0, 0x40, 0x20, 0x2C0, ((frame & 7) << 5) + 0x100);
            if (frame < 0xB) {
                break;
            }
        }
        w->unk0 = 2;
        entry->unk0 = -1;
        break;
    case 2:
        if (frame >= 0xC) {
            w->unk0 = 3;
            entry->unk0 = -1;
        }
        break;
    case 3:
        t = (frame << 12) / 29;
        if (frame == 0) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EBA94;
                p->unk12 = 0x10;
                p->unkC = 0x4C;
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&w->unk8);
            if (p != NULL) {
                p->unk14 = (s32)D_801EBF54;
                p->unk12 = 8;
                p->unkC = 0x4C;
                p->unk22 = 0x40;
            }
            o = (Fx1a2Rock *)&w->unk8;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)o);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)o);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
            o = NULL;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&w->unk8);
            if (p != NULL) {
                p->unk22 = 0x40;
            }
        }
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        rot.vx = 0;
        rot.vy = frame << 5;
        rot.vz = 0;
        scale.vx = scale.vz = (w->unk2C * 14) >> 4;
        scale.vy = w->unk2C + ex->math_cos(t >> 2) * 2;
        ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, w->unk40, w->unk3C, t);
        ex->Hi_ModifyEffModelRGB(w->unk40, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        scale.vx = scale.vz = (w->unk2C * 14) >> 4;
        if (frame < 4) {
            scale.vy = (w->unk2C * frame / 4) * 2;
        } else {
            scale.vy = ((w->unk2C * ex->math_cos(t >> 2)) >> 12) * 2;
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk44);
        ex->Hi_ModifyEffModelRGB(w->unk44, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                 (ex->math_cos(t >> 2) >> 5) - 0x80);
        ex->func_800E6624(frame * 8, 0x240, 0x100, 0x30, 0x80, 0x240, 0x180);
        if (frame >= 10) {
            if (frame == 10) {
                ((SVECTOR *)&m)->vx = 0;
                ((SVECTOR *)&m)->vy = 0;
                ((SVECTOR *)&m)->vz = 0;
                func_801E7B88(w->unk4C, (SVECTOR *)&m);
                w->unk22 = w->unk8.vy;
                w->unk28 = -0x28;
                w->unk20 = w->unk8.vx;
                w->unk24 = w->unk8.vz;
                w->unk22 -= 0x1AE;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x10, 0x3C, 0x46);
                ex->func_800E806C(0, 0);
                ex->func_800E806C(0x11, -2);
                ex->btlfx_setUnitPos(0x11, (s16 *)&w->unk8);
            }
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            ex->btlfx_getUnitRot(0, (s16 *)&rot);
            scale.vx = scale.vy = scale.vz = 0x3800;
            rot.vy += 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk4C);
            func_801E7D68();
            rot.vx = (w->unk28 + 0x28) * 32;
            pos.vx = w->unk20;
            pos.vy = w->unk22;
            pos.vz = w->unk24;
            w->unk22 += w->unk28;
            w->unk20 += 7;
            w->unk24 += 0xC;
            w->unk28 += 4;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk50);
        }
        if ((u32)(frame - 10) < 4) {
            for (i = 0; i < 6; i++) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = w->unk8.vx;
                    o->unkC.vy = w->unk8.vy;
                    o->unkC.vz = w->unk8.vz;
                    o->unkC.vx += ex->btlfx_randCentered(0x100);
                    o->unkC.vy += ex->btlfx_randCentered(0x180) - 0x100;
                    o->unkC.vz += ex->btlfx_randCentered(0x100);
                    o->unk2 = w->unk8.vy - w->unk10.vy;
                    o->unkC.vy = w->unk8.vy;
                    o->unk14.vz = 0;
                    o->unk14.vx = 0;
                    o->unk14.vy = ex->btlfx_randCentered(0x400);
                    o->unk1C = ((ex->btlfx_rand() & 3) << 8) + 0xAAA;
                    o->unk8 = 0;
                }
            }
            if (frame == 10) {
                o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (o != NULL) {
                    o->unkC.vx = w->unk8.vx;
                    o->unkC.vy = w->unk8.vy;
                    o->unkC.vz = w->unk8.vz - 0x20;
                    o->unk2 = 0x1A4;
                    o->unk1C = 0x1800;
                    o->unk8 = 1;
                    o->unk14.vz = 0;
                    o->unk14.vx = 0;
                    o->unk14.vy = 0;
                    o->unk6 = 0x40;
                }
            }
        }
        if (frame >= 0x1D) {
            w->unk0 = 4;
            entry->unk0 = -1;
        }
        break;
    case 4:
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        ex->btlfx_getUnitRot(0, (s16 *)&rot);
        scale.vx = scale.vy = scale.vz = 0x3800;
        rot.vy += 0x800;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk4C);
        func_801E7D68();
        rot.vx = (w->unk28 + 0x28) * 32;
        pos.vx = w->unk20;
        pos.vy = w->unk22;
        pos.vz = w->unk24;
        w->unk22 += w->unk28;
        w->unk20 += 7;
        w->unk24 += 0xC;
        w->unk28 += 4;
        if (w->unk28 > 0 && w->unk22 > 0) {
            w->unk28 = -w->unk28 / 2;
        }
        if (frame < 0x30) {
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk50);
            if ((u32)(frame - 0x20) < 0x10) {
                k = 0x1000 - ((frame - 0x20) << 8);
                ex->Hi_ModifyEffModelAbr(w->unk50, 1);
                ex->Hi_ModifyEffModelRGB(w->unk50, (k >> 5) - 0x80, (k >> 5) - 0x80, (k >> 5) - 0x80);
            }
        }
        if (frame == 0x4B) {
            ex->func_800E83EC();
        }
        if (frame >= 0x64) {
            ex->btlfx_poolClear((Unk800DA1B8 *)entry->unk10);
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    w->unk4++;
    }
    return 0;
}

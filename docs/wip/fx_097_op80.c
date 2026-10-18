/* Draft of fx_097 fx_097_op80. Status: 97 aligned mismatches, logic complete (fx_098 op80 is the same function with
 * other constants). Open: the original keeps 0x9A in s3 and 0x80 in s0 across the mode-1 registrations, ties sp58
 * (fade) and the pool-spawn results differently, and multiplies by 0xC8 held in s0; the rest is register allocation.
 * Levers used: constants through variables so they aren't folded (c = 0x5C, c = work->unk80 + 11), goto loop for the
 * item spawn loop (the original's loop isn't strength-reduced). */
#include "btlfx_mod.h"

extern s16 D_801E9D50[];
extern Unk800F78A4 *D_801E9DB4;

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx097Vec3;

/* one shard, a pool item */
typedef struct {
    s16 unk0; /* state */
    s16 unk2; /* flying */
    s32 unk4; /* index */
    s32 unk8; /* frame */
    Fx097Vec3 unkC; /* position << 4 */
    SVECTOR unk18; /* velocity */
    SVECTOR unk20; /* rotation */
    s16 unk28; /* rotation speed */
    s16 unk2A;
    s32 unk2C; /* models */
    s32 unk30;
    Pool6CEntry *unk34;
} Fx097Item; /* 0x38 */

typedef struct {
    u8 pad0[0x20];
    s32 unk20[22]; /* models */
    s16 unk78;
} Fx097Work;

extern SVECTOR D_801E9CE8[];
s32 func_801E82F4(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work);
typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* models */
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20[14];
    SVECTOR unk58; /* summon position */
    SVECTOR unk60;
    SVECTOR unk68;
    SVECTOR unk70;
    s16 unk78;
    s16 unk7A; /* shards spawned */
    s32 unk7C;
    s32 unk80;
    Pool6CEntry *unk84;
} Fx097Work80; /* 0x88 */

extern BtlModel D_801E9DC4[];
extern s32 D_801E9D18[14];
void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 p, s32 u, s32 tpage, s32 a6, s32 a7, s32 a8, s32 abr);
s32 fx_097_op80(s32 mode, Fx097Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init);

s32 fx_097_op80(s32 mode, Fx097Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    s32 k;
    Fx097Item *it;
    Pool6CEntry *p;
    SVECTOR *a;
    s32 frame;
    s32 i;
    s32 t;
    s32 u;
    s32 c;
    s32 d;

    if (mode == 0) {
        init->unk0 = sizeof(Fx097Work80);
        init->unk8 = sizeof(Fx097Item);
        init->unk14 = (s32)func_801E82F4;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        i = 0;
        D_801E9DB4 = env;
        ex->btlfx_initEffModels(D_801E9DC4, 0x19);
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), 0x9A, 0x3DC0, 0x80, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0x9A, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13), 0x9A, 0x3DC0, 0x80, 0);
        work->unk14 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0xBA, 0x3D40, 0, 0x80);
        work->unk18 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0xBA, 0x3D40, 0, 0x80);
        work->unk1C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0xBA, 0x3D40, 0, 0x80);
        for (; i < 14; i++) {
            work->unk20[i] =
                (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801E9D18[i]), 0x9A, 0x3DC0, 0x80, 0);
        }
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk7A = 0;
        work->unk78 = 0;
        work->unk7C = 0;
        work->unk80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk58);
        BTLFX_COPY_SVEC(&work->unk60, &work->unk58);
        BTLFX_COPY_SVEC(&work->unk70, &work->unk58);
        work->unk60.vy -= 0x22A;
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->unk0;
        pos.vx = work->unk58.vx;
        pos.vy = work->unk58.vy;
        pos.vz = work->unk58.vz;
        scale.vy = scale.vx = 0x1DFE;
        scale.vz = 0x1354;
        k = 0;
        switch (work->unk0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            func_801E7CBC(&work->unk58, &rot, 0x26A8, ((frame << 12) / 6) >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            u = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            func_801E7CBC(&work->unk58, &rot, 0x26A8, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(u) * 0x2BC) >> 12), 1);
            k = t;
            ex->func_800E6624(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            rot.pad = 1;
            d = ex->btlfx_getFileData(entry->unk8, 0x24);
            ex->func_800D23D8((u8 *)d, &work->unk58, &rot, ((((ex->math_sin(u) * 0x19FE) >> 12) + 0x1154) * 3) >> 2, 0, 1,
                              ex->math_cos(u));
            if (frame >= 10) {
                work->unk0 = 2;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk68, &work->unk58);
                work->unk68.vy -= 0x5C6;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unk68);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x29), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            t = 0x2000 - (frame << 10);
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_ModifyEffModelRGB(work->unk8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (frame < 6) {
                c = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 2, c * 2 / 3, c);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            k = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk2 = 0;
                    it->unk4 = work->unk7A;
                    a = &D_801E9CE8[work->unk7A];
                    if (work->unk7A >= 3) {
                        it->unk2C = work->unkC;
                        it->unk30 = work->unk18;
                    } else {
                        it->unk2C = work->unk10;
                        it->unk30 = work->unk1C;
                    }
                    it->unkC.vx = work->unk58.vx;
                    it->unkC.vy = work->unk58.vy;
                    it->unkC.vz = work->unk58.vz;
                    t = ex->math_sin(a->vy + 0x800);
                    c = 0xC8;
                    it->unkC.vx += (t * c) >> 12;
                    it->unkC.vz += (ex->math_cos(a->vy + 0x800) * c) >> 12;
                    it->unk18.vx = it->unkC.vx;
                    it->unk18.vy = it->unkC.vy;
                    it->unk18.vz = it->unkC.vz;
                    work->unk7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (work->unk7A >= 6) {
                work->unk0 = 4;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk58);
                work->unk84 = p;
                if (p != NULL) {
                    p->unk22 = 0x5C;
                }
            }
            break;
        case 4:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (frame >= 8) {
                work->unk0 = 5;
                entry->unk0 = -1;
            }
            break;
        case 5:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            rot.pad = 1;
            if (frame >= 10) {
                work->unk0 = 6;
                work->unk78 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
                ex->func_800E3D54(work->unk84);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk60);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                i = 0;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
            loop:
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk2 = 1;
                    it->unk0 = i;
                    it->unk4 = i;
                    it->unk8 = 0;
                    it->unk2C = work->unk20[i];
                    func_801E8284((s16 *)&work->unk58, (s16 *)&rot, i, (s16 *)&v);
                    BTLFX_COPY_SVEC(&it->unk20, &rot);
                    it->unkC.vx = v.vx << 4;
                    it->unkC.vy = v.vy << 4;
                    it->unkC.vz = v.vz << 4;
                    it->unk28 = ex->btlfx_randCentered(0x5C);
                    it->unk2A = ex->btlfx_randCentered(0x80);
                    c = 0x5C;
                    ex->func_800CFCA8(&rot, -(((ex->btlfx_rand() & 0xF) + c - (i << 3)) << 4), (s32)&it->unk18);
                    it->unk18.vx += ex->btlfx_randCentered(0x3E0);
                    it->unk18.vz += ex->btlfx_randCentered(0x3E0);
                }
                i++;
                if (i < 6) {
                    goto loop;
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                k = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 5) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame < 8) {
                c = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 2, c * 2 / 3, c);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (work->unk0 - 2 < 4) {
            if (++work->unk7C < 11) {
                c = work->unk80 + 11;
                work->unk80 = c - (work->unk7C * 10) / 11;
            } else {
                work->unk80++;
            }
            ex->func_800E6624(work->unk80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (k != 0) {
            v.vx = 0x400;
            v.vy = 0;
            v.pad = 1;
            v.vz = work->unk4 * 0x29;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk58, &v, (work->unk4 << 6) + 0x15A9, 0,
                              1, k);
            v.vz = work->unk4 * 0x18 + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk58, &v, (work->unk4 << 6) + 0x15A9, 0,
                              1, k);
            env->unk28 = 0x127;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk58, NULL, (work->unk4 << 5) + 0x15A9, 0,
                              1, k);
            env->unk28 = 0;
        }
        work->unk4++;
    }
    return 0;
}

/* Draft of fx_085 fx_085_op80. Status: full draft (sibling of fx_0d9 op80 with an extra state 0); register allocation
 * plus the (frame<<12)/24>>2 shift that the original does with srav by the file-id register and the mode-1 store
 * order of D_801E9DF4. */
#include "btlfx_mod.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx085Strip; /* 0x7C */
extern u16 D_801E9D94[];
void func_801E82A0(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9D9C[];
void func_801E8508(Unk80100C78Entry *entry, Fx085Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E878C(Unk80100C78Entry *entry, Fx085Strip *strip, Fx085Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9DF4;
extern SVECTOR D_801E9FA0[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_085_STRIPS ((Fx085Strip *)((u8 *)D_801E9FA0 - 4))
s32 func_801E8C28(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);
typedef struct {
    SVECTOR unk0; /* target position */
    SVECTOR unk8; /* ground position */
    s32 unk10; /* state */
    s32 unk14; /* model */
    s32 unk18; /* model */
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
} Fx085Work80; /* 0x24 */

extern BtlModel D_801E9DFC[];
extern s32 D_801E9F8C;
s32 fx_085_op80(s32 mode, Fx085Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_085_op80(s32 mode, Fx085Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR rot2;
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR p;
    Pool6CEntry *e;
    s32 frame;
    s32 t;
    s32 t_2;
    s32 t_1;
    s32 c;
    s32 c_5;
    s32 s;
    s32 i;
    s32 j;
    s32 a;
    s32 u;

    if (mode == 0) {
        *arg4 = sizeof(Fx085Work80);
    } else if (mode == 1) {
        work->unk0.vx = env->unkC->unk38;
        D_801E9DF4 = env;
        work->unk0.vy = env->unkC->unk3A;
        work->unk0.vy -= 0x200;
        work->unk0.vz = env->unkC->unk3C;
        BTLFX_COPY_SVEC(&work->unk8, &work->unk0);
        work->unk8.vy = 0;
        ex->btlfx_initEffModels(D_801E9DFC, 2);
        work->unk14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 6), 0xB8, 0x3E40, 0,
                                                 0x80);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x119, 0xFFFF,
                                                 0, 0x80);
        work->unk10 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk10) {
        case 0:
            if (frame == 0) {
                work->unk1C = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1B), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x46;
                }
            }
            if (frame == 5) {
                work->unk20 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x46;
                }
            }
            if (frame == 0x1C) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x40, 0);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame >= 0x1D) {
                work->unk10 = 1;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk1C);
                ex->func_800E3D54(work->unk20);
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x20;
                }
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x40, 0xFF);
            }
            if (frame == 1) {
                env->unk28 = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk8, NULL, 0x3000, 0, 1,
                                        0x1000, 0, 0);
                env->unk28 = 0;
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &work->unk8, NULL, 0x800, 0, 1, -1);
            env->unk28 = 0;
            if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0xFF);
            }
            if (frame >= 2) {
                work->unk10 = 2;
                entry->unk0 = -1;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
                work->unk1C = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            break;
        case 2:
            u = (frame << 12) / 50;
            if (frame < 6) {
                c = ex->math_cos((frame << 10) / 6) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 14, c >> 13, c >> 12);
            }
            if (frame < 20) {
                c = ex->math_cos((frame << 10) / 20);
                env->unk28 = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL, 0x3000, 0, 1,
                                        c, 0, 0);
                if (frame < 10) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk8, NULL, 0x3000, 0,
                                            1, 0x1000 - (frame << 12) / 10, 0, 0);
                }
                env->unk28 = 0;
            }
            if (frame < 24) {
                env->unk28 = 0x200;
                t_1 = (frame << 12) / 96;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &work->unk8, NULL,
                                  ex->math_cos(t_1) * 2 + 0x1000, 0, 1, ex->math_cos(t_1));
                env->unk28 = 0;
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            if (work->unk1C != NULL) {
                if (work->unk1C->unk30 == -1) {
                    work->unk1C = NULL;
                }
                if (work->unk1C != NULL) {
                    work->unk1C->unk5C.vx = rot.vx;
                    work->unk1C->unk5C.vy = rot.vy;
                    work->unk1C->unk5C.vz = rot.vz;
                }
            }
            if (frame < 16) {
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                t_2 = frame << 8;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x26), &work->unk8, &rot,
                                  ex->math_sin(t_2 >> 2) * 7, 0, 1, 0x1000 - t_2);
            }
            rot2.vx = 0x400;
            rot2.vz = 0;
            rot2.vy = 0;
            rot2.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x25), &work->unk8, &rot2,
                              ex->math_sin(u >> 2) * 7, 0, 1, ex->math_cos(u >> 2));
            if (frame == 1) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk8);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            if (frame < 24) {
                pos.vx = work->unk8.vx;
                a = frame * 0x18;
                pos.vy = work->unk8.vy;
                ang.vx = 0;
                ang.vy = a;
                ang.vz = 0;
                pos.vz = work->unk8.vz;
                t = (frame << 12) / 24;
                t >>= 2;
                s = ((ex->math_sin(t) * 3) >> 2) + 0x100;
                scale.vx = scale.vy = scale.vz = s;
                D_801E9F8C = (s * 0x708) >> 12;
                ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, work->unk14);
                ex->Hi_ModifyEffModelAbr(work->unk14, 1);
                ex->Hi_ModifyEffModelRGB(work->unk14, (ex->math_cos(t) * 2 / 3 >> 5) - 0x80, (ex->math_cos(t) * 2 / 3 >> 5) - 0x80,
                                         (ex->math_cos(t) * 2 / 3 >> 5) - 0x80);
                env->unk28 = 0x20;
                c_5 = ex->math_cos(t);
                for (i = 0, j = frame; i < 16; i++, j += 3) {
                    BTLFX_COPY_SVEC(&p, &work->unk8);
                    p.vx += (ex->math_cos(a) * D_801E9F8C) >> 12;
                    p.vz += (ex->math_sin(a) * D_801E9F8C) >> 12;
                    a += 0x100;
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &p, NULL,
                                            0x3000 - ((i & 1) << 12), j & 7, 1, c_5, ex->btlfx_randCentered(5),
                                            ex->btlfx_randCentered(5));
                }
                env->unk28 = 0;
            }
            if (frame >= 50) {
                work->unk10 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

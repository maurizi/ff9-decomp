/* Draft of fx_0d9 fx_0d9_op80. Status: full draft; same instructions as the original apart from register allocation
 * (122 aligned diff lines with registers, 26 without). Per-block temporaries (t_1, t_2, c_5) improved the allocation. */
#include "btlfx_mod.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* func_800E7D3C(0x10, 1) */
} Fx0d9Work84;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[15];
} Fx0d9Strip; /* 0x7C */
extern u16 D_801E9E40[];
void func_801E8188(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind);
extern u16 D_801E9E48[];
void func_801E83F0(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind);
void func_801E8674(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind);
extern Unk800F78A4 *D_801E9EA0;
extern SVECTOR D_801EA04C[]; /* strips[0].unk4: the strips start 4 bytes earlier */
#define FX_0D9_STRIPS ((Fx0d9Strip *)((u8 *)D_801EA04C - 4))
s32 func_801E8B10(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3);
typedef struct {
    SVECTOR unk0; /* caster body position */
    SVECTOR unk8; /* caster position */
    s32 unk10; /* state */
    s32 unk14; /* model */
    s32 unk18; /* model */
    Pool6CEntry *unk1C;
    s32 pad20;
} Fx0d9Work80; /* 0x24 */

extern BtlModel D_801E9EA8[];
extern s32 D_801EA038;
s32 fx_0d9_op80(s32 mode, Fx0d9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4);

s32 fx_0d9_op80(s32 mode, Fx0d9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
        *arg4 = sizeof(Fx0d9Work80);
    } else if (mode == 1) {
        D_801E9EA0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_initEffModels(D_801E9EA8, 2);
        work->unk14 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0xB8, 0x3E40, 0,
                                                 0x80);
        work->unk18 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x119, 0xFFFF,
                                                 0, 0x80);
        work->unk10 = 1;
        e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk8);
        if (e != NULL) {
            e->unk22 = 0x20;
        }
    } else {
        frame = entry->unk0;
        switch (work->unk10) {
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x40, 0xFF);
            }
            if (frame == 1) {
                env->unk28 = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &work->unk8, NULL, 0x2800, 0, 1,
                                        0x1000, 0, 0);
                env->unk28 = 0;
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL, 0x800, 0, 1, -1);
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
                work->unk1C = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unk8);
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
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk8, NULL, 0x2800, 0, 1,
                                        c, 0, 0);
                if (frame < 10) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), &work->unk8, NULL, 0x2800, 0,
                                            1, 0x1000 - (frame << 12) / 10, 0, 0);
                }
                env->unk28 = 0;
            }
            if (frame < 24) {
                env->unk28 = 0x200;
                t_1 = (frame << 12) / 24;
                t_1 >>= 2;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL,
                                  ex->math_cos(t_1) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(t_1));
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
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk8, &rot,
                                  ex->math_sin(t_2 >> 2) * 4, 0, 1, 0x1000 - t_2);
            }
            rot2.vx = 0x400;
            rot2.vz = 0;
            rot2.vy = 0;
            rot2.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, &rot2,
                              ex->math_sin(u >> 2) * 4, 0, 1, ex->math_cos(u >> 2));
            if (frame == 1) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unk8);
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
                s = ((ex->math_sin(t) * 0x5DC) >> 12) + 0x140;
                scale.vx = scale.vy = scale.vz = s;
                D_801EA038 = (s * 0x708) >> 12;
                ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, work->unk14);
                ex->Hi_ModifyEffModelAbr(work->unk14, 1);
                ex->Hi_ModifyEffModelRGB(work->unk14, (ex->math_cos(t) * 2 / 3 >> 5) - 0x80, (ex->math_cos(t) * 2 / 3 >> 5) - 0x80,
                                         (ex->math_cos(t) * 2 / 3 >> 5) - 0x80);
                env->unk28 = 0x20;
                c_5 = ex->math_cos(t);
                for (i = 0, j = frame; i < 16; i++, j += 3) {
                    BTLFX_COPY_SVEC(&p, &work->unk8);
                    p.vx += (ex->math_cos(a) * D_801EA038) >> 12;
                    p.vz += (ex->math_sin(a) * D_801EA038) >> 12;
                    a += 0x100;
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &p, NULL,
                                            0x1800 - ((i & 1) << 12), j & 7, 1, c_5, ex->btlfx_randCentered(5),
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

/* Draft of fx_1fe fx_1fe_op81. Status: 44 lines (build/w/nd.py); variable renaming search (build/w/rn.py) took it from 207. */
#include "fxmod/fx_1fe.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8;
    SVECTOR unk10;
    SVECTOR unk18;
    SVECTOR unk20; /* camera target */
    u8 pad28[0x34 - 0x28];
    s32 unk34; /* model */
    s32 unk38; /* model */
    s32 unk3C; /* model */
} Fx1feWork81; /* 0x40 */

extern BtlModel D_801E9070[];
extern SVECTOR D_801E88F8;
extern SVECTOR D_801E8900;
extern CVECTOR D_801E8908;
extern u8 D_801E8960[];

s32 fx_1fe_op81(s32 mode, Fx1feWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    s32 frame;
    s32 t;
    s32 r;
    s32 c;
    s32 d;
    s32 k;
    s32 f1;
    s32 f2;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk0 = 1;
        work->unk4 = 0;
        D_801E9060 = env;
        ex->btlfx_initEffModels(D_801E9070, 7);
        work->unk34 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk38 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 1);
        work->unk3C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xF), 1);
        work->unk8.vx = D_801E88F8.vx;
        work->unk8.vy = D_801E88F8.vy;
        work->unk8.vz = D_801E88F8.vz;
        work->unk20.vx = D_801E88F8.vx;
        work->unk20.vy = D_801E88F8.vy;
        work->unk20.vz = D_801E88F8.vz;
        work->unk20.vy -= 0xA00;
        env->unk2C->unk0[3].vx = work->unk20.vx;
        env->unk2C->unk0[3].vy = work->unk20.vy;
        env->unk2C->unk0[3].vz = work->unk20.vz;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 1:
            d = (frame << 12) / 37;
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->unk10, &D_801E88F8);
                work->unk10.vy -= 0x1000;
                work->unk10.vz -= 0x200;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = D_801E8960;
                    p->unk12 = 0x40;
                    p->unkC = 0x1C;
                    p->unk22 = 0x80;
                }
            }
            work->unk20.vx = D_801E88F8.vx;
            work->unk20.vy = D_801E88F8.vy;
            work->unk20.vz = D_801E88F8.vz;
            work->unk20.vy = (s16)(work->unk20.vy - 0x200) - (ex->math_sin(d >> 2) >> 1);
            env->unk2C->unk0[3].vx = work->unk20.vx;
            env->unk2C->unk0[3].vy = work->unk20.vy;
            env->unk2C->unk0[3].vz = work->unk20.vz;
            if (frame >= 0x25) {
                work->unk0 = 2;
                entry->unk0 = -1;
                work->unk20.vx = D_801E88F8.vx;
                work->unk20.vy = D_801E88F8.vy;
                work->unk20.vz = D_801E88F8.vz;
                work->unk20.vy -= 0x200;
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            break;
        case 2:
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&D_801E88F8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                work->unk10.vx = D_801E88F8.vx;
                work->unk10.vy = D_801E88F8.vy;
                work->unk10.vz = D_801E88F8.vz;
                work->unk10.vy -= 0x200;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 7) {
                ex->func_800E3C58();
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&D_801E88F8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame >= 7) {
                rot.vy = frame * 4;
                rot.vx = 0;
                rot.vz = 0;
                scale.vy = 0x555;
                pos.vx = D_801E88F8.vx;
                pos.vy = D_801E88F8.vy;
                pos.vz = D_801E88F8.vz;
                f1 = ((frame - 7) << 12) / 25;
                scale.vz = scale.vx = (f1 >> 4) + scale.vy;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk34);
                do {
                    r = frame * 7;
                } while (0);
                rot.vy = r;
                scale.vz = scale.vx = (f1 >> 4) + 0x400;
                scale.vy = 0x800;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk34);
                ex->Hi_ModifyEffModelRGB(work->unk34, -0x20, -0x20, -0x20);
                rot.vy = r;
                scale.vz = scale.vx = 0x400;
                scale.vy = ((ex->math_sin(f1 >> 2) / 2) + 0x800) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk38);
                ex->func_800E6624(1000 - frame * 9, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
                ex->func_800E6624(1000 - r, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
                ex->math_cos(f1);
                ex->math_cos(f1 * 2);
                ex->func_800D0B18((s32)(env->unk20 + 2), 0x42, 0x10, 3, 0x3C, 1, &D_801E8908, 0x18, frame * 400, 0x186, 0x100);
            }
            do {
                if ((u32)(frame - 7) < 0x14) {
                    s32 u;
    
                    rot.vx = 0x400;
                    pos.vx = D_801E88F8.vx;
                    pos.vy = D_801E88F8.vy;
                    pos.vz = D_801E88F8.vz;
                    u = ((frame - 7) << 12) / 20;
                    do {
                        r = u >> 2;
                    } while (0);
                    c = 0xC00;
                    c -= u;
                    rot.vy = ex->math_sin(r) / 3;
                    rot.vz = 0;
                    scale.vy = scale.vx = (ex->math_sin(r) >> 6) + 0x249;
                    scale.vz = 0x555;
                    d = (ex->math_sin(r) / 15) + 2;
                    c = (c >> 5) - 0x80;
                    for (k = 0; k < 7; k++) {
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk3C);
                        ex->Hi_ModifyEffModelRGB(work->unk3C, c, c, c);
                        pos.vy -= d;
                        rot.vy = ((rot.vy * 5) >> 2) + (k << 10);
                        scale.vy = scale.vx = (scale.vx * 9) >> 3;
                    }
                }
            } while (0);
            if (frame == 5) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x64);
            }
            if (frame == 6) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0x80, 0xFF);
            }
            if ((u32)(frame - 7) < 8) {
                s32 v = 0xF0 - (((frame - 7) * 0xF0) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, v, v, v >> 1);
            }
            if (frame >= 0x18) {
                s32 v = ((frame - 0x18) * 0xFF) >> 3;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, v, v, v);
            }
            if (frame >= 0x20) {
                work->unk0 = 6;
                entry->unk0 = -1;
                work->unk20.vx = D_801E8900.vx;
                work->unk20.vy = D_801E8900.vy;
                work->unk20.vz = D_801E8900.vz;
                work->unk20.vy -= 0x400;
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            break;
        case 6:
            t = (frame << 12) / 87;
            if (frame < 0x18) {
                s32 v = 0xFF - (frame * 0xFF) / 24;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, v, v, v);
            }
            work->unk20.vx = D_801E8900.vx;
            work->unk20.vy = D_801E8900.vy;
            work->unk20.vz = D_801E8900.vz;
            work->unk20.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
            env->unk2C->unk0[3].vx = work->unk20.vx;
            env->unk2C->unk0[3].vy = work->unk20.vy;
            env->unk2C->unk0[3].vz = work->unk20.vz;
            if (frame == 0) {
                work->unk10.vx = D_801E8900.vx;
                work->unk10.vy = D_801E8900.vy;
                work->unk10.vz = D_801E8900.vz - 0x100;
                work->unk10.vy -= 0x400;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x10;
                }
            }
            if (frame == 0x20) {
                work->unk18.vx = D_801E8900.vx;
                do {
                    work->unk18.vy = D_801E8900.vy;
                } while (0);
                work->unk10.vz -= 0x100;
                work->unk18.vz = D_801E8900.vz;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk22 = 0x10;
                }
            }
            if (frame >= 0x57) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

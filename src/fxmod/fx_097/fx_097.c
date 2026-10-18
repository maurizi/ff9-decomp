#include "fxmod/fx_097.h"

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E7720);

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E7CBC);

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9DB4)->func_800CFD78(a, b, -((D_801E9D50[i] * 0x455) >> 9), out);
}

s32 func_801E82F4(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx097Item *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else if (it->unk2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &D_801E9CE8[it->unk4];
        switch (it->unk0) {
        case 0:
            if (it->unk8 == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&it->unk18);
                it->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 > 0) {
                it->unk0 = 1;
                it->unk8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->unk8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 >= 4) {
                it->unk0 = 2;
                it->unk8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->unkC.vx;
                v38.vy = it->unkC.vy;
                v38.vz = it->unkC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->func_800CFD78((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->unk8, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->func_800D23D8((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->unk78 != 0) {
                it->unk8 = 0;
                if (it->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->unk18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (q != NULL) {
                        q->unk2 = 1;
                        q->unk0 = n;
                        q->unk4 = k;
                        q->unk8 = 0;
                        q->unk2C = work->unk20[n];
                        func_801E8284((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->unk20, p);
                        q->unkC.vx = v48.vx << 4;
                        q->unkC.vy = v48.vy << 4;
                        q->unkC.vz = v48.vz << 4;
                        q->unk28 = ex->btlfx_randCentered(0x84);
                        q->unk2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->func_800CFCA8(&v58, t, (s32)&q->unk18);
                        q->unk18.vx += ex->btlfx_randCentered(0x200);
                        q->unk18.vy += ex->btlfx_randCentered(0x200);
                        q->unk18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->unk2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->unkC.vx += it->unk18.vx;
        it->unkC.vy += it->unk18.vy;
        it->unkC.vz += it->unk18.vz;
        it->unk18.vy += 0x40;
        it->unk20.vx += it->unk28;
        it->unk20.vy += it->unk2A;
        c = (c * 3) >> 1;
        if (it->unk0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->unkC.vx >> 4;
        pos.vy = it->unkC.vy >> 4;
        pos.vz = it->unkC.vz >> 4;
        ex->Hi_DrawEffModel(&it->unk20, &pos, (s32)&scale, it->unk2C);
        ex->Hi_ModifyEffModelAbr(it->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(it->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->unk8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_097/nonmatchings/fx_097", fx_097_headerPtr);

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", fx_097_op80);

/* WIP func_800E57B0: 34 differing rows (build/cmp.py-style aligned diff), 4 insns long. New in pass 9 (effect step: sprite sheet on a unit, sibling of func_800E4C88). Control flow, switch layout and all calls match. Left: in the original the constant 1 compared against arg0 stays in s0 through the init path (the table index shift is sllv by s0), so a/b go to s1/s3 and arg2 to s4; here s0 is reused for a and arg2 lands in s3, which also shifts the unk1C copy scheduling. << arg0 or a k = 1 variable both fold back to a constant. */
#include "ovl_0b.h"

typedef struct {
    u8 unk0; /* kind */
    u8 unk1; /* unit id, 0xFF = none */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE; /* frame count */
    void *unk10;
    s32 unk14; /* frame */
    s16 unk18;
    s16 unk1A;
    SVECTOR unk1C;
    s16 unk24[3];
} Unk800E57B0; /* 0x2C */

extern u16 D_800F8D04[];
s32 func_800E7C54(s32, s32);
s32 func_800E7CC4(s32, s32);
void func_800E7D3C(s32, s32, s16 *);

s32 func_800E57B0(s32 arg0, Unk800E57B0 *arg1, Unk800F78A4 *arg2, s32 *arg3, s32 *arg4) {
    SVECTOR rot;
    s16 v[4];
    s32 a;
    s32 b;
    Pool6CEntry *e;
    u16 *p;

    if (arg0 == 0) {
        *arg4 = 0x2C;
    } else if (arg0 == 1) {
        a = arg4[0];
        b = arg4[1];
        arg1->unk0 = D_80104DF0.unk0 >> 6;
        arg1->unk4 = D_800F8D04[D_80104DF0.unk0 & 0xF];
        arg1->unk1 = D_80104DF4.unk0;
        arg1->unk14 = 0;
        arg1->unk2 = D_80104DF4.unk1 * 8;
        if (arg1->unk1 != 0xFF) {
            func_800E769C(arg1->unk1, 0, &arg1->unk1C.vx);
        } else {
            *(s32 *)&arg1->unk1C.vx = *(s32 *)&arg2->unkC->unk38;
            arg1->unk1C.vy -= 0xC0;
            *(s32 *)&arg1->unk1C.vz = *(s32 *)&arg2->unkC->unk3C;
        }
        switch (arg1->unk0) {
        case 0:
            arg1->unkC = a;
            arg1->unk8 = D_80104DF0.unk1;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[1] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            arg1->unk18 = (s8)D_80104DF0.unk2 * 4;
            arg1->unk1A = (s8)D_80104DF0.unk3 * 4;
            break;
        case 1:
            arg1->unkC = a;
            arg1->unk8 = D_80104DF0.unk1;
            arg1->unk6 = (s8)b << 8;
            arg1->unk10 = func_800E4530(arg1->unk8);
            ((u16 *)arg1->unk10)[2] = D_800F8CB4[arg1->unkC];
            arg1->unkE = *(u16 *)arg1->unk10 & 0x7FFF;
            break;
        case 2:
            arg1->unk8 = a;
            arg1->unkA = D_80104DF0.unk1;
            arg1->unk4 = b;
            arg1->unkE = D_80104DF0.unk0 & 0xF;
            break;
        case 3:
            arg1->unk8 = a;
            arg1->unkE = D_80104DF0.unk1;
            arg1->unk10 = func_800E4530(arg1->unk8);
            arg1->unk6 = 0;
            break;
        default:
            return 0;
        }
    } else {
        if (arg1->unk1 != 0xFF) {
            arg2->unk28 = func_800E7CC4(arg1->unk1, arg1->unk2);
            rot.vy = func_800E7C54(0x10, arg1->unk1) << 11;
            rot.pad = 0;
            rot.vx = 0;
            rot.vz = arg1->unk6;
        }
        switch (arg1->unk0) {
        case 0:
            func_800D1680(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000, arg1->unk18, arg1->unk1A);
            break;
        case 1:
            func_800D1D74(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, 0x1000);
            break;
        case 2:
            if (arg1->unk14 == arg1->unkE) {
                if (arg1->unk1 != 0xFF) {
                    if (arg1->unk1 == 0x10) {
                        func_800E7D3C(arg1->unk1, 1, &arg1->unk1C.vx);
                        func_800E769C(0, 0, v);
                        func_800CF808(&arg1->unk1C.vx, v, arg1->unk24);
                        if (func_800E716C(0) == func_800E716C(arg1->unk1) && arg1->unkA == 0x36) {
                            return 1;
                        }
                    } else {
                        func_800E769C(arg1->unk1, 0, &arg1->unk1C.vx);
                    }
                }
                e = func_800E3CEC((s32)func_800E4530(arg1->unkA), (s32)&arg1->unk1C);
                if (e != NULL) {
                    e->unk22 = arg1->unk2;
                }
                p = func_800E4530(arg1->unk8);
                if (e != NULL) {
                    e->unk18->unk1C = (s32)p;
                    e->unk18->unk0 = arg1->unk4;
                    e->unk18->unk8 = *p & 0x7FFF;
                }
                arg1->unk10 = e;
            }
            if (arg1->unkE < arg1->unk14) {
                e = arg1->unk10;
                if (arg1->unk1 == 0x10 && e != NULL) {
                    e->unk5C.vx = arg1->unk24[0];
                    e->unk5C.vy = arg1->unk24[1];
                    e->unk5C.vz = arg1->unk24[2];
                }
                if (e->unk30 == -1) {
                    return 1;
                }
            }
            goto next;
        case 3:
            func_800D23D8(arg1->unk10, &arg1->unk1C, &rot, arg1->unk4, arg1->unk14, 1, func_8004BB7C((arg1->unk14 << 10) / arg1->unkE));
            break;
        default:
            arg2->unk28 = 0;
            goto inc;
        }
        if (arg1->unk14 >= arg1->unkE - 1) {
            return 1;
        }
    next:
        arg2->unk28 = 0;
    inc:
        arg1->unk14++;
    }
    return 0;
}

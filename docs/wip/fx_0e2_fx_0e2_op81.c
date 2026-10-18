/* Draft of fx_0e2_op81 (module fx_0e2). Status: 203/422 aligned instructions with ccprobe; structure matches; work/ex/i get different callee-saved registers and the third loop's counter is not spilled.
 * Declarations it needs that are not yet in include/fxmod/fx_0e2.h are repeated below. */
#include "fxmod/fx_0e2.h"

s32 fx_0e2_op81(s32 mode, Fx0e2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Pool6CEntry **pp;
    SVECTOR v;
    s32 frame;
    s32 n;
    s32 m;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x13C;
    } else if (mode == 1) {
        D_801ECC0C = env;
        work->unk1C.vx = -0x18D;
        work->unk1C.vz = -0x25E;
        work->unk14.vx = 0;
        work->unk14.vy = 0;
        work->unk14.vz = 0;
        work->unk1C.vy = 0;
        work->unk2C.vx = 0x1000;
        work->unk2C.vy = 0x1000;
        work->unk2C.vz = 0x1000;
        for (i = 0; i < 3; i++) {
            work->unk124[i] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB68[i]), 1);
        }
        work->unk3C.vx = 0;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0;
        work->unk44.vz = 0;
        work->unk54.vx = 0x1000;
        work->unk54.vy = 0x1000;
        work->unk54.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            func_801EC508(&D_801EE99C[i], (u16 *)func_801EC3D8(ex->btlfx_getFileData(entry->unk8, 0x9C), (u16)i));
            work->unk124[i + 3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801ECB70[i]), 1);
        }
        for (i = 0; i < 4; i++) {
            setVector(&work->unk64[i], 0, 0, 0);
            setVector(&work->unk84[i], 0, 0, 0);
            setVector(&work->unkC4[i], 0x1000, 0x1000, 0x1000);
            work->unk124[i + 8] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 1);
        }
        return 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x300, 0x180, 0x80, 0x80, D_801EE9CC, -0x18D, -0x300, 0x25E);
            break;
        case 3:
            work->unk104[0].vx = work->unk44.vx;
            work->unk104[0].vy = work->unk44.vy;
            work->unk104[0].vz = work->unk44.vz;
            work->unk0[0] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[0]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 16:
            work->unk0[1] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[0]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 30:
            work->unk104[1].vx = 0x20C;
            work->unk104[1].vy = 0;
            work->unk104[1].vz = -0x3B6;
            work->unk0[2] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[1]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 35:
            work->unk104[2].vx = -0x205;
            work->unk104[2].vy = 0;
            work->unk104[2].vz = -0x5D9;
            work->unk0[3] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[2]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 38:
            work->unk104[3].vx = 0x110;
            work->unk104[3].vy = 0;
            work->unk104[3].vz = -0x703;
            work->unk0[4] = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA0), (s32)&work->unk104[3]);
            if (p != NULL) {
                p->unk22 = 0x100;
            }
            break;
        case 46:
            ex->func_800E3D54(work->unk0[0]);
            for (i = 0, pp = work->unk0; i < 4; i++, pp++) {
                ex->func_800E3D54(pp[1]);
            }
            break;
        }
        if ((u32)(frame - 1) < 0x2D) {
            if (frame - 1 >= 0x12) {
                m = work->unk124[2];
            } else if (frame - 1 >= 5) {
                m = work->unk124[1];
            } else {
                m = work->unk124[0];
            }
            ex->Hi_SetEffModelSlice(m, work->unk1C.vy);
            ex->Hi_DrawSliceEffModel(&work->unk14, &work->unk1C, (s32)&work->unk2C, m);
            ex->Hi_ModifyEffModelRGB(m, 6, 6, 6);
        }
        if ((u32)(frame - 1) < 0x2D) {
            n = frame - 1;
            for (i = 0; i < 5; i++) {
                func_801EC520(&D_801EE99C[i], n, &work->unk3C, &v);
                work->unk44.vx = v.vx;
                work->unk44.vy = v.vy;
                work->unk44.vz = v.vz;
                work->unk44.vx -= 0x18D;
                work->unk44.vz -= 0x25E;
                ex->Hi_SetEffModelSlice(work->unk124[i + 3], work->unk44.vy);
                ex->Hi_DrawSliceEffModel(&work->unk3C, &work->unk44, (s32)&work->unk54, work->unk124[i + 3]);
            }
        }
        if ((u32)(frame - 1) < 0x2D) {
            for (i = 1; i < 4; i++) {
                setVector(&work->unk84[i], D_801F103C[i].vx, D_801F103C[i].vy, D_801F103C[i].vz);
                ex->Hi_SetEffModelSlice(work->unk124[i + 8], work->unk84[i].vy);
                ex->Hi_DrawSliceEffModel(&work->unk64[i], &work->unk84[i], (s32)&work->unkC4[i], work->unk124[i + 8]);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

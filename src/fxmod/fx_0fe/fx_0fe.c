#include "fxmod/fx_0fe.h"

s32 fx_0fe_op80(s32 mode, Fx0feWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 j;
    s32 r;
    s32 m;
    Fx0feOrbit *o;
    Fx0feOrbit *q;
    Fx0feOrbit *p;

    if (mode == 0) {
        *arg4 = 0xC0;
    } else if (mode == 1) {
        D_801E7C44 = env;
        ex->btlfx_initEffModels(D_801E7C54, 3);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkA8);
        if (work->unkA8.vy >= -0x117) {
            work->unkA8.vy = -0x118;
        }
        work->unk94.vx = 0x8000;
        work->unk94.vy = 0x8000;
        work->unk94.vz = 0x8000;
        r = ex->btlfx_getUnitRadius(0x100);
        r = (r >= 0x899) ? 0x898 : r;
        q = D_801E7EB4;
        for (i = 0; i < 6; i++, q++) {
            q->unk0 = r;
            q->unk2 = i * 0x2AA;
            q->unk4 = i * 0x2AA;
            (work->unk4 + i)->vx = 0;
            (work->unk4 + i)->vy = 0;
            (work->unk4 + i)->vz = 0;
        }
        for (i = 0; i < 3; i++) {
            work->unkB8[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E7BF4[i]), 1);
        }
        work->unkB0 = -1;
        work->unkB4 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x16:
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkA8);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            break;
        case 0:
            work->unkB0 = 4;
            break;
        case 0x14:
            work->unkB4 = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1C) {
            if (work->unkB0 >= 0 || work->unkB4 >= 0) {
                if (work->unkB0 >= 0) {
                    work->unkA4 = func_801E7B9C(-0x80, 0, 4 - work->unkB0, 4);
                    work->unkB0--;
                }
                if (work->unkB4 >= 0) {
                    work->unkA4 = func_801E7B9C(0, -0x80, 8 - work->unkB4, 8);
                    work->unkB4--;
                }
            }
            o = D_801E7EB4;
            m = work->unkB8[0];
            for (j = 0; j < 6; j++, o++) {
                rot.vx = 0;
                rot.vy = o->unk2;
                rot.vz = 0;
                ex->func_800CFD78((s16 *)&work->unkA8, (s16 *)&rot, o->unk0, (s16 *)&pos);
                (work->unk34 + j)->vx = pos.vx;
                (work->unk34 + j)->vy = pos.vy;
                (work->unk34 + j)->vz = pos.vz;
                ex->Hi_DrawEffModel(&work->unk4[j], &work->unk34[j], (s32)&work->unk94, m);
                if (work->unkB0 >= 0 || work->unkB4 >= 0) {
                    ex->Hi_ModifyEffModelAbr(m, 1);
                    ex->Hi_ModifyEffModelRGB(m, work->unkA4, work->unkA4, work->unkA4);
                }
                m++;
                if (m > work->unkB8[2]) {
                    m = work->unkB8[0];
                }
                o->unk2 += 0x55;
                o->unk4 += 0x200;
                work->unk4[j].vy += 0x80;
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7B58(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7C44)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7B9C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0fe/nonmatchings/fx_0fe", fx_0fe_headerPtr);

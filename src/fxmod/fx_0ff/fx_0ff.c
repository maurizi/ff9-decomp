#include "fxmod/fx_0ff.h"

s32 fx_0ff_op80(s32 mode, Fx0ffWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 j;
    s32 r;
    s32 m;
    Fx0ffOrbit *o;
    Fx0ffOrbit *q;
    Fx0ffOrbit *p;

    if (mode == 0) {
        *arg4 = 0xCC;
    } else if (mode == 1) {
        D_801E7CB8 = env;
        ex->btlfx_initEffModels(D_801E7CC8, 3);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkAC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkB4);
        if (work->unkAC.vy >= -0x117) {
            work->unkAC.vy = -0x118;
        }
        work->unk98.vx = 0x8000;
        work->unk98.vy = 0x8000;
        work->unk98.vz = 0x8000;
        r = ex->btlfx_getUnitRadius(0x100);
        r = (r >= 0x899) ? 0x898 : r;
        q = D_801E7F28;
        for (i = 0; i < 6; i++, q++) {
            q->unk0 = r;
            q->unk2 = i * 0x2AA;
            q->unk4 = i * 0x2AA;
            (work->unk8 + i)->vx = 0;
            (work->unk8 + i)->vy = 0;
            (work->unk8 + i)->vz = 0;
        }
        for (i = 0; i < 3; i++) {
            work->unkC4[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E7C68[i]), 1);
        }
        work->unkBC = -1;
        work->unkC0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 2:
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unkB4);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x16:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unkAC);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0:
            work->unkBC = 4;
            break;
        case 0x14:
            work->unkC0 = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1C) {
            if (work->unkBC >= 0 || work->unkC0 >= 0) {
                if (work->unkBC >= 0) {
                    work->unkA8 = func_801E7C10(-0x80, 0, 4 - work->unkBC, 4);
                    work->unkBC--;
                }
                if (work->unkC0 >= 0) {
                    work->unkA8 = func_801E7C10(0, -0x80, 8 - work->unkC0, 8);
                    work->unkC0--;
                }
            }
            o = D_801E7F28;
            m = work->unkC4[0];
            for (j = 0; j < 6; j++, o++) {
                rot.vx = 0;
                rot.vy = o->unk2;
                rot.vz = 0;
                ex->btlfx_pointAhead((s16 *)&work->unkAC, (s16 *)&rot, o->unk0, (s16 *)&pos);
                (work->unk38 + j)->vx = pos.vx;
                (work->unk38 + j)->vy = pos.vy;
                (work->unk38 + j)->vz = pos.vz;
                ex->Hi_DrawEffModel(&work->unk8[j], &work->unk38[j], (s32)&work->unk98, m);
                if (work->unkBC >= 0 || work->unkC0 >= 0) {
                    ex->Hi_ModifyEffModelAbr(m, 1);
                    ex->Hi_ModifyEffModelRGB(m, work->unkA8, work->unkA8, work->unkA8);
                }
                m++;
                if (m > work->unkC4[2]) {
                    m = work->unkC4[0];
                }
                o->unk2 += 0x55;
                o->unk4 += 0x200;
                work->unk8[j].vy += 0x80;
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7BCC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7CB8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C10(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0ff/nonmatchings/fx_0ff", fx_0ff_headerPtr);

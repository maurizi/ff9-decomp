#include "fxmod/fx_126.h"

s32 fx_126_op80(s32 mode, Fx126Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 k;

    if (mode == 0) {
        *arg4 = 0xAC;
    } else if (mode == 1) {
        D_801E80DC = env;
        ex->func_800DC818(0x10, 0x30, (u16 *)&work->unk60);
        ex->func_800DC818(0x10, 8, (u16 *)&work->unk58);
        ex->btlfx_initEffModels(D_801E80EC, 2);
        work->unk0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 0xFF);
        work->unk4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 0xFF);
        work->unk28.vy = 0x700;
        work->unk28.vz = 0x400;
        work->unk30.vy = -0x700;
        work->unk30.vz = -0x400;
        work->unk38.vx = 0x2000;
        work->unk38.vy = 0x2000;
        work->unk38.vz = 0x2000;
        work->unk50.vz = -0x6E0;
        work->unk68.vx = 0x692;
        work->unk70.vx = -0x692;
        work->unk28.vx = 0;
        work->unk30.vx = 0;
        work->unk50.vx = 0;
        work->unk50.vy = 0;
        work->unk68.vy = 0;
        work->unk70.vy = 0;
        work->unk68.vz = (work->unk58.vz - 0x6E0) >> 1;
        work->unk70.vz = (work->unk60.vz - 0x6E0) >> 1;
        for (k = 3; k >= 0; k--) {
            work->unk9C[k] = 0;
        }
    } else {
    frame = entry->unk0;
    if (frame >= 1 && frame <= 0x18) {
        func_801E7D48(0x18, frame, &work->unk58, &work->unk70, &work->unk50, &work->unk68, &work->unk60, &work->unk8);
        ex->Hi_DrawEffModel(&work->unk28, &work->unk8, (s32)&work->unk38, work->unk0);
        func_801E7D48(0x18, frame, &work->unk60, &work->unk68, &work->unk50, &work->unk70, &work->unk58, &work->unk18);
        ex->Hi_DrawEffModel(&work->unk30, &work->unk18, (s32)&work->unk38, work->unk4);
        work->unk28.vy -= 0x180;
        work->unk30.vy += 0x180;
        env->camera->points[0].vx = work->unk8.vx;
        env->camera->points[0].vy = work->unk8.vy;
        env->camera->points[0].vz = work->unk8.vz;
        env->camera->points[1].vx = work->unk18.vx;
        env->camera->points[1].vy = work->unk18.vy;
        env->camera->points[1].vz = work->unk18.vz;
    }
    switch (frame) {
    case 8:
        work->unk9C[0] = 1;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk7C[0]);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk7C[0]);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk7C[0]);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        k = 3;
        if (k >= env->targets->targetCount) {
            k = env->targets->targetCount - 1;
        }
        if (work->unk9C[k] == 0) {
            work->unk9C[k] = 1;
            ex->btlfx_getUnitBodyPos(k, 0, (s16 *)&work->unk7C[k]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
        }
        break;
    case 10:
        k = 1;
        if (k >= env->targets->targetCount) {
            k = env->targets->targetCount - 1;
        }
        if (work->unk9C[k] == 0) {
            work->unk9C[k] = 1;
            ex->btlfx_getUnitBodyPos(k, 0, (s16 *)&work->unk7C[k]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
        }
        k = 2;
        if (k >= env->targets->targetCount) {
            k = env->targets->targetCount - 1;
        }
        if (work->unk9C[k] == 0) {
            work->unk9C[k] = 1;
            ex->btlfx_getUnitBodyPos(k, 0, (s16 *)&work->unk7C[k]);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk7C[k]);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(k, 0x80);
            }
        }
        break;
    }
    return frame >= 0x20;
    }
    return 0;
}

void func_801E7D48(s32 len, s32 t, SVECTOR *p0, SVECTOR *p1, SVECTOR *p2, SVECTOR *p3, SVECTOR *p4, VECTOR *out) {
    s32 w[5];
    s32 s;
    s32 r;

    s = ((0x80000 / len) * t) >> 12;
    r = 0x80 - s;
    w[0] = (r * r * r * r) >> 16;
    w[1] = (s * r * r * r) >> 13;
    w[2] = (s * s * r * r * 24) >> 16;
    w[3] = (s * s * s * r) >> 13;
    w[4] = (s * s * s * s) >> 16;
    out->vx = (p0->vx * w[0]) >> 12;
    out->vy = (p0->vy * w[0]) >> 12;
    out->vz = (p0->vz * w[0]) >> 12;
    out->vx += (p1->vx * w[1]) >> 12;
    out->vy += (p1->vy * w[1]) >> 12;
    out->vz += (p1->vz * w[1]) >> 12;
    out->vx += (p2->vx * w[2]) >> 12;
    out->vy += (p2->vy * w[2]) >> 12;
    out->vz += (p2->vz * w[2]) >> 12;
    out->vx += (p3->vx * w[3]) >> 12;
    out->vy += (p3->vy * w[3]) >> 12;
    out->vz += (p3->vz * w[3]) >> 12;
    out->vx += (p4->vx * w[4]) >> 12;
    out->vy += (p4->vy * w[4]) >> 12;
    out->vz += (p4->vz * w[4]) >> 12;
}

INCLUDE_RODATA("asm/us/fx_126/nonmatchings/fx_126", fx_126_headerPtr);

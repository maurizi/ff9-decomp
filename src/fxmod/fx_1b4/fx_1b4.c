#include "fxmod/fx_1b4.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[0], frame, n);
}

s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[1], frame, n);
}

s32 func_801E776C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[2], frame, n);
}

s32 func_801E77A0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[3], frame, n);
}

s32 func_801E77D4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[4], frame, n);
}

s32 func_801E7808(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[5], frame, n);
}

s32 func_801E783C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[6], frame, n);
}

s32 func_801E7870(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E78A4(pt, pos, &D_801E86A4[7], frame, n);
}

s32 func_801E78A4(Pool30Entry *e, SVECTOR *pos, Fx1b4Path *path, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8558(path, frame, &v);
    v.vx += D_801EF174.vx;
    v.vy += D_801EF174.vy;
    v.vz += D_801EF174.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1b4_op80(s32 mode, Fx1b4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    SVECTOR *v;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x94;
    } else if (mode == 1) {
        work->unk44.vx = env->targets->centerX;
        work->unk44.vy = env->targets->centerY;
        work->unk44.vz = env->targets->centerZ;
        D_801EF174.vx = work->unk44.vx;
        D_801EF174.vy = work->unk44.vy;
        D_801EF174.vz = work->unk44.vz;
        D_801E8654 = env;
        work->unk8C.vx = work->unk44.vx;
        work->unk8C.vy = work->unk44.vy;
        work->unk8C.vz = work->unk44.vz;
        work->unk8C.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            for (i = 0; i < 8; i++) {
                func_801E8540(&D_801E8664[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E85DC[i]));
                func_801E8558(&D_801E8664[i], 0, &work->unk4C[i]);
                v = (SVECTOR *)((u8 *)work + i * 8 + 0x4C);
                v->vx += work->unk44.vx;
                v->vy += work->unk44.vy;
                v->vz += work->unk44.vz;
                work->unk0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E85BC[i]), (s32)&work->unk4C[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk14 = (s32)D_801E86E4[i];
                    work->unk0[i]->unk12 = 5;
                    work->unk0[i]->unkC = 0x30;
                }
            }
            break;
        case 0x17:
            for (i = 0; i < 8; i++) {
                func_801E8540(&D_801E86A4[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E85DC[i]));
                work->unk20[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E85CC[i]), (s32)&D_801EF17C);
                if (work->unk20[i] != NULL) {
                    work->unk20[i]->unk14 = (s32)D_801E8E64[i];
                    work->unk20[i]->unk12 = 8;
                    work->unk20[i]->unkC = 0x70;
                    if (work->unk20[i] != NULL) {
                        ((EffMotion *)work->unk20[i]->file)->unk8 = D_801E85EC[i];
                    }
                }
            }
            break;
        case 0x25:
            work->unk40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x29), (s32)&work->unk8C);
            break;
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_1b4_op81(s32 mode, Fx1b4Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    VECTOR scale2;
    SVECTOR rot2;
    CVECTOR col2;
    DrMode2Words *dm;
    DrMode2Words *dm2;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        D_801E8654 = env;
        work->unk10.vz = env->targets->centerZ;
        work->unk10.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk10);
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk10);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->otZ = 0x100;
            }
            if (work->unk0[1] != NULL) {
                work->unk0[1]->otZ = 0x80;
            }
            break;
        case 0x1C:
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk10);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk10);
            if (work->unk0[2] != NULL) {
                work->unk0[2]->unk14 = (s32)D_801EAA64;
                work->unk0[2]->unk12 = 0xC0;
                work->unk0[2]->unkC = 0x30;
                if (work->unk0[2] != NULL) {
                    work->unk0[2]->otZ = 0x80;
                }
            }
            if (work->unk0[3] != NULL) {
                work->unk0[3]->otZ = 0x100;
            }
            break;
        case 0x1E:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        if ((u32)(frame - 0x1F) < 0x3C) {
            dm = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->ot[1], dm);
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            col.r = col.g = col.b = 0x28;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        t = frame - 0x5B;
        if ((u32)t < 0xF) {
            dm2 = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm2, 2);
            dm2->code[0] = 0xE6000001;
            dm2->code[1] = 0;
            addPrim(&env->ot[1], dm2);
            scale2.vz = scale2.vy = scale2.vx = (t << 7) + 0x1000;
            col2.r = col2.g = col2.b = 0x28;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot2, &scale2, &col2, 1, 0);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x1C), frame);
        return frame >= 0x8C;
    }
    return 0;
}

s32 fx_1b4_op82(s32 mode, Fx1b4Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        work->unk1C.vx = env->targets->centerX;
        work->unk1C.vy = env->targets->centerY;
        D_801E8654 = env;
        work->unk1C.vz = env->targets->centerZ;
        work->unk1C.vy -= 0x258;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk1C);
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk1C);
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk1C);
            if (work->unk0[0] != NULL) {
                work->unk0[0]->otZ = 0x100;
            }
            break;
        case 20:
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk1C);
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->unk1C);
            work->unk0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->unk1C);
            work->unk0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2A), (s32)&work->unk1C);
            if (work->unk0[4] != NULL) {
                work->unk0[4]->unk14 = (s32)D_801ECE64;
                work->unk0[4]->unk12 = 0x21;
                work->unk0[4]->unkC = 0x90;
            }
            if (work->unk0[5] != NULL) {
                work->unk0[5]->unk14 = (s32)D_801EE0F4;
                work->unk0[5]->unk12 = 0x58;
                work->unk0[5]->unkC = 0x30;
            }
            if (work->unk0[3] != NULL) {
                work->unk0[3]->otZ = 0x80;
            }
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1b4_op83(s32 mode, Fx1b4Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->targets->centerX;
        work->unk4.vy = env->targets->centerY;
        D_801E8654 = env;
        work->unk4.vz = env->targets->centerZ;
        work->unk4.vy -= 0x258;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x2B), (s32)&work->unk4);
            if (work->unk0 != NULL) {
                work->unk0->otZ = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

void func_801E8540(Fx1b4Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1b4Point *)data;
}

s32 func_801E8558(Fx1b4Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1b4Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

INCLUDE_RODATA("asm/us/fx_1b4/nonmatchings/fx_1b4", fx_1b4_headerPtr);

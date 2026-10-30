#include "fxmod/fx_0b4.h"

s32 func_801E77BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[0], &D_801E81C4[0], frame, n);
}

s32 func_801E77FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[1], &D_801E81C4[1], frame, n);
}

s32 func_801E783C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[2], &D_801E81C4[2], frame, n);
}

s32 func_801E787C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[3], &D_801E81C4[3], frame, n);
}

s32 func_801E78BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[4], &D_801E81C4[4], frame, n);
}

s32 func_801E78FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E793C(pt, pos, &D_801E80EC[5], &D_801E81C4[5], frame, n);
}

s32 func_801E793C(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n) {
    SVECTOR rot;
    SVECTOR p;

    func_801E8058(path, frame, &rot, &p);
    p.vx /= 2;
    p.vz /= 2;
    p.vx = p.vx * 12 / 10;
    p.vy = p.vy * 12 / 10;
    p.vz = p.vz * 12 / 10;
    p.vx += D_801E818C.vx;
    p.vy += D_801E818C.vy;
    p.vz += D_801E818C.vz;
    pos->vx = p.vx;
    pos->vy = p.vy;
    pos->vz = p.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b4/nonmatchings/fx_0b4", fx_0b4_headerPtr);

s32 fx_0b4_op80(s32 mode, Fx0b4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 k;
    s32 s;
    Fx0b4Timer *tm;
    Fx0b4Timer *tq;
    u16 *cnt;
    Pool6CEntry **pp;
    s32 *anim;
    SVECTOR *pos;
    u16 *file;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E8184 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk2C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk34);
        D_801E818C.vx = work->unk34.vx;
        D_801E818C.vy = work->unk34.vy;
        D_801E818C.vz = work->unk34.vz;
        for (k = 0, tq = D_801E80EC; k < 6; k++, tq++) {
            tq->unk0 = k * 2 + 0x14;
            func_801E8040(&D_801E81C4[k], ex->btlfx_getFileData(entry->file, D_801E8110[k]));
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk34);
            break;
        case 8:
            work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk34);
            break;
        case 20:
            work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->unk34);
            if (work->unk18 != NULL) {
                work->unk18->unk14 = (s32)D_801ED2F4;
                work->unk18->unk12 = 0x12;
                work->unk18->unkC = 0x98;
            }
            break;
        case 25:
            work->unk24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk2C);
            break;
        case 45:
            work->unk28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk34);
            break;
        }
        tm = D_801E80EC;
        for (i = 0, cnt = &tm->unk2; i < 6; i++, cnt += 2, tm++) {
            if (frame == tm->unk0) {
                work->unk0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8104[i]), (s32)&D_801E8194[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk14 = (s32)D_801E81F4[i];
                    work->unk0[i]->unk12 = 0x18;
                    work->unk0[i]->unkC = 0x90;
                }
                if (work->unk0[i] != NULL) {
                    ((EffMotion *)work->unk0[i]->file)->unk8 = D_801E8124[i];
                }
            }
            if (frame >= tm->unk0) {
                (*cnt)++;
            }
        }
        if (frame >= 0) {
            if (work->unk1C != NULL) {
                if (work->unk1C->state == -1) {
                    work->unk1C = NULL;
                }
                if (work->unk1C != NULL) {
                    work->unk1C->unk5C.vx = D_801E811C.vx;
                    work->unk1C->unk5C.vy = D_801E811C.vy;
                    work->unk1C->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 8) {
            if (work->unk20 != NULL) {
                if (work->unk20->state == -1) {
                    work->unk20 = NULL;
                }
                if (work->unk20 != NULL) {
                    work->unk20->unk5C.vx = D_801E811C.vx;
                    work->unk20->unk5C.vy = D_801E811C.vy;
                    work->unk20->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x14) {
            if (work->unk18 != NULL) {
                if (work->unk18->state == -1) {
                    work->unk18 = NULL;
                }
                if (work->unk18 != NULL) {
                    work->unk18->unk5C.vx = D_801E811C.vx;
                    work->unk18->unk5C.vy = D_801E811C.vy;
                    work->unk18->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x2D) {
            if (work->unk28 != NULL) {
                if (work->unk28->state == -1) {
                    work->unk28 = NULL;
                }
                if (work->unk28 != NULL) {
                    work->unk28->unk5C.vx = D_801E811C.vx;
                    work->unk28->unk5C.vy = D_801E811C.vy;
                    work->unk28->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        return frame >= 0x5A;
    }
    return 0;
}

void func_801E8040(Fx0b4Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0b4Point *)data;
}

s32 func_801E8058(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0b4Key *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}

#include "fxmod/fx_191.h"

s32 func_801E7704(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[0].vx -= 0x96;
    return func_801E7870(e, pos, &D_801E84E4[0], &D_801E857C[0], frame, frames);
}

s32 func_801E775C(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[1].vx += 0x96;
    return func_801E7870(e, pos, &D_801E84E4[1], &D_801E857C[1], frame, frames);
}

s32 func_801E77B8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[2].vx -= 0x32;
    return func_801E7870(e, pos, &D_801E84E4[2], &D_801E857C[2], frame, frames);
}

s32 func_801E7814(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[3].vx += 0x32;
    return func_801E7870(e, pos, &D_801E84E4[3], &D_801E857C[3], frame, frames);
}

s32 func_801E7870(Pool30Entry *e, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E855C);
    SVECTOR a;
    SVECTOR b;
    s32 t;

    a.vx = D_801E8574.vx + base->vx;
    t = (D_801E8D9C << 12) / 12;
    a.vy = D_801E8574.vy + base->vy;
    a.vz = D_801E8574.vz + base->vz;
    ex->btlfx_lerpVec((s16 *)&a, (s16 *)to, t, (s16 *)&b);
    pos->vx = b.vx;
    pos->vy = b.vy;
    pos->vz = b.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E7988(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    return func_801E7A00(e, pos, &z, frame, frames);
}

s32 func_801E79C4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    return func_801E7B24(e, pos, &z, frame, frames);
}

s32 func_801E7A00(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8478(&D_801E8564, D_801E8DA0 * 3, &v);
    v.vx += D_801E859C.vx;
    v.vy += D_801E859C.vy;
    v.vz += D_801E859C.vz;
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E7B24(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8478(&D_801E856C, (D_801E8DA0 - 5) * 3, &v);
    v.vx += D_801E859C.vx;
    v.vy += D_801E859C.vy;
    v.vz += D_801E859C.vz;
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_191_op80(s32 mode, Fx191Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E855C = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        D_801E8574.vx = work->unk10.vx;
        D_801E8574.vy = work->unk10.vy;
        D_801E8574.vz = work->unk10.vz;
        for (i = 0; i < 4; i++) {
            work->unk0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E84DC[i]), (s32)&D_801E85CC);
            if (work->unk0[i] != NULL) {
                work->unk0[i]->unk14 = (s32)D_801E85DC[i];
                work->unk0[i]->unk12 = 4;
                work->unk0[i]->unkC = 0x60;
            }
            if (work->unk0[i] != NULL) {
                ((EffMotion *)work->unk0[i]->file)->unk8 = D_801E8504[i];
            }
            D_801E857C[i].vx = work->unk18.vx;
            D_801E857C[i].vy = work->unk18.vy;
            D_801E857C[i].vz = work->unk18.vz;
        }
        D_801E857C[0].vx -= 0xC8;
        D_801E857C[0].vy -= 0xC8;
        D_801E857C[1].vx += 0xC8;
        D_801E857C[0].vy -= 0xC8;
        D_801E857C[2].vy -= 0x320;
        D_801E857C[3].vy -= 0xC8;
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk18, (s16 *)&work->unk20);
    } else {
        frame = entry->frame;
        D_801E8D9C = frame;
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_191_op81(s32 mode, Fx191Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        D_801E855C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        D_801E859C.vx = work->unk18.vx;
        D_801E859C.vy = work->unk18.vy;
        D_801E859C.vz = work->unk18.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E85CC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8BDC;
            work->unk0->unk12 = 2;
            work->unk0->unkC = 0x70;
        }
        if (work->unk0 != NULL) {
            ((EffMotion *)work->unk0->file)->unk8 = func_801E7988;
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
        func_801E8460(&D_801E8564, (u8 *)ex->btlfx_getFileData(entry->file, 4));
        func_801E8460(&D_801E856C, (u8 *)ex->btlfx_getFileData(entry->file, 5));
    } else {
        frame = entry->frame;
        D_801E8DA0 = frame;
        switch (frame) {
        case 5:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&D_801E85D4);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E8CBC;
                work->unk4->unk12 = 2;
                work->unk4->unkC = 0x70;
            }
            if (work->unk4 != NULL) {
                ((EffMotion *)work->unk4->file)->unk8 = func_801E79C4;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 3:
            work->unk30.vx = -0x140;
            work->unk30.vy = 0x8C;
            work->unk38.vx = 0x154;
            work->unk38.vy = -0x82;
            work->unk30.vz = 0;
            work->unk38.vz = 0;
            work->unk30.vx += work->unk18.vx;
            work->unk30.vy += work->unk18.vy;
            work->unk30.vz += work->unk18.vz;
            work->unk38.vx += work->unk18.vx;
            work->unk38.vy += work->unk18.vy;
            work->unk38.vz += work->unk18.vz;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->unk30);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 8:
            work->unk40.vx = 0xFA;
            work->unk40.vy = -0x12C;
            work->unk48.vx = -0x12C;
            work->unk48.vy = 0x140;
            work->unk40.vz = 0;
            work->unk48.vz = 0;
            work->unk40.vx += work->unk18.vx;
            work->unk40.vy += work->unk18.vy;
            work->unk40.vz += work->unk18.vz;
            work->unk48.vx += work->unk18.vx;
            work->unk48.vy += work->unk18.vy;
            work->unk48.vz += work->unk18.vz;
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk40);
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk18);
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unk14 != NULL) {
                work->unk14->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        }
        if (frame >= 3) {
            t = (frame - 3) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->unk30, (s16 *)&work->unk38, t, (s16 *)&work->unk50);
            if (work->unk8 != NULL && work->unk8->state == -1) {
                work->unk8 = NULL;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk2C = (s32)&work->unk50;
            }
        }
        if (frame >= 8) {
            t = (frame - 8) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->unk40, (s16 *)&work->unk48, t, (s16 *)&work->unk58);
            if (work->unk10 != NULL && work->unk10->state == -1) {
                work->unk10 = NULL;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk2C = (s32)&work->unk58;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E8460(Fx191Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx191Point *)data;
}

s32 func_801E8478(Fx191Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx191Point *p;

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

INCLUDE_RODATA("asm/us/fx_191/nonmatchings/fx_191", fx_191_headerPtr);

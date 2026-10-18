#include "fxmod/fx_026.h"

void func_801E7704(Fx026Track *track, s32 t, SVECTOR *out) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    track = (Fx026Track *)&track->unk4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

void func_801E7754(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

void func_801E77DC(Fx026Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    k = &track->unk4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

s32 func_801E7890(Fx026Track *track, s32 t) {
    if (t >= track->unk2) {
        t = track->unk2 - 1;
    }
    return (track->unk4 + t)[0];
}

void func_801E78BC(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801EACCC;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", func_801E7A5C);

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", fx_026_op80);

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", func_801E8670);

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", fx_026_op82);

void func_801E912C(s32 i, s32 n, Fx026Shatter *buf, Fx026Tri *tris) {
    Fx026Shatter *h;

    D_801ED9CC[i] = buf;
    D_801ED9EC[i] = (Fx026Shard *)(buf + 1);
    h = D_801ED9CC[i];
    h->unk0 = n;
    h->unk2 = 0;
    h->unkC = tris;
    h->unk8 = tris;
}

s32 func_801E9170(s32 i, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EACCC);
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Tri *t = h->unkC;
    Fx026Shard *p;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    h->unk10 = t;
    h->unk4 = n;
    h->unkC = t + n;
    h->unk2 += n;
    for (k = 0; k < n; k++, t++) {
        t->unk0.pad = 0;
    }
    p = D_801ED9EC[i];
    for (k = 0; k < n; k++, p++) {
        p->unk18 = -1;
    }
    return h->unk2;
}

Fx026Shard *func_801E928C(s32 i) {
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Shard *p = D_801ED9EC[i];
    s32 k;

    for (k = 0; k < h->unk2; k++, p++) {
        if (p->unk18 < 0) {
            return p;
        }
    }
    return NULL;
}

s32 func_801E92EC(s32 i) {
    s32 idx = 0;
    s32 k = 0;
    Fx026Shatter *h = D_801ED9CC[i];
    Fx026Tri *t = h->unk8;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EACCC);
    s32 n = 0;
    s32 y = D_801EDA08;
    Fx026Shard *p;
    SVECTOR v;

    for (; k < h->unk2; k++, t++, idx++) {
        if (y < t->unk8.vy && t->unk0.pad == 0) {
            p = func_801E928C(i);
            if (p != NULL) {
                t->unk0.pad = 1;
                p->unk0 = ((t->unk0.vx + t->unk8.vx + t->unk10.vx) / 3) << 12;
                p->unk4 = ((t->unk0.vy + t->unk8.vy + t->unk10.vy) / 3) << 12;
                p->unk8 = ((t->unk0.vz + t->unk8.vz + t->unk10.vz) / 3) << 12;
                p->unk18 = 0;
                p->unk1A = idx;
                p->unk12 = 0x1000;
                p->unk14 = ex->btlfx_randCentered(0x100);
                p->unk16 = ex->btlfx_randCentered(0x200);
                v.vx = p->unk0 >> 12;
                v.vy = p->unk4 >> 12;
                v.vz = p->unk8 >> 12;
                p->unkC = (v.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                p->unkE = (v.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                p->unk10 = (v.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", func_801E9564);

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", func_801E98BC);

INCLUDE_ASM("asm/us/fx_026/nonmatchings/fx_026", fx_026_op81);

s32 fx_026_op87(s32 mode, Fx026Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unk0 = *arg4;
        D_801EACCC = env;
    } else {
        frame = entry->unk0;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, D_801EA8C4[work->unk0].unk2), frame,
                      &work->unk4);
        env->unk2C->unk0[1].vx = work->unk4.vx;
        env->unk2C->unk0[1].vy = work->unk4.vy;
        env->unk2C->unk0[1].vz = work->unk4.vz;
        func_801E7704((Fx026Track *)ex->btlfx_getFileData(entry->unk8, D_801EA8C4[work->unk0].unk0), frame,
                      &work->unkC);
        env->unk2C->unk0[0].vx = work->unkC.vx;
        env->unk2C->unk0[0].vy = work->unkC.vy;
        env->unk2C->unk0[0].vz = work->unkC.vz;
        if (frame >= D_801EA8C4[work->unk0].unk4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_026/nonmatchings/fx_026", fx_026_headerPtr);

#include "fxmod/fx_17d_6.h"

INCLUDE_ASM("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_op80);

void func_801E8B10(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801E93B4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    a = cnt;
    trail->unk0 = trail->unk2 = a - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < a; i++, pt++) {
        t = 0x1000 - (i << 12) / (a - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++ / 3;
        h = (ex->math_sin((t >> 1) >> 1) * h) >> 12;
        t >>= 1;
        h += ex->btlfx_lerp(h0, h1, ex->math_sin(t));
        ex->func_800D044C(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->unk28 = 0;
}

void func_801E8DBC(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *path, Fx17d6Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E93B4);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E8B10(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801E8ED4(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 a;
    s32 c;
    s32 k;
    s32 i;
    Fx17d6Trail *tr;
    Fx17d6Trail *tr2;
    s32 u;

    if (mode != 1 && b->unk11 == 0) {
        t = (frame << 12) / 6;
        c = ex->math_cos(t >> 2);
        k = frame < 3;
        a = 0x1000 - t;
        i = b->unk10;
        u = b->unk15;
        tr = (Fx17d6Trail *)&D_801EB124[i];
        tr2 = (Fx17d6Trail *)&D_801EB474[i];
        b->unk14 += 3;
        func_801E8B10(entry, 8, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0xDC, b->unk14 * 16, 0xF0,
                      frame, c, a * u / 1500, 0);
        func_801E8DBC(entry, 7, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_6_op83(s32 mode, Fx17d6Work83 *work, Unk800F78A4 *env, Fx17d6Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 frame;
    Fx17d6Bolt *b;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E8ED4;
        init->unk4 = 6;
    } else if (mode == 1) {
        n = init->unk0;
        D_801E93B4 = env;
        work->unk0 = D_801EB584[0];
        p = D_801EB584[0];
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0xD2);
            *p++ = ex->btlfx_randCentered(0x1C3);
            *p++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        frame = inst->unk0;
        if (work->unk8 != 0) {
            return 0;
        }
        if (D_801EB114 == 0) {
            if (frame < 0x43 && (frame & 3)) {
                return 0;
            }
            work->unk4++;
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EB104)[0];
                ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EB104)[1];
                ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EB10C)[0];
                ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EB10C)[1];
                b->unk11 = 0;
                b->unk10 = work->unk4 % 6;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0xB;
                b->unk14 = ex->btlfx_rand();
            }
        }
        if (D_801EB114 == 1) {
            ex->btlfx_poolClear((Unk800DA1B8 *)inst->unk10);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_headerPtr);

#include "fxmod/fx_0df.h"

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E7704);

void func_801E7EB8(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *arg3, s32 *arg4, s32 *arg5, s32 arg6, s32 arg7) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9E90);

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(D_801EA414->unk8, 0x10), pos, arg3, *arg4 / 2, arg7, 1, *arg5, 0, 0);
    BTLFX_COPY_SVEC(&D_801EA350[D_801EA410], pos);
    D_801EA410 = (D_801EA410 + 1) % 24;
}

void func_801E7FE0(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - r) << 12) / 108;
    t = (t * D_801EA418) >> 12;
    r = (r * 9) >> 3;
    ex = BTLFX_EXPORTS(D_801E9E90);
    a += 0x9B;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

void func_801E80B0(SVECTOR *a, SVECTOR *b) {
    BTLFX_EXPORTS(D_801E9E90)->btlfx_aimAngles((s16 *)&D_801E9E90->unk2C->unk54, (s16 *)a, (s16 *)b);
}

s32 func_801E80F4(s32 mode, Fx0dfPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0dfWork *work) {
    s16 half;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 t;
    s32 b;
    s32 c;
    s32 r;
    s32 g;
    s32 k;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
    t = (frame << 12) / 12;
    p->unk14 += p->unk16;
    func_801E80B0(&p->unk4, &rot);
    rot.vz = p->unk14;
    pos.vx = p->unk4.vx;
    pos.vy = p->unk4.vy;
    pos.vz = p->unk4.vz;
    k = ex->math_sin(t >> 2);
    half = p->unk2 >> 1;
    k *= 3;
    scale.vx = scale.vy = scale.vz = ((p->unk2 * k) >> 13) + half;
    if (frame < 3) {
        b = ((frame + 1) << 12) / 3;
    } else {
        b = ex->math_cos(((frame - 3) << 10) / 9);
    }
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
    ex->Hi_ModifyEffModelRGB(work->unk28, (b >> 5) - 0x80, (b >> 5) - 0x80, (b >> 5) - 0x80);
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk2C);
    ex->Hi_ModifyEffModelRGB(work->unk2C, ((b * 2 - 0x1000) >> 5) - 0x80, ((b * 2 - 0x1000) >> 5) - 0x80,
                             ((b * 2 - 0x1000) >> 5) - 0x80);
    if (frame < 8) {
        p->unkC.vz += 0xFF;
        scale.vx = scale.vy = scale.vx / 5;
        scale.vz = scale.vx / 7;
        ex->Hi_DrawEffModel(&p->unkC, &pos, (s32)&scale, work->unk20);
        c = frame << 10;
        c >>= 3;
        r = (ex->math_cos(c) >> 5) - 0x80;
        g = (ex->math_cos(c) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk20, r, g, (ex->math_cos(c) >> 5) - 0x80);
    }
    env->unk28 = 0x200;
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &p->unk4, 0, (p->unk2 * 3) >> 1, 0, 1, b >> 1, 0, 0);
    env->unk28 = 0;
    if (frame >= 0xC) {
        return 1;
    }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_op80);

void func_801E93F4(Unk80100C78Entry *entry, Fx0dfBolt *bolt, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 arg9, s32 arg10, s32 pad) {
    Unk800F78A4 *env = D_801E9E90;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0xE;
    bolt->unk0 = 0xE;
    bolt->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *jitter++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = *jitter++;
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + *jitter++;
        pt++;
    }
    env->unk28 = 0x200;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), 0, 0, arg9, 0x1000, arg10 * 2 / 3, 2, (u8 *)bolt, 0);
    env->unk28 = 0;
}

s32 func_801E9600(s32 mode, Fx0dfArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    if (mode != 1) {
        func_801E93F4(entry, &D_801EA420[arc->unk10], arc->unk14, &arc->unk0, &arc->unk8, 0, 0x200, frame * 0x9B, 0x186,
                      2, 0x1000, ((0x1000 - (frame << 12) / 5) << 6) >> 12);
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0df_op83(s32 mode, Fx0dfWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx0dfArc *arc;
    Fx0dfArc *arc2;
    s32 frame;
    s32 i;
    s32 t;
    s32 *p;
    s32 *tab;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0dfWork83);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0dfArc);
        ((EffInit *)arg4)->unk14 = (s32)func_801E9600;
        ((EffInit *)arg4)->unk4 = 7;
    } else if (mode == 1) {
        D_801E9E90 = env;
        t = *arg4;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        work->unk8 = D_801EA790[0];
        p = work->unk8;
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0x100);
            *p++ = ex->btlfx_randCentered(0x180);
            *p++ = ex->btlfx_randCentered(0xC0);
        }
        work->unk10 = t;
        work->unkC = 0;
    } else {
        frame = entry->unk0;
        if (work->unk10 == 0) {
            if (frame >= 0x29) {
                return 1;
            }
            work->unkC++;
            arc = ex->btlfx_poolSpawn((s16 *)entry->unk10);
            if (arc != NULL) {
                ((s32 *)&arc->unk0)[0] = ((s32 *)&D_801EA350[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->unk0)[1] = ((s32 *)&D_801EA350[ex->btlfx_rand() % 24])[1];
                ((s32 *)&arc->unk8)[0] = ((s32 *)&D_801EA350[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->unk8)[1] = ((s32 *)&D_801EA350[ex->btlfx_rand() % 24])[1];
                arc->unk14 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                arc->unk10 = work->unkC % 7;
            }
        } else {
            if (frame < 0x10) {
                t = ex->btlfx_randCentered(0x300);
                work->unkC++;
                t += frame * 0x334;
                arc2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (arc2 != NULL) {
                    ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&v);
                    BTLFX_COPY_SVEC(&arc2->unk0, &v);
                    v.vz += ex->math_cos(t) / 2;
                    v.vy += ex->math_sin(t) / 2;
                    BTLFX_COPY_SVEC(&arc2->unk8, &v);
                    arc2->unk14 = work->unk8 + (ex->btlfx_rand() & 0x1F);
                    arc2->unk10 = work->unkC % 7;
                }
            }
            if (frame >= 0x10) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_0df_op84(s32 mode, Fx0dfWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            D_801E9E90 = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->unk0;
            ex->func_800E7D3C(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL, b, 0, 1, a / 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk8, &rot, c * 3 / 2, 0, 1, a * 2 / 3);
            env->unk28 = 0;
            work->unk4++;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_headerPtr);

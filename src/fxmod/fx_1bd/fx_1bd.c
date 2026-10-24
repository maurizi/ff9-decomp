#include "fxmod/fx_1bd.h"

void func_801E77C0(Fx1bdPath *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801E8E90(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

s32 fx_1bd_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E9E54 = env;
        ex->btlfx_initEffModels(D_801E9E64, 0x20);
        func_801E90CC();
        func_801E9118(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x15));
        func_801E9118(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x14));
        return 0;
    } else {
        frame = entry->unk0;
        func_801E9174(0);
        if (frame >= 0x5A) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1bd_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E9E54 = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_headerPtr);

s32 fx_1bd_op82(s32 mode, Fx1bdWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    CVECTOR col;
    DrMode2Words *dm;
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        work->unk4.vx = 0;
        work->unk4.vy = 0;
        work->unk4.vz = 0;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0x320;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        D_801E9E54 = env;
        work->unk66 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0x117, 0, 0, 0);
        ex->Hi_SetEffModelOffset(work->unk66, 0x6E);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0x320;
        work->unk44.vx = 0x1000;
        work->unk44.vy = 0x1000;
        work->unk44.vz = 0x1000;
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x2B), 0x17, 0x3D00, 0x80, 0xE0);
        D_801EB764.unk10 = 0x60;
        D_801EB764.unk8 = 0;
        D_801EB764.unkA = 0;
        D_801EB764.unkC = 0x1000;
        D_801EB764.unkE = 0x1000;
        D_801EB764.unk12 = 0x30;
        work->unk60 = -1;
        work->unk62 = -1;
        work->unk64 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x80, 0xA0, D_801EB854, 0, 0x168, 0xF6E);
            break;
        case 22:
            work->unk58.vx = -3;
            work->unk58.vy = 0;
            work->unk58.vz = 0x320;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk58);
            break;
        case 21:
            work->unk60 = 4;
            break;
        case 15:
            work->unk62 = 6;
            break;
        case 46:
            ex->Hi_FreeEffModel(work->unk66);
            break;
        case 33:
            ex->Hi_FreeEffModel(work->unk68);
            break;
        }
        if (frame >= 0 && frame < 0x2E) {
            ex->Hi_DrawEffModel(&work->unk4, &work->unkC, (s32)&work->unk1C, work->unk66);
            ex->Hi_ModifyEffModelRGB(work->unk66, 6, 6, 6);
        }
        if (frame >= 21 && frame < 31) {
            D_801EB764.unkA = (frame - 21) * 2;
            D_801EB764.unkA = D_801EB764.unkA >= 10 ? 9 : D_801EB764.unkA;
            if (work->unk60 >= 0) {
                D_801EB764.unkC = func_801E8DA0(0, 0x1000, 4 - work->unk60, 4);
                work->unk60--;
            }
            if (work->unk62 >= 0) {
                D_801EB764.unkC = func_801E8DA0(0x1000, 0, 6 - work->unk62, 6);
                work->unk62--;
            }
            for (i = 0; i < 13; i++) {
                func_801E8E78(&D_801EB764.unk0, func_801E901C((u16 *)ex->btlfx_getFileData(entry->unk8, 0x16), i));
                func_801E77C0(&D_801EB764.unk0, D_801EBC54[i], 0x50, D_801EB764.unk8, D_801EB764.unkA, D_801EB764.unk10,
                              D_801EB764.unk12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), 0, NULL, 0, D_801EB764.unkE,
                                    D_801EB764.unkC, 1, D_801EBC54[i], 0);
            }
        }
        if (frame >= 26 && frame < 30) {
            t = frame - 26;
            dm = (DrMode2Words *)env->prim;
            env->prim += sizeof(DrMode2Words);
            setlen(dm, 2);
            dm->code[0] = 0xE6000001;
            dm->code[1] = 0;
            addPrim(&env->ot[1], dm);
            scale.vx = (t << 7) + 0x1000;
            scale.vy = (t << 7) + 0x1000;
            scale.vz = (t << 7) + 0x1000;
            col.r = col.g = col.b = 0x3C;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
            env->camera->projDist = 0x208 - t * t * 4;
        }
        work->unk64++;
        return frame >= 0x32;
    }
    return 0;
}

s32 fx_1bd_op83(s32 mode, Fx1bdWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR d;
    Fx1bdRock *r;
    s16 *tex;
    s16 *clut;
    u16 *a;
    u16 *b;
    s32 frame;
    s32 i;
    s32 j;
    s32 t;
    s32 dist;

    if (mode == 0) {
        *arg4 = 0x150;
    } else if (mode == 1) {
        work->unkC.vx = 0x400;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        work->unkC.pad = 1;
        work->unk14.vx = 0;
        work->unk14.vy = -0x78;
        work->unk14.vz = 0x320;
        work->unk1C = 0x1800;
        work->unk20.unk0.vx = 0;
        work->unk20.unk0.vy = 0;
        work->unk20.unk0.vz = 0;
        work->unk20.unk8.vx = 0;
        work->unk20.unk8.vy = 0;
        work->unk20.unk8.vz = 0x320;
        work->unk20.unk18.vx = 0x1000;
        work->unk20.unk18.vy = 0x1000;
        work->unk20.unk18.vz = 0x1000;
        D_801E9E54 = env;
        work->unk118[0] = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->unk8, 0x2C), D_801E9DC4,
                                                                  D_801E9DD0);
        ex->Hi_SetEffModelOffset(work->unk118[0], 0x6E);
        work->unk48.unk0.vx = 0;
        work->unk48.unk0.vy = 0;
        work->unk48.unk0.vz = 0;
        work->unk48.unk8.vx = 0;
        work->unk48.unk8.vy = 0;
        work->unk48.unk8.vz = 0x320;
        work->unk48.unk18.vx = 0x1000;
        work->unk48.unk18.vy = 0x1000;
        work->unk48.unk18.vz = 0x1000;
        for (j = 0; j < 23; j++) {
            if (D_801E9DAC[j]) {
                tex = D_801E9DE4;
                clut = D_801E9DF4;
            } else {
                tex = D_801E9DDC;
                clut = D_801E9DEC;
            }
            a = func_801E901C((u16 *)ex->btlfx_getFileData(entry->unk8, 0x29), j);
            b = func_801E901C((u16 *)ex->btlfx_getFileData(entry->unk8, 0x28), j);
            work->unk118[j + 1] = ((s32(*)())ex->Hi_RegisterTexListModel)(a, tex, clut);
            ex->Hi_SetEffModelOffset(work->unk118[j + 1], 0x6E);
            func_801E8EF4(&D_801EB784[j], b);
        }
        work->unk70.unk0.vx = 0;
        work->unk70.unk0.vy = 0;
        work->unk70.unk0.vz = 0;
        work->unk70.unk8.vx = 0;
        work->unk70.unk8.vy = 0;
        work->unk70.unk8.vz = 0x320;
        work->unk70.unk18.vx = 0x1000;
        work->unk70.unk18.vy = 0x1000;
        work->unk70.unk18.vz = 0x1000;
        func_801E8FA4((Fx1bdTrack12 *)&D_801EB77C, (u16 *)ex->btlfx_getFileData(entry->unk8, 6));
        work->unk148 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->unk8, 0x13), D_801E9DFC,
                                                               D_801E9E04);
        work->unk9C.unk0.vx = 0;
        work->unk9C.unk0.vy = 0;
        work->unk9C.unk0.vz = 0;
        work->unk9C.unk8.vx = 0;
        work->unk9C.unk8.vy = 0;
        work->unk9C.unk8.vz = 0x320;
        work->unk9C.unk18.vx = 0x1000;
        work->unk9C.unk18.vy = 0x1000;
        work->unk9C.unk18.vz = 0x1000;
        work->unkCC.unk0.vx = 0;
        work->unkCC.unk0.vy = 0;
        work->unkCC.unk0.vz = 0;
        work->unkCC.unk8.vx = 0;
        work->unkCC.unk8.vy = 0;
        work->unkCC.unk8.vz = 0x320;
        work->unkCC.unk18.vx = 0x1000;
        work->unkCC.unk18.vy = 0x1000;
        work->unkCC.unk18.vz = 0x1000;
        work->unk14A = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x9A, 0x3DC0, 0, 0);
        work->unk14C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x9A, 0x3DC0, 0, 0);
        func_801E8DF8((Fx1bdTrack12 *)&D_801EB83C, (u16 *)ex->btlfx_getFileData(entry->unk8, 8));
        func_801E8FA4((Fx1bdTrack12 *)&D_801EB844, (u16 *)ex->btlfx_getFileData(entry->unk8, 9));
        func_801E8FA4((Fx1bdTrack12 *)&D_801EB84C, (u16 *)ex->btlfx_getFileData(entry->unk8, 7));
        work->unk10C = 0;
        work->unk104.vx = work->unk14.vx;
        work->unk104.vy = work->unk14.vy;
        work->unk104.vz = work->unk14.vz;
        work->unk10E = -1;
        work->unk110 = -1;
        work->unk112 = -1;
        work->unk114 = -1;
        work->unk116 = -1;
        work->unk104.vy -= 0xF0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unkF4.vx = work->unk20.unk8.vx;
            work->unkF4.vy = work->unk20.unk8.vy;
            work->unkF4.vz = work->unk20.unk8.vz;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unkF4);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unkF4);
            break;
        case 3:
            work->unkCC.unk8.vx = work->unk20.unk8.vx;
            work->unkCC.unk8.vy = work->unk20.unk8.vy;
            work->unkCC.unk8.vz = work->unk20.unk8.vz;
            work->unkFC.vx = work->unk20.unk8.vx;
            work->unkFC.vy = work->unk20.unk8.vy;
            work->unkFC.vz = work->unk20.unk8.vz;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unkFC);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = 0x100;
            }
            break;
        case 4:
            work->unk110 = 3;
            work->unk114 = 6;
            break;
        case 21:
            work->unk112 = 0x10;
            work->unk116 = 0x10;
            work->unk10E = 0x10;
            break;
        }
        if (frame >= 6 && frame < 20) {
            ex->Hi_DrawEffModel(&work->unk20.unk0, &work->unk20.unk8, (s32)&work->unk20.unk18, work->unk118[0]);
            ex->Hi_ModifyEffModelRGB(work->unk118[0], 6, 6, 6);
            for (i = 0, r = D_801E9A14; i < 23; i++, r++) {
                t = frame - 2;
                if (r->unk2) {
                    t = 0;
                }
                if (t == r->unk0) {
                    v.vx = r->unk18.vx;
                    v.vy = r->unk18.vy;
                    v.vz = r->unk18.vz - 400;
                    ex->btlfx_aimAngles((s16 *)&work->unk48.unk8, (s16 *)&v, (s16 *)&r->unk8);
                    dist = ex->btlfx_distance(&v, (SVECTOR *)&work->unk48.unk8);
                    r->unk6 = 200;
                    r->unk4 = dist / 30;
                    r->unk6 += ex->btlfx_rand() % 100;
                    r->unk4 += ex->btlfx_rand() % 50;
                    r->unk6 /= 2;
                    r->unk8.vx &= 0xFFF;
                    r->unk8.vy &= 0xFFF;
                    r->unk8.vz &= 0xFFF;
                    r->unk4 /= 2;
                    if (r->unk8.vy < 0x800) {
                        r->unk1 |= 1;
                    } else {
                        r->unk1 |= 2;
                    }
                    if (r->unk8.vy < 0x800) {
                        r->unk1 |= 4;
                    } else {
                        r->unk1 |= 8;
                    }
                }
                if (t >= r->unk0) {
                    v.vx = r->unk18.vx;
                    v.vy = r->unk18.vy;
                    v.vz = r->unk18.vz;
                    ex->btlfx_pointAhead((s16 *)&v, (s16 *)&r->unk8, r->unk4, (s16 *)&v);
                    r->unk18.vx = v.vx;
                    r->unk18.vy = v.vy;
                    r->unk18.vz = v.vz;
                    r->unk18.vy -= r->unk6;
                    if (t < r->unk0 + 2) {
                        if (r->unk1 & 1) {
                            r->unk10.vx += 0x40;
                        } else {
                            r->unk10.vx -= 0x40;
                        }
                        if (r->unk1 & 4) {
                            r->unk10.vz += 0x40;
                        } else {
                            r->unk10.vz -= 0x40;
                        }
                    }
                    work->unk48.unk0.vx = r->unk10.vx;
                    work->unk48.unk0.vy = r->unk10.vy;
                    work->unk48.unk0.vz = r->unk10.vz;
                    r->unk6 -= 20;
                    if (r->unk6 >= 4) {
                        r->unk6 -= t * 5;
                    }
                    if (r->unk18.vy >= 50) {
                        r->unk2 = 1;
                        continue;
                    }
                } else {
                    func_801E8F08(&D_801EB784[i], t, &work->unk48.unk0, &v);
                    v.vx = -v.vx;
                    work->unk48.unk8.vx = 0;
                    work->unk48.unk8.vy = 0;
                    work->unk48.unk8.vz = 0x320;
                    work->unk48.unk0.vz = -work->unk48.unk0.vz;
                    work->unk48.unk8.vx += v.vx;
                    work->unk48.unk8.vy += v.vy;
                    work->unk48.unk8.vz += v.vz;
                    r->unk18.vx = work->unk48.unk8.vx;
                    r->unk18.vy = work->unk48.unk8.vy;
                    r->unk18.vz = work->unk48.unk8.vz;
                    work->unk48.unk8.vx = 0;
                    work->unk48.unk8.vy = 0;
                    work->unk48.unk8.vz = 0x320;
                }
                ex->Hi_DrawEffModel(&work->unk48.unk0, &r->unk18, (s32)&work->unk48.unk18, work->unk118[i + 1]);
                ex->Hi_ModifyEffModelRGB(work->unk118[i + 1], 6, 6, 6);
            }
        }
        if (frame >= 4 && frame < 37) {
            func_801E8FB8(&D_801EB77C, frame - 4, &work->unk70.unk18);
            ex->Hi_DrawEffModel(&work->unk70.unk0, &work->unk70.unk8, (s32)&work->unk70.unk18, work->unk148);
            ex->Hi_ModifyEffModelAbr(work->unk148, 1);
            if (work->unk10E >= 0) {
                work->unk98 = func_801E8DA0(0, -0x80, 0x10 - work->unk10E, 0x10);
                ex->Hi_ModifyEffModelRGB(work->unk148, work->unk98, work->unk98, work->unk98);
                work->unk10E--;
            }
            func_801E8FB8(&D_801EB844, frame - 4, &work->unk9C.unk18);
            ex->Hi_DrawEffModel(&work->unk9C.unk0, &work->unk9C.unk8, (s32)&work->unk9C.unk18, work->unk14A);
            ex->Hi_ModifyEffModelAbr(work->unk14A, 1);
            if (work->unk110 >= 0 || work->unk112 >= 0) {
                if (work->unk110 >= 0) {
                    work->unkC4 = func_801E8DA0(-0x80, 0, 3 - work->unk110, 3);
                    work->unk110--;
                }
                if (work->unk112 >= 0) {
                    work->unkC4 = func_801E8DA0(0, -0x80, 0x10 - work->unk112, 0x10);
                    work->unk112--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk14A, work->unkC4, work->unkC4, work->unkC4);
            }
            work->unk9C.unk0.vy += 0x40;
        }
        if (frame >= 4 && frame < 37) {
            work->unkCC.unk8.vx = 0;
            work->unkCC.unk8.vy = 0;
            work->unkCC.unk8.vz = 0x320;
            func_801E8E0C(&D_801EB83C, frame - 4, &d);
            work->unkCC.unk8.vx += d.vx;
            work->unkCC.unk8.vy += d.vy;
            work->unkCC.unk8.vz += d.vz;
            func_801E8FB8(&D_801EB84C, frame - 4, &work->unkCC.unk18);
            ex->Hi_DrawEffModel(&work->unkCC.unk0, &work->unkCC.unk8, (s32)&work->unkCC.unk18, work->unk14C);
            ex->Hi_ModifyEffModelAbr(work->unk14C, 1);
            if (work->unk114 >= 0 || work->unk116 >= 0) {
                if (work->unk114 >= 0) {
                    work->unkC8 = func_801E8DA0(-0x80, 0, 6 - work->unk114, 6);
                    work->unk114--;
                }
                if (work->unk116 >= 0) {
                    work->unkC8 = func_801E8DA0(0, -0x80, 0x10 - work->unk116, 0x10);
                    work->unk116--;
                }
                ex->Hi_ModifyEffModelRGB(work->unk14C, work->unkC8, work->unkC8, work->unkC8);
            }
        }
        ex->btlfx_drawScrollMove(0x3C0 - work->unk10C * 16, 0x280, 0x100, 0x40, 0x80, 0x240, 0x100);
        work->unk10C++;
        return frame >= 0x3C;
    }
    return 0;
}

void func_801E8D5C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E54)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8DA0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8DF8(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8E0C(Fx1bdTrack6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = -k->x;
    out->vy = k->y;
    out->vz = -k->z;
    return done;
}

void func_801E8E78(Fx1bdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1bdPoint *)data;
}

s32 func_801E8E90(Fx1bdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1bdPoint *p;

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

void func_801E8EF4(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8F08(Fx1bdTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    if (rot != NULL) {
        rot->vx = k->unk6;
        rot->vy = k->unk8;
        rot->vz = k->unkA;
    }
    if (pos != NULL) {
        pos->vx = -k->unk0;
        pos->vy = k->unk2;
        pos->vz = -k->unk4;
    }
    return done;
}

void func_801E8FA4(Fx1bdTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx1bdKey12 *)data;
}

s32 func_801E8FB8(Fx1bdTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx1bdKey6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

u16 *func_801E901C(u16 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

void func_801E9034(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9E54)->btlfx_drawScreenTint((u32 *)(D_801E9E54->ot + ot), arg4, c, c, c);
}

void func_801E90CC(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EC064[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EC464[i] = 0;
    }
}

void func_801E9118(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801EC064[i].unk6 == -1) {
            D_801EC064[i].unk6 = 0;
            D_801EC064[i].unk0 = data;
            D_801EC064[i].unk4 = start;
            D_801EC064[i].unkA = dst;
            D_801EC064[i].unkC = 0;
            D_801EC064[i].unkE = c;
            D_801EC064[i].unk8 = ch;
            return;
        }
    }
}

void func_801E9174(s32 ch) {
    Unk800F78A4 *env = D_801E9E54;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801EC064[i].unk8 == ch) {
            if (D_801EC064[i].unk6 == 0 && D_801EC064[i].unk4 == D_801EC464[ch]) {
                D_801EC064[i].unk6 = *D_801EC064[i].unk0;
                D_801EC064[i].unk0++;
            }
            if (D_801EC064[i].unk6 > 0) {
                D_801EC064[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801EC064[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = D_801EC064[i].unk0[0];
                        ((SVECTOR *)env->camera)[d].vy = D_801EC064[i].unk0[1];
                        ((SVECTOR *)env->camera)[d].vz = D_801EC064[i].unk0[2];
                        if (D_801EC064[i].unkC < D_801EC064[i].unkE) {
                            D_801EC064[i].unk0 += 3;
                            D_801EC064[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EC464[ch]++;
}

void func_801E92D4(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801E9E54;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR v[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 z;
    s32 flag;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *p;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 hh;
    s32 x0;
    s32 col;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        hh = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * hh);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->func_800316D8(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&z);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        x0 = -(w * size) >> 1;
        h = hh;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = x0;
            p = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, p++) {
                v[0].vx = v[2].vx = px;
                v[0].vy = v[1].vy = py;
                v[1].vx = v[3].vx = px + size;
                v[2].vy = v[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        col = p[0];
                        c1.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        col = p[w - 1];
                        c3.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&v[0], &v[1], &v[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&flag);
                gte_ldv0xy(&v[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_bm();
                *(s32 *)&c0 = *(s32 *)&c1;
                *(s32 *)&c2 = *(s32 *)&c3;
                gte_stsxy(&prim->x3);
                if (flag != 0) {
                    ex->btlfx_addPrim(0x8000000, z, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, z, tpage);
    }
    w--;
    h--;
    x0 = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = x0;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

void func_801E9874(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E9E54;
    u32 *p = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->prim = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->ot + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + 0xFFF, p);
    }
}

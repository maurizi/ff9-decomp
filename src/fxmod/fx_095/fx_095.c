#include "fxmod/fx_095.h"

s32 func_801E7704(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx095Ctl *ctl) {

    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale;
    s32 a;
    s32 glow;
    s32 bright;
    s32 glowFrame;
    s32 abr;
    s32 t;
    s32 r;
    s32 x;
    s32 y;

    if (mode == 1) {
        part->unk4 = 0;
        return 0;
    }
    bright = glow = a = scale = 0;
    glowFrame = 0;
    abr = scale;
    switch (part->unk4) {
    case 0:
        t = frame << 9;
        scale = ((ex->math_cos(t >> 2) * 3) >> 1) + 0x1000;
        a = part->unk0 + ((0x1000 - t) >> 1);
        abr = 1;
        bright = 0x1000;
        if (frame >= 8) {
            part->unk4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        scale = 0x1000;
        a = part->unk0;
        abr = 1;
        bright = scale;
        if (frame < 5) {
            t = frame << 10;
            glow = ex->math_cos(t >> 2);
            glowFrame = frame;
        }
        if (ctl->unkC != 0) {
            part->unk4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        t = frame << 8;
        scale = 0x1000;
        ex->math_cos(t >> 2);
        abr = 0xFF;
        bright = scale;
        a = part->unk0;
        if (frame >= 16) {
            part->unk4 = 3;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 3:
        t = frame << 9;
        scale = 0x1000;
        bright = ex->math_cos(t >> 2);
        abr = 1;
        a = part->unk0;
        if (frame >= 8) {
            return 1;
        }
        break;
    }
    t = (scale * 62) >> 12;
    a -= 0x400;
    x = ctl->unk8 + ((ex->math_cos(a) * t) >> 12);
    y = ctl->unkA + ((ex->math_sin(a) * t) >> 12);
    /* One call per table: jump2 cross-jumps the identical tails, leaving only the table address in each arm. */
    if (part->unk4 < 2) {
        ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E882C[part->unk8]), x, y, 0, abr, bright);
    } else {
        ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E888C[part->unk8]), x, y, 0, abr, bright);
    }
    if (glow != 0) {
        ex->func_800D1BC4(3, (u8 *)ex->btlfx_getFileData(entry->unk8, D_801E885C[part->unk8]), x, y, 0, 1, glow);
        ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x2C), x, y, glowFrame, 1, 0x1000);
    }
    return 0;
}

s32 fx_095_op80(s32 mode, Fx095Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx095Part *part;
    s32 frame;
    s32 i;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx095Work80);
        ((EffInit *)arg4)->unk8 = sizeof(Fx095Part);
        ((EffInit *)arg4)->unk14 = (s32)func_801E7704;
        ((EffInit *)arg4)->unk4 = 0xC;
    } else if (mode == 1) {
        D_801E8904 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E890C);
        work->unk4 = 0;
        work->unk0 = 0;
        work->unkC = 0;
    } else {
        frame = entry->unk0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&D_801E890C);
        gte_rtps();
        gte_stsxy(&work->unk8);
        switch (work->unk0) {
        case 0:
            for (i = 0; i < 2; i++) {
                part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (part != NULL) {
                    part->unk8 = work->unk4;
                    part->unk0 = ((work->unk4 + 1) << 12) / 12;
                }
                work->unk4++;
            }
            if (work->unk4 >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
                work->unkC = 1;
            }
            break;
        case 2:
            if (frame >= 16) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

void func_801E7CBC(s32 n, s32 x, s32 y, u16 *buf, RECT *r) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8904);
    RECT a;
    RECT b;
    RECT c;
    s32 k = (n - 1) >> 1;
    s32 h;
    s32 h1;
    s32 i;
    u16 *p;
    u32 v;
    u32 px;
    s32 cr;
    s32 cg;
    s32 cb;
    s32 sx;
    s32 sy;

    if (n != 0) {
        n = ((n - 1) & 1) + 1;
    }
    switch (n) {
    case 0:
        sx = r->x;
        sy = r->y;
        a.x = D_801E8904->drawEnv->clip.x + (x - (r->w >> 1));
        a.y = D_801E8904->drawEnv->clip.y + (y - (r->h >> 1));
        a.w = r->w;
        a.h = r->h;
        ex->MoveImage(&a, sx, sy);
        break;
    case 1:
        h1 = (s16)(r->h / 3); /* an s32 holding the truncated quotient */
        b.x = r->x;
        b.y = r->y + k * h1;
        b.w = r->w;
        b.h = h1;
        ex->StoreImage(&b, (u32 *)buf);
    case 2:
        h = (s16)(r->h / 3);
        c.x = r->x;
        c.y = r->y + k * h;
        c.w = r->w;
        c.h = h;
        for (i = 0, p = buf; i < r->w * h; i++, p++) {
            px = *p;
            cr = (px & 0x1F) * 4;
            cg = (px >> 3) & 0x7C;
            cb = (px >> 8) & 0x7C;
            v = (cr + cg + cb) / 3 >> 2;
            if (v == 0) {
                v = 1;
            }
            v = v | 0x8000 | (v << 5) | (v << 10);
            *p = v;
        }
        ex->LoadImage(&c, (u32 *)buf);
        break;
    }
}

void func_801E7F48(s32 ot, s32 x, s32 y, s32 r, s32 c, RECT *rect) {
    BtlfxExports *ex;
    POLY_GT4 *gt4;
    POLY_FT3 *ft3;
    POLY_G4 *g4;
    POLY_F3 *f3;
    s32 i;
    s32 tpage;
    Unk800F78A4 *env;
    s32 r1;
    SVECTOR v[5];
    SVECTOR uv[5];
    CVECTOR col;
    CVECTOR col2;
    s32 a;
    s32 c2;
    s32 cu;
    s32 cv;

    env = D_801E8904;
    gt4 = (POLY_GT4 *)env->prim;
    ft3 = (POLY_FT3 *)(gt4 + 16);
    g4 = (POLY_G4 *)(ft3 + 16);
    f3 = (POLY_F3 *)(g4 + 16);
    ex = BTLFX_EXPORTS(env);
    env->prim = (u8 *)(f3 + 16);
    col.r = col.g = col.b = c;
    c *= 2;
    if (c >= 0x100) {
        c = 0xFF;
    }
    a = 0;
    col2.r = col2.g = col2.b = c;
    r1 = r - 0x18;
    tpage = getTPage(2, 1, rect->x, rect->y) << 16;
    cu = (rect->w >> 1) + (rect->x & 0x3F);
    cv = (rect->h >> 1) + (rect->y & 0xFF);
    v[4].vx = x;
    v[4].vy = y;
    for (i = 0; i < 16; i++) {
        /* Dead tests, removed after reload: their loop-weighted references rank a above the prim pointers in
         * global-alloc, so it gets $s0 as in the original. */
        if (a != 0) {
            c = 0;
        }
        if (a > 5) {
            c = 0;
        }
        if (a < 3) {
            c = 0;
        }
        if (a == 7) {
            c = 0;
        }
        if (a != 9) {
            c = 0;
        }
        if (a > 100) {
            c = 0;
        }
        v[0].vx = (ex->math_cos(a) * r1) >> 12;
        v[0].vy = (ex->math_sin(a) * r1) >> 12;
        v[2].vx = (ex->math_cos(a) * r) >> 12;
        v[2].vy = (ex->math_sin(a) * r) >> 12;
        a += 0x100;
        v[1].vx = (ex->math_cos(a) * r1) >> 12;
        v[1].vy = (ex->math_sin(a) * r1) >> 12;
        v[3].vx = (ex->math_cos(a) * r) >> 12;
        v[3].vy = (ex->math_sin(a) * r) >> 12;
        uv[4].vx = cu;
        uv[4].vy = cv;
        uv[0].vx = v[0].vx + cu;
        uv[0].vy = v[0].vy + cv;
        uv[1].vx = v[1].vx + cu;
        uv[1].vy = v[1].vy + cv;
        uv[2].vx = v[2].vx + cu;
        uv[2].vy = v[2].vy + cv;
        uv[3].vx = v[3].vx + cu;
        uv[3].vy = v[3].vy + cv;
        v[0].vx += x;
        v[0].vy += y;
        v[1].vx += x;
        v[1].vy += y;
        v[2].vx += x;
        v[2].vy += y;
        v[3].vx += x;
        v[3].vy += y;
        *(s32 *)&ft3->x0 = *(s32 *)&f3->x0 = *(s32 *)&v[4];
        *(s32 *)&ft3->x1 = *(s32 *)&f3->x1 = *(s32 *)&v[0];
        *(s32 *)&ft3->x2 = *(s32 *)&f3->x2 = *(s32 *)&v[1];
        *(s32 *)&ft3->u0 = uv[4].vx | (uv[4].vy << 8);
        *(s32 *)&ft3->u1 = uv[0].vx | (uv[0].vy << 8) | tpage;
        *(s32 *)&ft3->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&ft3->r0 = *(s32 *)&col;
        *(s32 *)&f3->r0 = *(s32 *)&col2;
        ft3->code = 0x26;
        f3->code = 0x22;
        *(s32 *)&gt4->x0 = *(s32 *)&g4->x0 = *(s32 *)&v[0];
        *(s32 *)&gt4->x1 = *(s32 *)&g4->x1 = *(s32 *)&v[2];
        *(s32 *)&gt4->x2 = *(s32 *)&g4->x2 = *(s32 *)&v[1];
        *(s32 *)&gt4->x3 = *(s32 *)&g4->x3 = *(s32 *)&v[3];
        *(s32 *)&gt4->u0 = uv[0].vx | (uv[0].vy << 8);
        *(s32 *)&gt4->u1 = uv[2].vx | (uv[2].vy << 8) | tpage;
        *(s32 *)&gt4->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&gt4->u3 = uv[3].vx | (uv[3].vy << 8);
        *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&col;
        *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&col2;
        *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = 0;
        *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = 0;
        gt4->code = 0x3E;
        g4->code = 0x3A;
        if ((u32)ot < 0x1000) {
            addPrimLenIn(&env->ot[ot], gt4, 0x0C000000);
            addPrimLenIn(&env->ot[ot], ft3, 0x07000000);
            ex->btlfx_linkPrimAbr(0x08000000, (u32 *)&env->ot[ot], g4, 2);
            ex->btlfx_linkPrimAbr(0x04000000, (u32 *)&env->ot[ot], f3, 2);
        }
        ft3++;
        gt4++;
        f3++;
        g4++;
    }
}

s32 fx_095_op81(s32 mode, Fx095Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT rect;
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = sizeof(Fx095Work81);
    } else if (mode == 1) {
        D_801E8904 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        frame = entry->unk0;
        rect.x = 0x1C0;
        rect.y = 0x100;
        rect.w = 0xC0;
        rect.h = 0xC0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&D_801E890C);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->unk0) {
        case 0:
            func_801E7CBC(frame, xy.vx, xy.vy, work->unk8, &rect);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0x9B, 0xFF, 0);
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0xFF);
            }
            func_801E7F48(8, xy.vx, xy.vy, 0x60, 0x80, &rect);
            ex->func_800D1BC4(7, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), xy.vx, xy.vy, 0, 1, 0x1000);
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            c = 0x1000 - (frame << 9);
            func_801E7F48(8, xy.vx, xy.vy, 0x60, c >> 5, &rect);
            ex->func_800D1BC4(7, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), xy.vx, xy.vy, 0, 1, c);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_095/nonmatchings/fx_095", fx_095_headerPtr);

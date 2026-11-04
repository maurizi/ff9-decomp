#include "fxmod/fx_095.h"

static s32 fx_095_op80PoolTask(s32 mode, Fx095Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool, Fx095Ctl *ctl) {

    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale;
    s32 ang;
    s32 glow;
    s32 bright;
    s32 glowFrame;
    s32 abr;
    s32 t;
    s32 unused;
    s32 x;
    s32 y;

    if (mode == 1) {
        part->state4 = 0;
        return 0;
    }
    bright = glow = ang = scale = 0;
    glowFrame = 0;
    abr = scale;
    switch (part->state4) {
    case 0:
        t = frame << 9;
        scale = ((ex->math_cos(t >> 2) * 3) >> 1) + 0x1000;
        ang = part->angle0 + ((0x1000 - t) >> 1);
        abr = 1;
        bright = 0x1000;
        if (frame >= 8) {
            part->state4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        scale = 0x1000;
        ang = part->angle0;
        abr = 1;
        bright = scale;
        if (frame < 5) {
            t = frame << 10;
            glow = ex->math_cos(t >> 2);
            glowFrame = frame;
        }
        if (ctl->releaseC != 0) {
            part->state4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        t = frame << 8;
        scale = 0x1000;
        ex->math_cos(t >> 2);
        abr = 0xFF;
        bright = scale;
        ang = part->angle0;
        if (frame >= 16) {
            part->state4 = 3;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 3:
        t = frame << 9;
        scale = 0x1000;
        bright = ex->math_cos(t >> 2);
        abr = 1;
        ang = part->angle0;
        if (frame >= 8) {
            return 1;
        }
        break;
    }
    t = (scale * 62) >> 12;
    ang -= 0x400;
    x = ctl->screenX8 + ((ex->math_cos(ang) * t) >> 12);
    y = ctl->screenYA + ((ex->math_sin(ang) * t) >> 12);
    /* One call per table: jump2 cross-jumps the identical tails, leaving only the table address in each arm. */
    if (part->state4 < 2) {
        ex->btlfx_drawSpriteAtOt(3, (u8 *)ex->btlfx_getFileData(entry->file, fx_095_markBlocks[part->index8]), x, y, 0, abr, bright);
    } else {
        ex->btlfx_drawSpriteAtOt(3, (u8 *)ex->btlfx_getFileData(entry->file, fx_095_markBlocks2[part->index8]), x, y, 0, abr, bright);
    }
    if (glow != 0) {
        ex->btlfx_drawSpriteAtOt(3, (u8 *)ex->btlfx_getFileData(entry->file, fx_095_glowBlocks[part->index8]), x, y, 0, 1, glow);
        ex->btlfx_drawSpriteAtOt(2, (u8 *)ex->btlfx_getFileData(entry->file, 0x2C), x, y, glowFrame, 1, 0x1000);
    }
    return 0;
}

s32 fx_095_op80(s32 mode, Fx095Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx095Part *part;
    s32 frame;
    s32 i;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx095Work80);
        ((EffInit *)args)->elemSize = sizeof(Fx095Part);
        ((EffInit *)args)->elemFn = (s32)fx_095_op80PoolTask;
        ((EffInit *)args)->poolCount = 0xC;
    } else if (mode == 1) {
        fx_095_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_095_bodyPos);
        work->spawnCount4 = 0;
        work->state0 = 0;
        work->releaseC = 0;
    } else {
        frame = entry->frame;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&fx_095_bodyPos);
        gte_rtps();
        gte_stsxy(&work->screenXY8);
        switch (work->state0) {
        case 0:
            for (i = 0; i < 2; i++) {
                part = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (part != NULL) {
                    part->index8 = work->spawnCount4;
                    part->angle0 = ((work->spawnCount4 + 1) << 12) / 12;
                }
                work->spawnCount4++;
            }
            if (work->spawnCount4 >= 12) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame >= 16) {
                work->state0 = 2;
                entry->frame = -1;
                work->releaseC = 1;
            }
            break;
        case 2:
            if (frame >= 16) {
                work->state0 = 3;
                entry->frame = -1;
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

void fx_095_greyScreenCopy(s32 step, s32 x, s32 y, u16 *buf, RECT *area) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_095_env);
    RECT moveRect;
    RECT storeRect;
    RECT loadRect;
    s32 band = (step - 1) >> 1;
    s32 loadH;
    s32 storeH;
    s32 i;
    u16 *pix;
    u32 grey;
    u32 px;
    s32 cr;
    s32 cg;
    s32 cb;
    s32 sx;
    s32 sy;

    if (step != 0) {
        step = ((step - 1) & 1) + 1;
    }
    switch (step) {
    case 0:
        sx = area->x;
        sy = area->y;
        moveRect.x = fx_095_env->drawEnv->clip.x + (x - (area->w >> 1));
        moveRect.y = fx_095_env->drawEnv->clip.y + (y - (area->h >> 1));
        moveRect.w = area->w;
        moveRect.h = area->h;
        ex->MoveImage(&moveRect, sx, sy);
        break;
    case 1:
        storeH = (s16)(area->h / 3); /* an s32 holding the truncated quotient */
        storeRect.x = area->x;
        storeRect.y = area->y + band * storeH;
        storeRect.w = area->w;
        storeRect.h = storeH;
        ex->StoreImage(&storeRect, (u32 *)buf);
    case 2:
        loadH = (s16)(area->h / 3);
        loadRect.x = area->x;
        loadRect.y = area->y + band * loadH;
        loadRect.w = area->w;
        loadRect.h = loadH;
        for (i = 0, pix = buf; i < area->w * loadH; i++, pix++) {
            px = *pix;
            cr = (px & 0x1F) * 4;
            cg = (px >> 3) & 0x7C;
            cb = (px >> 8) & 0x7C;
            grey = (cr + cg + cb) / 3 >> 2;
            if (grey == 0) {
                grey = 1;
            }
            grey = grey | RGB15_STP | (grey << 5) | (grey << 10);
            *pix = grey;
        }
        ex->LoadImage(&loadRect, (u32 *)buf);
        break;
    }
}

void fx_095_drawTexDisc(s32 ot, s32 x, s32 y, s32 outerR, s32 bright, RECT *rect) {
    BtlfxExports *ex;
    POLY_GT4 *gt4;
    POLY_FT3 *ft3;
    POLY_G4 *g4;
    POLY_F3 *f3;
    s32 i;
    s32 tpage;
    Unk800F78A4 *env;
    s32 innerR;
    SVECTOR verts[5];
    SVECTOR uv[5];
    CVECTOR col;
    CVECTOR col2;
    s32 ang;
    s32 unused;
    s32 cu;
    s32 cv;

    env = fx_095_env;
    gt4 = (POLY_GT4 *)env->prim;
    ft3 = (POLY_FT3 *)(gt4 + 16);
    g4 = (POLY_G4 *)(ft3 + 16);
    f3 = (POLY_F3 *)(g4 + 16);
    ex = BTLFX_EXPORTS(env);
    env->prim = (u8 *)(f3 + 16);
    col.r = col.g = col.b = bright;
    bright *= 2;
    if (bright >= 0x100) {
        bright = 0xFF;
    }
    ang = 0;
    col2.r = col2.g = col2.b = bright;
    innerR = outerR - 0x18;
    tpage = getTPage(2, 1, rect->x, rect->y) << 16;
    cu = (rect->w >> 1) + (rect->x & 0x3F);
    cv = (rect->h >> 1) + (rect->y & 0xFF);
    verts[4].vx = x;
    verts[4].vy = y;
    for (i = 0; i < 16; i++) {
        /* Dead tests, removed after reload: their loop-weighted references rank ang above the prim pointers in
         * global-alloc, so it gets $s0 as in the original. */
        if (ang != 0) {
            bright = 0;
        }
        if (ang > 5) {
            bright = 0;
        }
        if (ang < 3) {
            bright = 0;
        }
        if (ang == 7) {
            bright = 0;
        }
        if (ang != 9) {
            bright = 0;
        }
        if (ang > 100) {
            bright = 0;
        }
        verts[0].vx = (ex->math_cos(ang) * innerR) >> 12;
        verts[0].vy = (ex->math_sin(ang) * innerR) >> 12;
        verts[2].vx = (ex->math_cos(ang) * outerR) >> 12;
        verts[2].vy = (ex->math_sin(ang) * outerR) >> 12;
        ang += 0x100;
        verts[1].vx = (ex->math_cos(ang) * innerR) >> 12;
        verts[1].vy = (ex->math_sin(ang) * innerR) >> 12;
        verts[3].vx = (ex->math_cos(ang) * outerR) >> 12;
        verts[3].vy = (ex->math_sin(ang) * outerR) >> 12;
        uv[4].vx = cu;
        uv[4].vy = cv;
        uv[0].vx = verts[0].vx + cu;
        uv[0].vy = verts[0].vy + cv;
        uv[1].vx = verts[1].vx + cu;
        uv[1].vy = verts[1].vy + cv;
        uv[2].vx = verts[2].vx + cu;
        uv[2].vy = verts[2].vy + cv;
        uv[3].vx = verts[3].vx + cu;
        uv[3].vy = verts[3].vy + cv;
        verts[0].vx += x;
        verts[0].vy += y;
        verts[1].vx += x;
        verts[1].vy += y;
        verts[2].vx += x;
        verts[2].vy += y;
        verts[3].vx += x;
        verts[3].vy += y;
        *(s32 *)&ft3->x0 = *(s32 *)&f3->x0 = *(s32 *)&verts[4];
        *(s32 *)&ft3->x1 = *(s32 *)&f3->x1 = *(s32 *)&verts[0];
        *(s32 *)&ft3->x2 = *(s32 *)&f3->x2 = *(s32 *)&verts[1];
        *(s32 *)&ft3->u0 = uv[4].vx | (uv[4].vy << 8);
        *(s32 *)&ft3->u1 = uv[0].vx | (uv[0].vy << 8) | tpage;
        *(s32 *)&ft3->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&ft3->r0 = *(s32 *)&col;
        *(s32 *)&f3->r0 = *(s32 *)&col2;
        ft3->code = GPU_POLY_FT3 | GPU_ABE;
        f3->code = GPU_POLY_F3 | GPU_ABE;
        *(s32 *)&gt4->x0 = *(s32 *)&g4->x0 = *(s32 *)&verts[0];
        *(s32 *)&gt4->x1 = *(s32 *)&g4->x1 = *(s32 *)&verts[2];
        *(s32 *)&gt4->x2 = *(s32 *)&g4->x2 = *(s32 *)&verts[1];
        *(s32 *)&gt4->x3 = *(s32 *)&g4->x3 = *(s32 *)&verts[3];
        *(s32 *)&gt4->u0 = uv[0].vx | (uv[0].vy << 8);
        *(s32 *)&gt4->u1 = uv[2].vx | (uv[2].vy << 8) | tpage;
        *(s32 *)&gt4->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&gt4->u3 = uv[3].vx | (uv[3].vy << 8);
        *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&col;
        *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&col2;
        *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = 0;
        *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = 0;
        gt4->code = GPU_POLY_GT4 | GPU_ABE;
        g4->code = GPU_POLY_G4 | GPU_ABE;
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

s32 fx_095_op81(s32 mode, Fx095Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT rect;
    s32 frame;
    s32 fade;

    if (mode == 0) {
        *args = sizeof(Fx095Work81);
    } else if (mode == 1) {
        fx_095_env = env;
        work->state0 = 0;
        work->unk4 = 0;
    } else {
        frame = entry->frame;
        rect.x = VRAM_BTLFX_SCRATCH_X;
        rect.y = VRAM_BTLFX_SCRATCH_Y;
        rect.w = 0xC0;
        rect.h = 0xC0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&fx_095_bodyPos);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->state0) {
        case 0:
            fx_095_greyScreenCopy(frame, xy.vx, xy.vy, work->pixels8, &rect);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0x9B, 0xFF, 0);
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0xFF);
            }
            fx_095_drawTexDisc(8, xy.vx, xy.vy, 0x60, 0x80, &rect);
            ex->btlfx_drawSpriteAtOt(7, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), xy.vx, xy.vy, 0, 1, 0x1000);
            if (frame >= 16) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            fade = 0x1000 - (frame << 9);
            fx_095_drawTexDisc(8, xy.vx, xy.vy, 0x60, fade >> 5, &rect);
            ex->btlfx_drawSpriteAtOt(7, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), xy.vx, xy.vy, 0, 1, fade);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_095/nonmatchings/fx_095", fx_095_headerPtr);

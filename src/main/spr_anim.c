#include "ff9.h"

void sprAnim_drawCell(s32 x, s32 y, s32 otz, s32 unused, SprCell *cell) {
    u32 clutTp = cell->clutTpage;
    SprtWords *sprt;

    setlen(g_primPtr, PRIM_LEN(SPRT));
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = ((x + cell->x - 0x80) & 0xFFFF) | ((y + cell->y - 0x80) << 16);
    sprt->uvclut = cell->u | (cell->v << 8) | ((u16)cell->clutTpage << 16);
    g_primPtr = (u8 *)(sprt + 1);
    sprt->wh = cell->w | (cell->h << 16);
    addPrim(&g_ot[otz], sprt);
    SetDrawTPage(g_primPtr, 1, 1, SPRCELL_TPAGE(clutTp));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void sprAnim_drawScaledCell(s32 x, s32 y, s32 otz, SprAnim *unused, AnimCell *animCell) {
    u32 clutTp;
    s32 sw;
    u32 tp;
    s32 sh;
    PolyFT4Words *poly;

    clutTp = animCell->cell.clutTpage;
    sw = (animCell->cell.w * animCell->scaleX) >> 12;
    tp = SPRCELL_TPAGE(clutTp);
    sh = (animCell->cell.h * animCell->scaleY) >> 12;
    x = x + animCell->cell.x - 0x80 + ((animCell->cell.w - sw) >> 1);
    y = y + animCell->cell.y - 0x80 + ((animCell->cell.h - sh) >> 1);
    setlen(g_primPtr, PRIM_LEN(POLY_FT4));
    poly = (PolyFT4Words *)g_primPtr;
    poly->rgbc = GPU_RGBC(GPU_POLY_FT4, GPU_RGB_NEUTRAL);
    poly->xy0 = (x & 0xFFFF) | (y << 16);
    poly->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    poly->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    poly->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    poly->uv0clut = animCell->cell.u | (animCell->cell.v << 8) | ((u16)animCell->cell.clutTpage << 16);
    poly->uv1tpage = (animCell->cell.u + animCell->cell.w - 1) | (animCell->cell.v << 8) | (tp << 16);
    poly->uv2 = animCell->cell.u | ((animCell->cell.v + animCell->cell.h - 1) << 8);
    poly->uv3 = (animCell->cell.u + animCell->cell.w - 1) | ((animCell->cell.v + animCell->cell.h - 1) << 8);
    addPrim(&g_ot[otz], poly);
    g_primPtr = (u8 *)(poly + 1);
}

void sprAnim_init(SprAnim *anim, SprAnimData *data) {
    anim->data = data;
    anim->frames = data->frames;
    anim->cellLists = &anim->frames[anim->data->frameCount];
    anim->base = data;
    anim->playing = 0;
    anim->frame = 0;
    anim->tick = 0;
    anim->onDone = 0;
    anim->frameCount = data->frameCount;
}

u8 sprAnim_setPlaying(SprAnim *anim, u8 playing) {
    u8 old = anim->playing;
    anim->playing = playing;
    return old;
}

void sprAnim_setOnDone(SprAnim *anim, void (*fn)()) {
    anim->onDone = fn;
}

void sprAnim_reset(SprAnim *anim) {
    anim->playing = 0;
    anim->frame = 0;
    anim->tick = 0;
    anim->onDone = 0;
}

void sprAnim_update(SprAnim *anim) {
    SprAnimFrame *frameDef = &anim->frames[anim->frame];

    if (anim->playing != 0) {
        if (++anim->tick >= frameDef->ticks - 1) {
            anim->tick = 0;
            if (++anim->frame >= anim->frameCount - 1) {
                anim->playing = 0;
                if (anim->onDone != NULL) {
                    anim->onDone(anim);
                }
            }
        }
    }
}

void sprAnim_draw(s32 x, s32 y, s32 otz, SprAnim *anim) {
    s32 i;
    SprAnimFrame *frameDef = &anim->frames[anim->frame];
    AnimCellList *cl = &((AnimCellList *)anim->cellLists)[frameDef->cellList];
    AnimCell *animCell = (AnimCell *)((u8 *)anim->base + cl->offset);

    for (i = 0; i < cl->count; i++) {
        if (!SPRCELL_SCALED(animCell->cell.clutTpage)) {
            sprAnim_drawCell(x + frameDef->x, y + frameDef->y, otz, (s32)anim, &animCell->cell);
        } else {
            sprAnim_drawScaledCell(x + frameDef->x, y + frameDef->y, otz, anim, animCell);
        }
        animCell++;
    }
}

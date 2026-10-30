#include "ovl_0b.h"

void btl_spawnPopups(BtlUnit *arg0) {
    u16 f = arg0->popupFlags;
    u8 n = 0;
    if (!((arg0->flags >> 22) & 1)) {
        if (f & 0x100) {
            btl_spreadTroubleDamage(arg0);
        }
        if (f & 0x80) {
            btl_popupText((s32)arg0, 2, 0, 0);
            arg0->popupFlags = 0;
        } else if (f & 0x60) {
            if (f & 0x20) {
                btl_popupText((s32)arg0, 0, 0, 0);
                n = 2;
            }
            if (f & 0x40) {
                btl_popupText((s32)arg0, 1, 0, n);
            }
            arg0->popupFlags = 0;
        } else {
            if (f & 1) {
                if (f & 4) {
                    btl_popupText((s32)arg0, 3, 0x80, 0);
                    n = 2;
                }
                if (f & 2) {
                    btl_popupHp((s32)arg0, arg0->hpChange, 0xC0, n);
                } else {
                    btl_popupHp((s32)arg0, arg0->hpChange, 0, n);
                }
                n += 4;
            }
            if (f & 8) {
                if (f & 0x10) {
                    btl_popupMp((s32)arg0, arg0->mpChange, 0xC0, n);
                } else {
                    btl_popupMp((s32)arg0, arg0->mpChange, 0, n);
                }
            }
            arg0->popupFlags = 0;
        }
    } else {
        arg0->popupFlags = 0;
    }
    arg0->hpChange = 0;
    arg0->mpChange = 0;
}

void btl_spawnTickPopups(BtlUnit *arg0) {
    u8 f = arg0->tickFlags;
    u8 n = 0;
    if (!((arg0->flags >> 22) & 1)) {
        if (f & 1) {
            btl_popupHp((s32)arg0, arg0->tickHeal, (f & 8) ? 0 : 0xC0, 0)->info.b.noClamp = 1;
            n = 4;
        }
        if (f & 2) {
            btl_popupHp((s32)arg0, arg0->tickDamage, 0, n)->info.b.noClamp = 1;
            n += 4;
        }
        if (f & 4) {
            btl_popupMp((s32)arg0, arg0->tickMpDamage, 0, n)->info.b.noClamp = 1;
        }
    }
    arg0->tickFlags = 0;
    arg0->tickHeal = 0;
    arg0->tickDamage = 0;
    arg0->tickMpDamage = 0;
}

BtlPopup *btl_newPopup(BtlUnit *u) {
    BtlPopupRing *r;
    BtlPopup *p;
    BtlBone *bone;
    s16 i;

    r = &g_gameState->gameData->ctx.battle->popups;
    i = r->last - 1;
    if (i < 0) {
        i = 15;
    }
    r->last = i;
    p = &r->entries[i];
    p->unit = u;
    p->frame = 0;
    p->info.w = 0;
    bone = &u->model->bones[u->baseBone];
    p->x = bone->tx;
    p->y = bone->ty - 4;
    p->z = bone->tz;
    return p;
}

BtlPopup *btl_popupHp(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    BtlPopup *p = btl_newPopup((BtlUnit *)arg0);
    p->info.b.kind = 0;
    p->info.b.delay = arg3;
    p->clut = arg2;
    p->data.w = btl_packDigits(arg1);
    return p;
}

BtlPopup *btl_popupMp(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    BtlPopup *p = btl_newPopup((BtlUnit *)arg0);
    p->info.b.kind = 1;
    p->info.b.delay = arg3;
    p->clut = arg2;
    p->data.w = btl_packDigits(arg1);
    return p;
}

BtlPopup *btl_popupText(arg0, arg1, arg2, arg3)
    s32 arg0;
    u8 arg1;
    s16 arg2;
    s8 arg3;
{
    BtlPopup *p = btl_newPopup((BtlUnit *)arg0);
    p->info.b.kind = 2;
    p->info.b.delay = arg3;
    p->data.p = &D_800F6A54[arg1];
    p->clut = arg2;
    return p;
}

/* Packs n (< 9999) as decimal digits, one per nibble, with the digit count in bits 16+. */
s32 btl_packDigits(u32 n) {
    u32 r = 0;
    u16 d;
    u16 i;
    u32 t;
    do {
        if (n >= 9999) {
            return 0x49999;
        }
        for (i = 3; i != 0; i--) {
            d = 0;
            if (n >= D_800F6A74[i]) {
                d = n / D_800F6A74[i];
                n = n % D_800F6A74[i];
            }
            r = (r << 4) | d;
        }
        r = (r << 4) | n;
    } while (0);
    for (i = 1, t = r >> 4; t != 0; t >>= 4) {
        i++;
    }
    return r | (i << 16);
}

u8 *btl_drawPopups(u8 *arg0) {
    Unk800CCD00 c;
    BtlPopupRing *r;
    BtlPopup *p;
    s16 i;
    s16 j;
    BtlUnit *u;
    u8 mask;
    Unk800F78A40 *e;

    r = &g_gameState->gameData->ctx.battle->popups;
    c.prim = (u32 *)arg0;
    c.ot = &g_gameState->gameData->ot[g_gameState->gameData->drawBuf][4];
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    j = r->last;
    for (i = 0; i < 16; i++) {
        p = &r->entries[j];
        if (p->unit != NULL) {
            if (p->info.b.kind >= 3) {
                p->unit = NULL;
            } else if (p->info.b.delay != 0) {
                p->info.b.delay--;
            } else {
                gte_ldv0(&p->x);
                gte_rtps();
                gte_stsxy(&c.xy);
                if (p->info.b.yOfs != 0 && (u16)(c.xy.s.y - 21) < 180) {
                    p->info.b.yOfs = 0;
                }
                D_800F6A7C[p->info.b.kind](p, &c);
            }
        }
        j++;
        if (j >= 16) {
            j = 0;
        }
    }
    btl_drawStatusCountdown(&c);
    ((u8 *)c.prim)[3] = 1;
    c.prim[1] = get_mode(0, 0, D_80068BF0[0].tpage);
    addPrim(c.ot, c.prim);
    c.prim += 2;
    e = g_btlfxEnv->state;
    if (((e->flags ^ 1) & 1) | ((e->phase == -1) | ((e->flags >> 15) & 1))) {
        btl_drawStatusIcons(&c);
    }
    r->tick++;
    btl_updateScan();
    btl_updateDetect();
    mask = 0xFF;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (!((u->flags >> 22) & 1)) {
            mask &= ~u->unitBit;
        }
    }
    r->hiddenMask = mask;
    return (u8 *)c.prim;
}

void btl_drawNumberPopup(BtlPopup *arg0, Unk800CCD00 *arg1) {
    u8 *anim;
    u8 c;
    u32 n;
    u32 w;
    s16 x;

    if (arg0->clut == 0xC0) {
        anim = D_800F6A38;
    } else {
        anim = D_800F6A28;
    }
    c = anim[arg0->frame];
    if (!(arg0->unit->model->flags & 0x10)) {
        n = arg0->data.w;
        arg1->rgb = (c & 0x80) ? 0x03000000 : 0x01000000;
        arg1->clut = arg0->clut;
        w = (n >> 16) * 8;
        arg1->xy.s.x -= w >> 1;
        arg1->xy.s.y -= (c & 0x7F) + 5;
        if (arg0->info.b.noClamp == 0) {
            x = w;
            if (arg0->info.b.kind != 0) {
                x = D_80068BE8.w + w;
            }
            btl_clampPopup(arg0, arg1, x, 10);
        } else {
            arg1->xy.s.y += arg0->info.b.yOfs;
        }
        btl_drawPopupDigits(arg1, n);
        if (arg0->info.b.kind != 0) {
            arg1->xy.s.x += (n >> 16) * 8;
            arg1->xy.s.y += 3;
            btl_drawPopupGlyph(arg1, &D_80068BE8);
        }
    }
    if (anim[++arg0->frame] == 0xFF) {
        arg0->unit = NULL;
    }
}

void btl_drawTextPopup(BtlPopup *arg0, Unk800CCD00 *arg1) {
    u8 *anim;
    u8 c;
    SpriteDef *g;

    anim = arg0->data.p->anim;
    c = anim[arg0->frame];
    if (!(arg0->unit->model->flags & 0x10)) {
        arg1->rgb = (c & 0x80) ? 0x03000000 : 0x01000000;
        arg1->clut = arg0->clut;
        g = &g_uiSprites[arg0->data.p->glyph];
        arg1->xy.s.x -= g->w >> 1;
        arg1->xy.s.y -= (g->h >> 1) + (c & 0x1F);
        if (arg0->info.b.noClamp == 0) {
            btl_clampPopup(arg0, arg1, g->w, g->h);
        }
        btl_drawPopupGlyph(arg1, g);
    }
    arg0->frame++;
    if (anim[arg0->frame] == 0xFF) {
        arg0->unit = NULL;
    }
}

void btl_clampPopup(BtlPopup *arg0, Unk800CCD00 *arg1, s32 w, s32 h) {
    if (arg0->frame == 0) {
        if (arg1->xy.s.y - 21 < 0) {
            arg0->info.b.yOfs = 21 - arg1->xy.s.y;
        } else if (arg1->xy.s.y + h >= 211) {
            arg0->info.b.yOfs = -46 - (arg1->xy.s.y + h);
        }
    }
    if (arg1->xy.s.x < 8) {
        arg1->xy.s.x = 8;
    } else if (arg1->xy.s.x + w >= 313) {
        arg1->xy.s.x = 312 - w;
    }
    arg1->xy.s.y += arg0->info.b.yOfs;
    if (arg1->xy.s.y < 8) {
        arg1->xy.s.y = 8;
    } else if (arg1->xy.s.y + h >= 211) {
        arg1->xy.s.y = 210 - h;
    }
}

void btl_drawPopupDigits(Unk800CCD00 *arg0, u32 arg1) {
    DVECTOR xy;
    s16 i;
    SpriteDef *g;

    i = (arg1 >> 16) - 1;
    xy.vx = arg0->xy.s.x + (((s32)arg1 >> 16) - 1) * 8;
    xy.vy = arg0->xy.s.y;
    for (; i != -1; i--) {
        arg0->prim[0] = 0x04000000;
        arg0->prim[1] = arg0->rgb | 0x64000000;
        arg0->prim[2] = *(s32 *)&xy;
        g = &D_80068BF0[arg1 & 0xF];
        arg0->prim[3] = g->uvclut + (arg0->clut << 16);
        arg0->prim[4] = (g->h << 16) | g->w;
        addPrim(arg0->ot, arg0->prim);
        arg0->prim += 5;
        xy.vx -= 8;
        arg1 >>= 4;
    }
}

void btl_drawPopupGlyph(Unk800CCD00 *arg0, SpriteDef *arg1) {
    arg0->prim[0] = 0x04000000;
    arg0->prim[1] = arg0->rgb | 0x64000000;
    arg0->prim[2] = arg0->xy.w;
    arg0->prim[3] = arg1->uvclut + (arg0->clut << 16);
    arg0->prim[4] = (arg1->h << 16) | arg1->w;
    addPrim(arg0->ot, arg0->prim);
    arg0->prim += 5;
}

void btl_drawStatusIcons(Unk800CCD00 *arg0) {
    BtlPopupRing *r;
    BtlUnit *u;
    BtlUnit9C *mdl;
    BtlBone *b;
    Unk800F6A88 *e;
    SVECTOR pos;
    SVECTOR rot;
    u32 st;
    s16 sn;
    s16 cs;
    s16 y;
    s32 y0;
    s32 z0;
    s16 z;
    s32 k;
    s32 f;
    s16 i;
    u8 *bone;
    u8 *ty;
    u8 *tz;

    r = &g_gameState->gameData->ctx.battle->popups;
    btlfx_beginPrims((u8 *)arg0->prim, g_gameState->gameData->ot[g_gameState->gameData->drawBuf]);
    g_btlfxEnv->otZ = 0;
    rot.vx = 0;
    rot.vz = 0;
    rot.pad = 1;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (((u->flags >> 22) & 1) || (u->model->flags & 0x10)) {
            continue;
        }
        if (r->hiddenMask & u->unitBit) {
            continue;
        }
        st = u->status | u->autoStatus;
        if ((st & BTL_STATUS_KO) || !(st & (BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | BTL_STATUS_TROUBLE | BTL_STATUS_BERSERK | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_REFLECT))) {
            continue;
        }
        if ((u->flags & 1) && u->model->motion == (BtlUnit9CC *)u->motions[0x11]) {
            continue;
        }
        sn = math_sin(u->rot[1]);
        cs = math_cos(u->rot[1]);
        if (u->flags & 1) {
            bone = D_800F6B48[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind];
            ty = D_800F6BBC[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind];
            tz = D_800F6C30[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind];
        } else {
            BtlEnemyType *m = g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type;
            bone = m->iconBones;
            ty = m->iconY;
            tz = m->iconZ;
        }
        for (e = g_btlStatusIcons, i = 11; i != -1; e++, i--) {
            if (!(st & e->showMask) || (st & e->hideMask)) {
                continue;
            }
            k = e->attach;
            mdl = u->model;
            y0 = (ty[k] << 24) >> 20;
            z0 = (tz[k] << 24) >> 20;
            y = y0;
            z = z0;
            if (mdl->flags & 2) {
                y = (u32)(y0 * mdl->scaleY) >> 12;
                z = (u32)(z0 * mdl->scaleZ) >> 12;
            }
            b = &mdl->bones[bone[k]];
            pos.vx = b->tx + (((s16)z * sn) >> 12);
            pos.vz = b->tz + (((s16)z * cs) >> 12);
            pos.vy = b->ty + y;
            if (e->single != 0) {
                f = (u16)r->tick;
                btlfx_drawSprite(e->sprite, &pos, f % (*(u16 *)e->sprite & 0x7FFF), e->abr, -1);
            } else {
                if ((rot.pad = e->turnsWithUnit) != 0) {
                    rot.vy = (u->rot[1] + 0xC00) & 0xFFF;
                } else {
                    rot.vy = 0;
                }
                btlfx_drawSprites(e->sprite, &pos, &rot, -1, (u16)r->tick, e->abr, -1);
            }
        }
    }
    arg0->prim = (u32 *)btlfx_endPrims();
}

void btl_drawStatusCountdown(Unk800CCD00 *arg0) {
    BtlUnit *u;
    BtlEnemyType *m;
    BtlUnit9C *mdl;
    BtlBone *b;
    Unk800F6CA4 *e;
    SVECTOR v;
    u32 st;
    s16 bone;
    s16 y;
    s16 i;
    s16 t;
    s32 q;
    s16 d;
    u8 c;
    s32 blink;
    u32 n;

    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (((u->flags >> 22) & 1) || (u->model->flags & 0x10)) {
            continue;
        }
        if (g_gameState->gameData->ctx.battle->popups.hiddenMask & u->unitBit) {
            continue;
        }
        st = u->status;
        if ((st & BTL_STATUS_KO) || !(st & (BTL_STATUS_DOOM | BTL_STATUS_GRADUAL_PETRIFY))) {
            continue;
        }
        if (u->flags & 1) {
            bone = D_800F6B48[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind][5];
            y = (s8)D_800F6BBC[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind][5];
        } else {
            m = g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type;
            bone = m->iconBones[5];
            y = (s8)m->iconY[5];
        }
        mdl = u->model;
        if (mdl->flags & 2) {
            y = (u32)(y * mdl->scaleY) >> 12;
        }
        b = &mdl->bones[bone];
        v.vx = b->tx;
        v.vy = b->ty + (y << 4);
        v.vz = b->tz;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&arg0->xy);
        arg0->xy.s.y -= 12;
        for (e = D_800F6CA4, i = 1; i != -1; e++, i--) {
            if (st & e->status) {
                if (u->countdownStep > 0 && (t = u->statusTimers[e->timer]) >= 0) {
                    q = t * 10 / u->countdownStep;
                    d = u->atbSpeed;
                    bone = q;
                    if (t <= 0) {
                        blink = 2;
                    } else {
                        blink = ((s16)q != (s16)((t - d) * 10 / u->countdownStep)) * 2;
                    }
                    if (e->colorByLevel != 0) {
                        c = (bone << 4) + 0x20;
                        arg0->rgb = (c << 16) | (c << 8) | c;
                    } else {
                        arg0->rgb = 0x1000000;
                    }
                    arg0->clut = 0;
                    arg0->rgb |= blink << 24;
                    t = bone + 1;
                    if (t < 10) {
                        n = t | 0x10000;
                    } else {
                        n = 0x20010;
                    }
                    arg0->xy.s.x -= (n >> 16) * 4;
                    btl_drawPopupDigits(arg0, n);
                }
                break;
            }
        }
    }
}

void btl_showMessage(u32 arg0, ...) {
    u8 buf[0x50];
    va_list ap;
    TextTbl *t = (TextTbl *)D_800F6CB4;
    TextTblEnt *e;
    u8 *src;
    u8 *dst;
    u8 *special;
    s32 kind;
    u8 c;
    s32 v;

    arg0 += 8;
    if (arg0 < t->count) {
        va_start(ap, arg0);
        dst = &buf[1];
        e = t->entries;
        src = (u8 *)e + e[arg0].ofs;
        kind = *src++;
        special = (u8 *)e + t->entries[0].ofs;
        c = buf[0] = *src++;
        while (c != 0xFF) {
            if (c == special[0]) {
                dst--;
                v = va_arg(ap, s32);
                if (v != 0) {
                    text_copy(dst, (u8 *)v);
                    goto adv;
                }
            } else if (c == special[1]) {
                dst--;
                text_formatInt(va_arg(ap, s32), dst, 0);
            adv:
                dst += text_getLength(dst);
            }
            c = *dst++ = *src++;
        }
        btl_setBanner((s32)buf, kind);
    }
}

s32 btl_getElementName(u8 arg0) {
    s32 n = 0;
    while (arg0 >>= 1) {
        n++;
    }
    return text_getFrom(D_800F6CB4, n + 1);
}

u8 *btl_getItemName(u8 arg0) {
    return &g_itemNames[g_itemTable[arg0].name];
}

void *btl_getUnitName(BtlUnit *arg0) {
    u32 f = arg0->flags;
    Battle *b;
    if (!(f & 1)) {
        b = g_gameState->gameData->ctx.battle;
        return b->work->texts + b->enemies[(f >> 1) & 0xF].type->nameOfs;
    }
    return &g_gameState->gameData->characters[(f >> 1) & 0xF];
}

void btl_startScan(s32 arg0) {
    g_gameState->gameData->ctx.battle->popups.scanUnit = arg0;
    g_gameState->gameData->ctx.battle->popups.scan.w = 0;
}

void btl_updateScan(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *s = (BtlUnit *)b->popups.scanUnit;
    u8 buf[64];
    u32 k;
    u32 j;
    u16 mask;
    Battle *c;

    if (s == NULL) {
        return;
    }
    if (--b->popups.scan.b[0] != 0xFF) {
        return;
    }
    c = g_gameState->gameData->ctx.battle;
    switch (c->popups.scan.b[2]) {
    case 0:
        text_copy(buf, btl_getUnitName(s));
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 10));
        text_formatInt(s->level, buf + text_getLength(buf), 0);
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 1;
        break;
    case 1:
        text_copy(buf, (u8 *)text_getFrom(D_800F6FB8, 11));
        text_formatInt(s->hp, buf + text_getLength(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 13));
        text_formatInt(s->max->maxHp, buf + text_getLength(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 12));
        text_formatInt(s->mp, buf + text_getLength(buf), 0);
        text_append(buf, (u8 *)text_getFrom(D_800F6FB8, 13));
        text_formatInt(s->max->maxMp, buf + text_getLength(buf), 0);
        g_gameState->gameData->ctx.battle->popups.scan.b[1] = 0;
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 2;
        break;
    case 2:
        if (!(s->flags & 1)) {
            mask = c->enemies[(s->flags >> 1) & 0xF].type->category;
            while ((j = g_gameState->gameData->ctx.battle->popups.scan.b[1]++) < 8) {
                if ((mask >> j) & 1) {
                    text_copy(buf, (u8 *)text_getFrom(D_800F6FB8, j));
                    goto show;
                }
            }
        }
        g_gameState->gameData->ctx.battle->popups.scan.b[1] = 0;
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 3;
    case 3:
        mask = s->elemWeak & ~s->elemGuard;
        while ((k = g_gameState->gameData->ctx.battle->popups.scan.b[1]++) < 8) {
            if ((mask >> k) & 1) {
                text_copy(buf, (u8 *)text_getFrom(D_800F6FB8, k + 14));
                goto show;
            }
        }
    default:
        g_gameState->gameData->ctx.battle->popups.scanUnit = 0;
        btl_setBanner(0, 2);
        return;
    }
show:
    g_gameState->gameData->ctx.battle->popups.scan.b[0] = func_800F4FC8() - 1;
    btl_setBanner((s32)buf, 2);
}

void btl_startDetect(BtlUnit *arg0) {
    if (!(arg0->flags & 1)) {
        g_gameState->gameData->ctx.battle->popups.detectEnemy = &g_gameState->gameData->ctx.battle->enemies[(arg0->flags >> 1) & 0xF];
        if (g_gameState->gameData->ctx.battle->popups.detectEnemy->steal.w == -1) {
            btl_setBanner(text_getFrom(D_800F6FB8, 9), 2);
            g_gameState->gameData->ctx.battle->popups.detectSlot = 4;
            g_gameState->gameData->ctx.battle->popups.detectDelay = func_800F4FC8() - 1;
            return;
        }
        g_gameState->gameData->ctx.battle->popups.detectSlot = 0;
        g_gameState->gameData->ctx.battle->popups.detectDelay = 0;
    }
}

void btl_updateDetect(void) {
    u8 buf[0x40];
    u8 i;
    u8 id;
    u8 *s;
    s32 kind;

    if (g_gameState->gameData->ctx.battle->popups.detectEnemy == NULL) {
        return;
    }
    if (--g_gameState->gameData->ctx.battle->popups.detectDelay != 0xFF) {
        return;
    }
    while ((i = g_gameState->gameData->ctx.battle->popups.detectSlot++) < 4) {
        id = g_gameState->gameData->ctx.battle->popups.detectEnemy->steal.b[3 - i];
        if (id != 0xFF) {
            g_gameState->gameData->ctx.battle->popups.detectDelay = func_800F4FC8() - 1;
            text_copy(buf, (u8 *)text_getFrom(D_800F6FB8, 8));
            text_append(buf, btl_getItemName(id));
            text_append(buf, D_800A7C20);
            s = buf;
            kind = 2;
            goto show;
        }
    }
    s = NULL;
    kind = 1;
    g_gameState->gameData->ctx.battle->popups.detectEnemy = NULL;
show:
    btl_setBanner((s32)s, kind);
}

void btl_showCmdName(BtlCmd *arg0) {
    u8 *s = NULL;
    u8 buf[32];
    u16 t;
    switch (arg0->cmdNo) {
    case BTL_CMD_ITEM:
    case BTL_CMD_THROW:
        s = btl_getItemName(arg0->subNo);
        break;
    case BTL_CMD_RETURN_MAGIC:
        s = g_gameState->gameData->ctx.battle->work->texts + arg0->ability->nameOfs;
        break;
    default:
        if (arg0->subNo < 0xC0) {
            t = D_800F7124[arg0->subNo];
            switch (t) {
            case 0:
                break;
            case 0xFF:
                s = btl_getAbilityName(arg0->subNo);
                break;
            case 0xFE:
                s = buf;
                text_copy(s, btl_getAbilityName(arg0->subNo));
                text_append(s, (u8 *)text_getFrom(D_800F71E4, 0));
                break;
            default:
                if (t < 0xC0) {
                    s = btl_getAbilityName(t);
                } else {
                    s = (u8 *)text_getFrom(D_800F71E4, (t & 0x3F) + 1);
                }
                break;
            }
        }
        break;
    }
    if (s != NULL) {
        btl_setBannerAt((s32)s, 1);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btl_popup", D_800A7C20);

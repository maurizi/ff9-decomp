#include "ovl_0b.h"

void btl_spawnPopups(BtlUnit *unit) {
    u16 flags = unit->popupFlags;
    u8 delay = 0;
    if (!((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
        if (flags & BTL_POPUP_TROUBLE) {
            btl_spreadTroubleDamage(unit);
        }
        if (flags & BTL_POPUP_GUARD) {
            btl_popupText((s32)unit, 2, 0, 0);
            unit->popupFlags = 0;
        } else if (flags & (BTL_POPUP_MISS | BTL_POPUP_DEATH)) {
            if (flags & BTL_POPUP_MISS) {
                btl_popupText((s32)unit, 0, 0, 0);
                delay = 2;
            }
            if (flags & BTL_POPUP_DEATH) {
                btl_popupText((s32)unit, 1, 0, delay);
            }
            unit->popupFlags = 0;
        } else {
            if (flags & BTL_POPUP_HP) {
                if (flags & BTL_POPUP_CRITICAL) {
                    btl_popupText((s32)unit, 3, 0x80, 0);
                    delay = 2;
                }
                if (flags & BTL_POPUP_HEAL) {
                    btl_popupHp((s32)unit, unit->hpChange, BTL_POPCLUT_HEAL, delay);
                } else {
                    btl_popupHp((s32)unit, unit->hpChange, 0, delay);
                }
                delay += 4;
            }
            if (flags & BTL_POPUP_MP) {
                if (flags & BTL_POPUP_MP_HEAL) {
                    btl_popupMp((s32)unit, unit->mpChange, BTL_POPCLUT_HEAL, delay);
                } else {
                    btl_popupMp((s32)unit, unit->mpChange, 0, delay);
                }
            }
            unit->popupFlags = 0;
        }
    } else {
        unit->popupFlags = 0;
    }
    unit->hpChange = 0;
    unit->mpChange = 0;
}

void btl_spawnTickPopups(BtlUnit *unit) {
    u8 flags = unit->tickFlags;
    u8 delay = 0;
    if (!((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
        if (flags & BTL_TICK_REGEN) {
            btl_popupHp((s32)unit, unit->tickHeal, (flags & BTL_TICK_REGEN_HURTS) ? 0 : BTL_POPCLUT_HEAL, 0)->info.b.noClamp = 1;
            delay = 4;
        }
        if (flags & BTL_TICK_POISON) {
            btl_popupHp((s32)unit, unit->tickDamage, 0, delay)->info.b.noClamp = 1;
            delay += 4;
        }
        if (flags & BTL_TICK_VENOM_MP) {
            btl_popupMp((s32)unit, unit->tickMpDamage, 0, delay)->info.b.noClamp = 1;
        }
    }
    unit->tickFlags = 0;
    unit->tickHeal = 0;
    unit->tickDamage = 0;
    unit->tickMpDamage = 0;
}

BtlPopup *btl_newPopup(BtlUnit *owner) {
    BtlPopupRing *ring;
    BtlPopup *popup;
    BtlBone *bone;
    s16 i;

    ring = &g_gameState->gameData->ctx.battle->popups;
    i = ring->last - 1;
    if (i < 0) {
        i = 15;
    }
    ring->last = i;
    popup = &ring->entries[i];
    popup->unit = owner;
    popup->frame = 0;
    popup->info.w = 0;
    bone = &owner->model->bones[owner->baseBone];
    popup->x = bone->tx;
    popup->y = bone->ty - 4;
    popup->z = bone->tz;
    return popup;
}

BtlPopup *btl_popupHp(s32 unit, s16 value, s16 clut, u8 delay) {
    BtlPopup *popup = btl_newPopup((BtlUnit *)unit);
    popup->info.b.kind = BTL_POPKIND_HP;
    popup->info.b.delay = delay;
    popup->clut = clut;
    popup->data.w = btl_packDigits(value);
    return popup;
}

BtlPopup *btl_popupMp(s32 unit, s16 value, s16 clut, u8 delay) {
    BtlPopup *popup = btl_newPopup((BtlUnit *)unit);
    popup->info.b.kind = BTL_POPKIND_MP;
    popup->info.b.delay = delay;
    popup->clut = clut;
    popup->data.w = btl_packDigits(value);
    return popup;
}

BtlPopup *btl_popupText(unit, textId, clut, delay)
    s32 unit;
    u8 textId;
    s16 clut;
    s8 delay;
{
    BtlPopup *popup = btl_newPopup((BtlUnit *)unit);
    popup->info.b.kind = BTL_POPKIND_TEXT;
    popup->info.b.delay = delay;
    popup->data.p = &g_btlTextPopups[textId];
    popup->clut = clut;
    return popup;
}

/* Packs value (< 9999) as decimal digits, one per nibble, with the digit count in bits 16+. */
s32 btl_packDigits(u32 value) {
    u32 packed = 0;
    u16 digit;
    u16 i;
    u32 rest;
    do {
        if (value >= 9999) {
            return 0x49999;
        }
        for (i = 3; i != 0; i--) {
            digit = 0;
            if (value >= g_btlPowersOf10[i]) {
                digit = value / g_btlPowersOf10[i];
                value = value % g_btlPowersOf10[i];
            }
            packed = (packed << 4) | digit;
        }
        packed = (packed << 4) | value;
    } while (0);
    for (i = 1, rest = packed >> 4; rest != 0; rest >>= 4) {
        i++;
    }
    return packed | (i << 16);
}

u8 *btl_drawPopups(u8 *pkt) {
    Unk800CCD00 ctx;
    BtlPopupRing *ring;
    BtlPopup *popup;
    s16 i;
    s16 j;
    BtlUnit *listUnit;
    u8 hiddenMask;
    Unk800F78A40 *fxState;

    ring = &g_gameState->gameData->ctx.battle->popups;
    ctx.prim = (u32 *)pkt;
    ctx.ot = &g_gameState->gameData->ot[g_gameState->gameData->drawBuf][4];
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    j = ring->last;
    for (i = 0; i < 16; i++) {
        popup = &ring->entries[j];
        if (popup->unit != NULL) {
            if (popup->info.b.kind >= BTL_POPKIND_DONE) {
                popup->unit = NULL;
            } else if (popup->info.b.delay != 0) {
                popup->info.b.delay--;
            } else {
                gte_ldv0(&popup->x);
                gte_rtps();
                gte_stsxy(&ctx.xy);
                if (popup->info.b.yOfs != 0 && (u16)(ctx.xy.s.y - 21) < 180) {
                    popup->info.b.yOfs = 0;
                }
                g_btlPopupDrawFns[popup->info.b.kind](popup, &ctx);
            }
        }
        j++;
        if (j >= 16) {
            j = 0;
        }
    }
    btl_drawStatusCountdown(&ctx);
    ((u8 *)ctx.prim)[3] = 1;
    ctx.prim[1] = get_mode(0, 0, g_popupDigitSprites[0].tpage);
    addPrim(ctx.ot, ctx.prim);
    ctx.prim += 2;
    fxState = g_btlfxEnv->state;
    if (((fxState->flags ^ 1) & 1) | ((fxState->phase == BTLFX_PHASE_IDLE) | ((fxState->flags >> 15) & 1))) {
        btl_drawStatusIcons(&ctx);
    }
    ring->tick++;
    btl_updateScan();
    btl_updateDetect();
    hiddenMask = 0xFF;
    for (listUnit = g_gameState->gameData->ctx.battle->unitList; listUnit != NULL; listUnit = listUnit->next) {
        if (!((listUnit->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
            hiddenMask &= ~listUnit->unitBit;
        }
    }
    ring->hiddenMask = hiddenMask;
    return (u8 *)ctx.prim;
}

void btl_drawNumberPopup(BtlPopup *popup, Unk800CCD00 *ctx) {
    u8 *anim;
    u8 step;
    u32 digits;
    u32 width;
    s16 x;

    if (popup->clut == BTL_POPCLUT_HEAL) {
        anim = g_btlPopupBounceHeal;
    } else {
        anim = g_btlPopupBounce;
    }
    step = anim[popup->frame];
    if (!(popup->unit->model->flags & BTL_MODEL_OFFSCREEN)) {
        digits = popup->data.w;
        ctx->rgb = (step & 0x80) ? 0x03000000 : 0x01000000;
        ctx->clut = popup->clut;
        width = (digits >> 16) * 8;
        ctx->xy.s.x -= width >> 1;
        ctx->xy.s.y -= (step & 0x7F) + 5;
        if (popup->info.b.noClamp == 0) {
            x = width;
            if (popup->info.b.kind != BTL_POPKIND_HP) {
                x = g_popupMpSprite.w + width;
            }
            btl_clampPopup(popup, ctx, x, 10);
        } else {
            ctx->xy.s.y += popup->info.b.yOfs;
        }
        btl_drawPopupDigits(ctx, digits);
        if (popup->info.b.kind != BTL_POPKIND_HP) {
            ctx->xy.s.x += (digits >> 16) * 8;
            ctx->xy.s.y += 3;
            btl_drawPopupGlyph(ctx, &g_popupMpSprite);
        }
    }
    if (anim[++popup->frame] == 0xFF) {
        popup->unit = NULL;
    }
}

void btl_drawTextPopup(BtlPopup *popup, Unk800CCD00 *ctx) {
    u8 *anim;
    u8 step;
    SpriteDef *glyph;

    anim = popup->data.p->anim;
    step = anim[popup->frame];
    if (!(popup->unit->model->flags & BTL_MODEL_OFFSCREEN)) {
        ctx->rgb = (step & 0x80) ? 0x03000000 : 0x01000000;
        ctx->clut = popup->clut;
        glyph = &g_uiSprites[popup->data.p->glyph];
        ctx->xy.s.x -= glyph->w >> 1;
        ctx->xy.s.y -= (glyph->h >> 1) + (step & 0x1F);
        if (popup->info.b.noClamp == 0) {
            btl_clampPopup(popup, ctx, glyph->w, glyph->h);
        }
        btl_drawPopupGlyph(ctx, glyph);
    }
    popup->frame++;
    if (anim[popup->frame] == 0xFF) {
        popup->unit = NULL;
    }
}

void btl_clampPopup(BtlPopup *popup, Unk800CCD00 *ctx, s32 w, s32 h) {
    if (popup->frame == 0) {
        if (ctx->xy.s.y - 21 < 0) {
            popup->info.b.yOfs = 21 - ctx->xy.s.y;
        } else if (ctx->xy.s.y + h >= 211) {
            popup->info.b.yOfs = -46 - (ctx->xy.s.y + h);
        }
    }
    if (ctx->xy.s.x < 8) {
        ctx->xy.s.x = 8;
    } else if (ctx->xy.s.x + w >= 313) {
        ctx->xy.s.x = 312 - w;
    }
    ctx->xy.s.y += popup->info.b.yOfs;
    if (ctx->xy.s.y < 8) {
        ctx->xy.s.y = 8;
    } else if (ctx->xy.s.y + h >= 211) {
        ctx->xy.s.y = 210 - h;
    }
}

void btl_drawPopupDigits(Unk800CCD00 *ctx, u32 digits) {
    DVECTOR xy;
    s16 i;
    SpriteDef *glyph;

    i = (digits >> 16) - 1;
    xy.vx = ctx->xy.s.x + (((s32)digits >> 16) - 1) * 8;
    xy.vy = ctx->xy.s.y;
    for (; i != -1; i--) {
        ctx->prim[0] = 0x04000000;
        ctx->prim[1] = ctx->rgb | GPU_CODE(GPU_SPRT);
        ctx->prim[2] = *(s32 *)&xy;
        glyph = &g_popupDigitSprites[digits & 0xF];
        ctx->prim[3] = glyph->uvclut + (ctx->clut << 16);
        ctx->prim[4] = (glyph->h << 16) | glyph->w;
        addPrim(ctx->ot, ctx->prim);
        ctx->prim += 5;
        xy.vx -= 8;
        digits >>= 4;
    }
}

void btl_drawPopupGlyph(Unk800CCD00 *ctx, SpriteDef *glyph) {
    ctx->prim[0] = 0x04000000;
    ctx->prim[1] = ctx->rgb | GPU_CODE(GPU_SPRT);
    ctx->prim[2] = ctx->xy.w;
    ctx->prim[3] = glyph->uvclut + (ctx->clut << 16);
    ctx->prim[4] = (glyph->h << 16) | glyph->w;
    addPrim(ctx->ot, ctx->prim);
    ctx->prim += 5;
}

void btl_drawStatusIcons(Unk800CCD00 *ctx) {
    BtlPopupRing *ring;
    BtlUnit *unit;
    BtlModel *mdl;
    BtlBone *attachBone;
    Unk800F6A88 *icon;
    SVECTOR pos;
    SVECTOR rot;
    u32 status;
    s16 sn;
    s16 cs;
    s16 y;
    s32 y0;
    s32 z0;
    s16 z;
    s32 attach;
    s32 frame;
    s16 i;
    u8 *iconBones;
    u8 *iconY;
    u8 *iconZ;

    ring = &g_gameState->gameData->ctx.battle->popups;
    btlfx_beginPrims((u8 *)ctx->prim, g_gameState->gameData->ot[g_gameState->gameData->drawBuf]);
    g_btlfxEnv->otZ = 0;
    rot.vx = 0;
    rot.vz = 0;
    rot.pad = 1;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1) || (unit->model->flags & BTL_MODEL_OFFSCREEN)) {
            continue;
        }
        if (ring->hiddenMask & unit->unitBit) {
            continue;
        }
        status = unit->status | unit->autoStatus;
        if ((status & BTL_STATUS_KO) || !(status & (BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | BTL_STATUS_TROUBLE | BTL_STATUS_BERSERK | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_REFLECT))) {
            continue;
        }
        if ((unit->flags & BTL_UNIT_PARTY) && unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_FLEE]) {
            continue;
        }
        sn = math_sin(unit->rot[1]);
        cs = math_cos(unit->rot[1]);
        if (unit->flags & BTL_UNIT_PARTY) {
            iconBones = g_btlPartyIconBones[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind];
            iconY = g_btlPartyIconY[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind];
            iconZ = g_btlPartyIconZ[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind];
        } else {
            BtlEnemyType *enemyType = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type;
            iconBones = enemyType->iconBones;
            iconY = enemyType->iconY;
            iconZ = enemyType->iconZ;
        }
        for (icon = g_btlStatusIcons, i = 11; i != -1; icon++, i--) {
            if (!(status & icon->showMask) || (status & icon->hideMask)) {
                continue;
            }
            attach = icon->attach;
            mdl = unit->model;
            y0 = (iconY[attach] << 24) >> 20;
            z0 = (iconZ[attach] << 24) >> 20;
            y = y0;
            z = z0;
            if (mdl->flags & BTL_MODEL_SCALED) {
                y = (u32)(y0 * mdl->scaleY) >> 12;
                z = (u32)(z0 * mdl->scaleZ) >> 12;
            }
            attachBone = &mdl->bones[iconBones[attach]];
            pos.vx = attachBone->tx + (((s16)z * sn) >> 12);
            pos.vz = attachBone->tz + (((s16)z * cs) >> 12);
            pos.vy = attachBone->ty + y;
            if (icon->single != 0) {
                frame = (u16)ring->tick;
                btlfx_drawSprite(icon->sprite, &pos, frame % (*(u16 *)icon->sprite & 0x7FFF), icon->abr, -1);
            } else {
                if ((rot.pad = icon->turnsWithUnit) != 0) {
                    rot.vy = (unit->rot[1] + 0xC00) & 0xFFF;
                } else {
                    rot.vy = 0;
                }
                btlfx_drawSprites(icon->sprite, &pos, &rot, -1, (u16)ring->tick, icon->abr, -1);
            }
        }
    }
    ctx->prim = (u32 *)btlfx_endPrims();
}

void btl_drawStatusCountdown(Unk800CCD00 *ctx) {
    BtlUnit *unit;
    BtlEnemyType *enemyType;
    BtlModel *mdl;
    BtlBone *headBone;
    Unk800F6CA4 *countdown;
    SVECTOR pos;
    u32 status;
    s16 bone;
    s16 y;
    s16 i;
    s16 timer;
    s32 count;
    s16 speed;
    u8 shade;
    s32 blink;
    u32 digits;

    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1) || (unit->model->flags & BTL_MODEL_OFFSCREEN)) {
            continue;
        }
        if (g_gameState->gameData->ctx.battle->popups.hiddenMask & unit->unitBit) {
            continue;
        }
        status = unit->status;
        if ((status & BTL_STATUS_KO) || !(status & (BTL_STATUS_DOOM | BTL_STATUS_GRADUAL_PETRIFY))) {
            continue;
        }
        if (unit->flags & BTL_UNIT_PARTY) {
            bone = g_btlPartyIconBones[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind][5];
            y = (s8)g_btlPartyIconY[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind][5];
        } else {
            enemyType = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type;
            bone = enemyType->iconBones[5];
            y = (s8)enemyType->iconY[5];
        }
        mdl = unit->model;
        if (mdl->flags & BTL_MODEL_SCALED) {
            y = (u32)(y * mdl->scaleY) >> 12;
        }
        headBone = &mdl->bones[bone];
        pos.vx = headBone->tx;
        pos.vy = headBone->ty + (y << 4);
        pos.vz = headBone->tz;
        gte_ldv0(&pos);
        gte_rtps();
        gte_stsxy(&ctx->xy);
        ctx->xy.s.y -= 12;
        for (countdown = g_btlStatusCountdowns, i = 1; i != -1; countdown++, i--) {
            if (status & countdown->status) {
                if (unit->countdownStep > 0 && (timer = unit->statusTimers[countdown->timer]) >= 0) {
                    count = timer * 10 / unit->countdownStep;
                    speed = unit->atbSpeed;
                    bone = count;
                    if (timer <= 0) {
                        blink = 2;
                    } else {
                        blink = ((s16)count != (s16)((timer - speed) * 10 / unit->countdownStep)) * 2;
                    }
                    if (countdown->colorByLevel != 0) {
                        shade = (bone << 4) + 0x20;
                        ctx->rgb = (shade << 16) | (shade << 8) | shade;
                    } else {
                        ctx->rgb = 0x1000000;
                    }
                    ctx->clut = 0;
                    ctx->rgb |= blink << 24;
                    timer = bone + 1;
                    if (timer < 10) {
                        digits = timer | 0x10000;
                    } else {
                        digits = 0x20010;
                    }
                    ctx->xy.s.x -= (digits >> 16) * 4;
                    btl_drawPopupDigits(ctx, digits);
                }
                break;
            }
        }
    }
}

void btl_showMessage(u32 msgId, ...) {
    u8 msg[0x50];
    va_list ap;
    TextTbl *tbl = (TextTbl *)g_btlMessages;
    TextTblEnt *entries;
    u8 *src;
    u8 *dst;
    u8 *special;
    s32 kind;
    u8 ch;
    s32 str;

    msgId += 8;
    if (msgId < tbl->count) {
        va_start(ap, msgId);
        dst = &msg[1];
        entries = tbl->entries;
        src = (u8 *)entries + entries[msgId].ofs;
        kind = *src++;
        special = (u8 *)entries + tbl->entries[0].ofs;
        ch = msg[0] = *src++;
        while (ch != 0xFF) {
            if (ch == special[0]) {
                dst--;
                str = va_arg(ap, s32);
                if (str != 0) {
                    text_copy(dst, (u8 *)str);
                    goto adv;
                }
            } else if (ch == special[1]) {
                dst--;
                text_formatInt(va_arg(ap, s32), dst, TEXT_NUM_CODES);
            adv:
                dst += text_getLength(dst);
            }
            ch = *dst++ = *src++;
        }
        btl_setBanner((s32)msg, kind);
    }
}

s32 btl_getElementName(u8 element) {
    s32 index = 0;
    while (element >>= 1) {
        index++;
    }
    return text_getFrom(g_btlMessages, index + 1);
}

u8 *btl_getItemName(u8 itemId) {
    return &g_itemNames[g_itemTable[itemId].name];
}

void *btl_getUnitName(BtlUnit *unit) {
    u32 flags = unit->flags;
    Battle *battle;
    if (!(flags & BTL_UNIT_PARTY)) {
        battle = g_gameState->gameData->ctx.battle;
        return battle->work->texts + battle->enemies[BTL_UNIT_INDEX(flags)].type->nameOfs;
    }
    return &g_gameState->gameData->characters[BTL_UNIT_INDEX(flags)];
}

void btl_startScan(s32 unit) {
    g_gameState->gameData->ctx.battle->popups.scanUnit = unit;
    g_gameState->gameData->ctx.battle->popups.scan.w = 0;
}

void btl_updateScan(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlUnit *unit = (BtlUnit *)battle->popups.scanUnit;
    u8 line[64];
    u32 k;
    u32 j;
    u16 bits;
    Battle *scanBattle;

    if (unit == NULL) {
        return;
    }
    if (--battle->popups.scan.b[0] != 0xFF) {
        return;
    }
    scanBattle = g_gameState->gameData->ctx.battle;
    switch (scanBattle->popups.scan.b[2]) {
    case 0:
        text_copy(line, btl_getUnitName(unit));
        text_append(line, (u8 *)text_getFrom(g_btlScanTexts, 10));
        text_formatInt(unit->level, line + text_getLength(line), TEXT_NUM_CODES);
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 1;
        break;
    case 1:
        text_copy(line, (u8 *)text_getFrom(g_btlScanTexts, 11));
        text_formatInt(unit->hp, line + text_getLength(line), TEXT_NUM_CODES);
        text_append(line, (u8 *)text_getFrom(g_btlScanTexts, 13));
        text_formatInt(unit->max->maxHp, line + text_getLength(line), TEXT_NUM_CODES);
        text_append(line, (u8 *)text_getFrom(g_btlScanTexts, 12));
        text_formatInt(unit->mp, line + text_getLength(line), TEXT_NUM_CODES);
        text_append(line, (u8 *)text_getFrom(g_btlScanTexts, 13));
        text_formatInt(unit->max->maxMp, line + text_getLength(line), TEXT_NUM_CODES);
        g_gameState->gameData->ctx.battle->popups.scan.b[1] = 0;
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 2;
        break;
    case 2:
        if (!(unit->flags & BTL_UNIT_PARTY)) {
            bits = scanBattle->enemies[BTL_UNIT_INDEX(unit->flags)].type->category;
            while ((j = g_gameState->gameData->ctx.battle->popups.scan.b[1]++) < 8) {
                if ((bits >> j) & 1) {
                    text_copy(line, (u8 *)text_getFrom(g_btlScanTexts, j));
                    goto show;
                }
            }
        }
        g_gameState->gameData->ctx.battle->popups.scan.b[1] = 0;
        g_gameState->gameData->ctx.battle->popups.scan.b[2] = 3;
    case 3:
        bits = unit->elemWeak & ~unit->elemGuard;
        while ((k = g_gameState->gameData->ctx.battle->popups.scan.b[1]++) < 8) {
            if ((bits >> k) & 1) {
                text_copy(line, (u8 *)text_getFrom(g_btlScanTexts, k + 14));
                goto show;
            }
        }
    default:
        g_gameState->gameData->ctx.battle->popups.scanUnit = 0;
        btl_setBanner(0, 2);
        return;
    }
show:
    g_gameState->gameData->ctx.battle->popups.scan.b[0] = btl_getBannerDuration() - 1;
    btl_setBanner((s32)line, 2);
}

void btl_startDetect(BtlUnit *unit) {
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        g_gameState->gameData->ctx.battle->popups.detectEnemy = &g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)];
        if (g_gameState->gameData->ctx.battle->popups.detectEnemy->steal.w == -1) {
            btl_setBanner(text_getFrom(g_btlScanTexts, 9), 2);
            g_gameState->gameData->ctx.battle->popups.detectSlot = 4;
            g_gameState->gameData->ctx.battle->popups.detectDelay = btl_getBannerDuration() - 1;
            return;
        }
        g_gameState->gameData->ctx.battle->popups.detectSlot = 0;
        g_gameState->gameData->ctx.battle->popups.detectDelay = 0;
    }
}

void btl_updateDetect(void) {
    u8 line[0x40];
    u8 slot;
    u8 id;
    u8 *text;
    s32 kind;

    if (g_gameState->gameData->ctx.battle->popups.detectEnemy == NULL) {
        return;
    }
    if (--g_gameState->gameData->ctx.battle->popups.detectDelay != 0xFF) {
        return;
    }
    while ((slot = g_gameState->gameData->ctx.battle->popups.detectSlot++) < 4) {
        id = g_gameState->gameData->ctx.battle->popups.detectEnemy->steal.b[3 - slot];
        if (id != 0xFF) {
            g_gameState->gameData->ctx.battle->popups.detectDelay = btl_getBannerDuration() - 1;
            text_copy(line, (u8 *)text_getFrom(g_btlScanTexts, 8));
            text_append(line, btl_getItemName(id));
            text_append(line, s_btlDetectItemSuffix);
            text = line;
            kind = 2;
            goto show;
        }
    }
    text = NULL;
    kind = 1;
    g_gameState->gameData->ctx.battle->popups.detectEnemy = NULL;
show:
    btl_setBanner((s32)text, kind);
}

void btl_showCmdName(BtlCmd *cmd) {
    u8 *name = NULL;
    u8 nameBuf[32];
    u16 mapped;
    switch (cmd->cmdNo) {
    case BTL_CMD_ITEM:
    case BTL_CMD_THROW:
        name = btl_getItemName(cmd->subNo);
        break;
    case BTL_CMD_RETURN_MAGIC:
        name = g_gameState->gameData->ctx.battle->work->texts + cmd->ability->nameOfs;
        break;
    default:
        if (cmd->subNo < ABILITY_SUPPORT_FIRST) {
            mapped = g_btlCmdNameMap[cmd->subNo];
            switch (mapped) {
            case 0:
                break;
            case 0xFF:
                name = btl_getAbilityName(cmd->subNo);
                break;
            case 0xFE:
                name = nameBuf;
                text_copy(name, btl_getAbilityName(cmd->subNo));
                text_append(name, (u8 *)text_getFrom(g_btlCmdNameTexts, 0));
                break;
            default:
                if (mapped < 0xC0) {
                    name = btl_getAbilityName(mapped);
                } else {
                    name = (u8 *)text_getFrom(g_btlCmdNameTexts, (mapped & 0x3F) + 1);
                }
                break;
            }
        }
        break;
    }
    if (name != NULL) {
        btl_setBannerAt((s32)name, 1);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btl_popup", s_btlDetectItemSuffix);

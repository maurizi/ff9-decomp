#include "ovl_0b.h"

void btl_addMeshRgb(BtlModel *model, u8 meshIndex, s16 addR, s16 addG, s16 addB) {
    BtlMesh *mesh = &model->header->meshes[meshIndex];
    u32 *p = mesh->prims;
    u32 i;
    for (i = 0; i < mesh->nFT4; i++) {
        p[4] = btl_addRgbClamped(p[4], addR, addG, addB);
        p += 6;
    }
    for (i = 0; i < mesh->nFT3; i++) {
        p[2] = btl_addRgbClamped(p[2], addR, addG, addB);
        p += 5;
    }
}

void btl_setMeshRgb(BtlModel *model, u8 meshIndex, s16 r, s16 g, s16 b) {
    BtlMesh *mesh = &model->header->meshes[meshIndex];
    u32 *p = mesh->prims;
    u32 rgb = (b << 16) | (g << 8) | r;
    u32 i;
    for (i = 0; i < mesh->nFT4; i++) {
        p[4] = (p[4] & 0xFF000000) | rgb;
        p += 6;
    }
    for (i = 0; i < mesh->nFT3; i++) {
        p[2] = (p[2] & 0xFF000000) | rgb;
        p += 5;
    }
}

u32 btl_addRgbClamped(u32 color, s32 addR, s32 addG, s32 addB) {
    s16 b = addB + ((color >> 16) & 0xFF);
    s16 g = addG + ((color >> 8) & 0xFF);
    s16 r = addR + (color & 0xFF);
    color &= 0xFF000000;
    if (r >= 0x100) {
        color |= 0xFF;
    } else if (r > 0) {
        color |= r;
    }
    if (g >= 0x100) {
        color |= 0xFF00;
    } else if (g > 0) {
        color |= g << 8;
    }
    if (b >= 0x100) {
        color |= 0xFF0000;
    } else if (b > 0) {
        color |= b << 16;
    }
    return color;
}

void btl_setModelAbr(BtlModel *model, s32 abr) {
    u8 *p;
    u32 i;
    u8 code;
    if (!(model->flags & BTL_MODEL_OFFSCREEN)) {
        model->abr = abr;
        p = (u8 *)model->prims;
        for (i = 0; i < model->header->meshCount; i++) {
            if (!(model->hiddenMeshes & (1 << i))) {
                do {
                    code = getcode(p);
                    if (code == 0xFF) {
                        p += sizeof(DR_TPAGE);
                    } else if (code == 0xE1) {
                        ((DR_TPAGE *)p)->code[0] = (u16)abr | 0xE1000000;
                        p += sizeof(DR_TPAGE);
                    } else {
                        code &= 0x3C;
                    }
                    switch (code) {
                    case GPU_POLY_FT4:
                        setSemiTrans(p, 1);
                        ((POLY_FT4 *)p)->tpage = abr | (((POLY_FT4 *)p)->tpage & ~TPAGE_ABR_MASK);
                        p += sizeof(POLY_FT4);
                        break;
                    case GPU_POLY_FT3:
                        setSemiTrans(p, 1);
                        ((POLY_FT3 *)p)->tpage = abr | (((POLY_FT3 *)p)->tpage & ~TPAGE_ABR_MASK);
                        p += sizeof(POLY_FT3);
                        break;
                    case GPU_POLY_GT4:
                        setSemiTrans(p, 1);
                        ((POLY_GT4 *)p)->tpage = abr | (((POLY_GT4 *)p)->tpage & ~TPAGE_ABR_MASK);
                        p += sizeof(POLY_GT4);
                        break;
                    case GPU_POLY_GT3:
                        setSemiTrans(p, 1);
                        ((POLY_GT3 *)p)->tpage = abr | (((POLY_GT3 *)p)->tpage & ~TPAGE_ABR_MASK);
                        p += sizeof(POLY_GT3);
                        break;
                    case GPU_POLY_G4:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G4);
                        break;
                    case GPU_POLY_G3:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G3);
                        break;
                    case GPU_POLY_F4:
                        p += sizeof(POLY_F4);
                        break;
                    case GPU_POLY_F3:
                        p += sizeof(POLY_F3);
                        break;
                    }
                } while (code != 0xFF);
            }
        }
    }
}

void btl_updateTexAnims(TexAnimSet *set) {
    RECT srcRect;
    u32 i;
    u32 j;
    TexAnim *anim;
    TexAnimRect *rect;
    s32 pos;
    s16 frame;
    s16 last;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->count; i++) {
        anim = &set->anims[i];
        if (!(anim->flags & BTL_TEXANIM_PLAY)) {
            continue;
        }
        if (anim->frameCount != 0) {
            pos = anim->pos;
            frame = pos >> 12;
            last = anim->lastFrame;
            if (frame >= 0) {
                if (frame != last) {
                    for (j = 0; j < anim->rectCount; j++) {
                        rect = &anim->rects[j];
                        srcRect.x = rect->src[frame][0];
                        srcRect.y = rect->src[frame][1];
                        srcRect.w = rect->w;
                        srcRect.h = rect->h;
                        MoveImage(&srcRect, rect->x, rect->y);
                    }
                    anim->lastFrame = frame;
                }
                pos += anim->speed;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < anim->frameCount) {
                anim->pos = pos;
            } else if (anim->waitRand != 0) {
                RAND_REFILL2();
                anim->pos = -((anim->waitMin + (anim->waitRand + 1) * RAND16() / 0x10000) << 12);
            } else if (anim->flags & BTL_TEXANIM_ONCE) {
                anim->flags &= 0xFC;
            } else {
                anim->pos = 0;
            }
        } else if (anim->flags & BTL_TEXANIM_VSCROLL) {
            pos = anim->pos;
            frame = pos >> 12;
            for (j = 0; j < anim->rectCount; j++) {
                rect = &anim->rects[j];
                MoveImage((RECT *)rect, rect->x, rect->y - frame);
                srcRect.x = rect->x;
                srcRect.y = rect->y - frame;
                srcRect.w = rect->w;
                srcRect.h = frame;
                MoveImage(&srcRect, rect->x, rect->y + rect->h - frame);
            }
            anim->pos = (pos & 0xFFF) + anim->speed;
        }
    }
}

void btl_relocTexAnims(TexAnimSet *set, u16 (*ofs)[4]) {
    u32 i;
    u32 j;
    u32 k;
    TexAnimRect *rects;
    u16 *ptr;
    u16 *yPtr;
    u8 idx;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->count; i++) {
        set->anims[i].lastFrame = 1;
        ptr = (u16 *)set->anims[i].rects;
        if ((u32)ptr > 0x80000000) {
            return;
        }
        if (ptr == NULL) {
            continue;
        }
        idx = set->anims[i].relocIndex;
        ptr = (u16 *)((u8 *)ptr + (s32)set);
        set->anims[i].rects = (TexAnimRect *)ptr;
        if (ofs != NULL) {
            ptr[0] += ofs[idx][2];
            ptr[1] += ofs[idx][3];
        }
        rects = (TexAnimRect *)ptr;
        for (j = 0; j < set->anims[i].rectCount; j++) {
            ptr = (u16 *)rects[j].src;
            ptr = (u16 *)((u8 *)ptr + (s32)set);
            rects[j].src = (void *)ptr;
            if (ofs != NULL) {
                for (k = 0; k < set->anims[i].frameCount; k++) {
                    yPtr = ptr + 1;
                    *ptr += ofs[idx][0];
                    *yPtr += ofs[idx][1];
                    ptr += 2;
                }
            }
        }
    }
}

Battle11C020 *btl_initScene(void) {
    Battle *battle;
    Battle11C020 *enc = g_gameState->gameData->ctx.battle->work->encounter;
    u16 *bundle = g_gameState->gameData->ctx.battle->work->bundle;
    s32 rnd;
    Battle132C *sc = &g_gameState->gameData->ctx.battle->scene;
    u8 groupCam;
    u8 cam;

    sc->seqTable = (u16 *)((u8 *)bundle + bundle[0]);
    sc->sceneData = (s16 *)((u8 *)bundle + bundle[1]);
    sc->group = btl_pickEnemyGroup(g_gameState->gameData->ctx.battle->work->encounter);
    sc->enemyTypes = (Battle1334 *)&g_gameState->gameData->ctx.battle->work->encounter->groups[enc->groupCount];
    groupCam = sc->group->cameraNo;
    battle = g_gameState->gameData->ctx.battle;
    if (groupCam >= 3) {
        rnd = rand8_inline();
        cam = rnd % 3;
    } else {
        cam = groupCam;
    }
    battle->cameraNo = cam;
    return enc;
}

Battle1330 *btl_pickEnemyGroup(Battle11C020 *encounter) {
    Battle1330 *group;
    u16 roll;
    u16 sum;
    u16 i;
    s8 forced;
    roll = (rand8_inline() * 100) / 256;
    group = encounter->groups;
    sum = group->weight;
    i = 0;
    while (roll >= sum) {
        group++;
        sum += group->weight;
        i++;
    }
    forced = g_gameState->gameData->enemyGroup;
    if (forced != -1) {
        i = forced;
        if (i >= encounter->groupCount) {
            i = 0;
        }
        group = &encounter->groups[i];
    }
    g_gameState->gameData->ctx.battle->groupNo = i;
    return group;
}

void btl_initSceneFlags(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    battle->scene.flags = (battle->work->encounter->flags ^ 0x30) * 4;
    g_btlRewards.ap = g_gameState->gameData->ctx.battle->scene.group->ap;
}

void btl_setupEnemies(void) {
    Battle *battle;
    Battle1330 *grp;
    Battle11C020 *hdr;
    s16 i;
    s16 k;
    u8 nextAbility;
    Unk800C9D1C *recs;
    Unk800C9D1C *rec;
    BtlEnemyType *en;
    U16Pair *ofs;
    BtlCmd8 *src;
    u32 *flags;
    BtlCmd8 *dst;
    BtlCmd8 *cmd;
    BtlUnit *unit;
    Battle1330 *grp2;

    battle = g_gameState->gameData->ctx.battle;
    flags = &battle->scene.flags;
    grp = battle->scene.group;
    hdr = battle->work->encounter;
    battle->scene.flags = (battle->scene.flags & ~BTL_SCENE_AMBUSH) | (btl_rollAmbush(flags) & 3);
    i = 0;
    nextAbility = hdr->typeCount + hdr->abilityCount;
    grp2 = hdr->groups;
    recs = (Unk800C9D1C *)(grp2 + hdr->groupCount);
    rec = recs;
    en = g_gameState->gameData->ctx.battle->enemyTypes;
    ofs = (U16Pair *)(g_gameState->gameData->ctx.battle->work->texts + 4);
    src = (BtlCmd8 *)&recs[hdr->typeCount];
    for (; i < hdr->typeCount; i++) {
        btl_initEnemyType(rec, en);
        en->camera = btl_getSceneData(i + 5);
        en->nameOfs = ofs->text + 4;
        ofs++;
        en->firstAbility = nextAbility;
        nextAbility += rec->abilityCount;
        en++;
        rec++;
    }
    dst = g_gameState->gameData->ctx.battle->enemyAbilities;
    for (i = 0, cmd = src; i < hdr->abilityCount; i++, cmd++) {
        btl_copyAbility(cmd, dst);
        dst->nameOfs = ofs->text + 4;
        ofs++;
        dst++;
    }
    unit = g_gameState->gameData->ctx.battle->unitList;
    for (i = 0; i < grp->enemyCount; i++) {
        if (unit == NULL) {
            break;
        }
        btl_initEnemyUnit(&grp->enemies[i], unit, (s32 *)flags, i);
        ((void (*)(Unk800C9D1C *, BtlUnit *, s32))btl_setEnemyStats)(&recs[grp->enemies[i].type], unit, i);
        unit = unit->next;
    }
}

void btl_placeParty(void) {
    s16 partyCount;
    s16 i;
    Battle *battle;
    s32 x;
    BtlUnit *unit;
    s16 rot;
    BtlUnit98 *p98;
    BtlUnitA0 *pa0;
    BtlUnit98 *q98;
    BtlUnitA0 *qa0;
    s32 w;
    s32 posY;
    Unk800F57F0 *shadow;
    Unk800F57F0 *tbl;
    u32 *flags;
    u32 unitFlags;

    partyCount = 0;
    battle = g_gameState->gameData->ctx.battle;
    flags = &battle->scene.flags;
    for (i = 0; i < PARTY_SIZE; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            partyCount++;
        }
    }
    x = (partyCount - 1) * 0x13C;
    i = 0;
    rot = ((*flags & 3) != 0) << 11;
    unit = g_gameState->gameData->ctx.battle->unitList;
    for (; i < partyCount; i++) {
        if (!(unit->flags & BTL_UNIT_PARTY)) {
            return;
        }
        unitFlags = unit->flags & ~BTL_UNIT_ROW_MASK;
        unitFlags |= (g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.w >> (CHR_INFO_BATTLE_ROW_BIT - BTL_UNIT_ROW_SHIFT)) & (1 << BTL_UNIT_ROW_SHIFT);
        unit->flags = unitFlags;
        if (!(*flags & 3)) {
            unit->flags = (unit->flags & ~BTL_UNIT_ROW_MASK) | ((BTL_UNIT_ROW(unit->flags) ^ 1) << BTL_UNIT_ROW_SHIFT);
        }
        unit->pos[0] = unit->home->pos[0] = unit->obj->x = x << 16;
        p98 = unit->home;
        pa0 = unit->obj;
        if ((u8)btl_hasStatus(unit, BTL_STATUS_FLOAT)) {
            posY = 0xFF380000;
        } else {
            posY = 0;
        }
        unit->pos[1] = p98->pos[1] = pa0->y = posY;
        q98 = unit->home;
        qa0 = unit->obj;
        w = 0xF9E80000;
        if (!BTL_UNIT_ROW(unit->flags)) {
            w = 0xF8580000;
        }
        unit->pos[2] = q98->pos[2] = qa0->z = w;
        unit->obj->rotX = 0;
        unit->rot[0] = 0;
        unit->obj->rotY = rot;
        unit->rot[1] = rot;
        unit->obj->rotZ = 0;
        unit->rot[2] = 0;
        tbl = g_btlPartyShadows;
        shadow = &tbl[g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind];
        unit->shadowBone = shadow->shadowBone;
        unit->shadowDirBone = shadow->shadowDirBone;
        btl_setShadowSize(unit, shadow->shadowW, shadow->shadowD);
        x -= 0x278;
        g_gameState->gameData->ctx.battle->work->shadows[BTL_UNIT_INDEX(unit->flags)].offsetZ = shadow->shadowZ;
        unit = unit->next;
    }
}

void btl_initEnemyType(Unk800C9D1C *rec, BtlEnemyType *type) {
    s16 i;
    BtlMotion *motion;
    type->radius = rec->radius;
    type->category = rec->category;
    type->level = rec->level;
    type->eatAbility = rec->eatAbility;
    type->max.maxHp = rec->maxHp;
    type->max.maxMp = rec->maxMp;
    type->gil = rec->gil;
    type->exp = rec->exp;
    type->card = rec->card;
    type->drops.w = rec->drops;
    for (i = 0; i < 6; i++) {
        motion = db_lookupRegistered(DB_GET_ADDR, rec->motionIds[i] | DB_ANIM, 0);
        type->motions[i] = motion;
        if (IS_OFFSET(motion->rotKeys)) {
            motion->rotKeys = (s32)motion + motion->rotKeys;
            if (motion->rotKeysLo != 0) {
                motion->rotKeysLo = (s32)type->motions[i] + motion->rotKeysLo;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        type->bones[i] = rec->bones[i];
    }
    type->deathCry = rec->deathCry;
    type->confuseAbility = rec->confuseAbility;
    for (i = 0; i < 6; i++) {
        type->iconBones[i] = rec->iconBones[i];
        type->iconY[i] = rec->iconY[i];
        type->iconZ[i] = rec->iconZ[i];
    }
}

void btl_initEnemyUnit(Unk800C9EE0 *placement, BtlUnit *unit, s32 *sceneFlags, s32 index) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlEnemy *enemy = &battle->enemies[BTL_UNIT_INDEX(unit->flags)];
    BtlEnemyType *type;
    s16 rotY;
    enemy->type = &battle->enemyTypes[placement->type];
    unit->flags = (((unit->flags & ~BTL_UNIT_TARGETABLE) | ((placement->flags & 1) << BTL_UNIT_TARGETABLE_BIT)) & ~BTL_UNIT_ROW_MASK) | (2 << BTL_UNIT_ROW_SHIFT);
    type = &g_gameState->gameData->ctx.battle->enemyTypes[placement->type];
    unit->max = &type->max;
    unit->hp = type->max.maxHp;
    unit->mp = unit->max->maxMp;
    enemy->flags = (enemy->flags & ~BTL_ENEMYF_SHARED_MODEL) | ((placement->flags << 7) & BTL_ENEMYF_SHARED_MODEL);
    if (!((enemy->flags >> BTL_ENEMYF_NO_MODEL_BIT) & 1)) {
        unit->pos[0] = unit->home->pos[0] = unit->obj->x = placement->x << 16;
        unit->pos[1] = unit->home->pos[1] = unit->obj->y = placement->y << 16;
        unit->pos[2] = unit->home->pos[2] = unit->obj->z = placement->z << 16;
        unit->obj->rotX = 0;
        unit->rot[0] = 0;
        unit->obj->rotZ = 0;
        unit->rot[2] = 0;
        rotY = (placement->rotY + g_btlEnemyRotByAmbush[*sceneFlags & 3]) & 0xFFF;
        unit->obj->rotY = rotY;
        unit->rot[1] = rotY;
    }
    unit->motions = (s32 *)enemy->type->motions;
    unit->model->motion = enemy->type->motions[BTL_MOTION_IDLE];
}

void btl_setEnemyStats(Unk800C9D1C *rec, BtlUnit *unit) {
    BtlEnemy *enemy;
    BtlShadow *shadow;

    unit->statusImmune = rec->statusImmune;
    unit->autoStatus = rec->autoStatus;
    unit->status = rec->status;
    unit->hp = rec->maxHp;
    unit->mp = rec->maxMp;
    unit->defense = rec->defence[0];
    unit->evade = rec->defence[1];
    unit->magicDef = rec->defence[2];
    unit->magicEva = rec->defence[3];
    unit->speed = rec->stats[0];
    unit->strength = rec->stats[1];
    unit->magic = rec->stats[2];
    unit->spirit = rec->stats[3];
    *(s32 *)&unit->elemGuard = rec->elements;
    *(s32 *)&unit->hiddenMeshes = rec->meshMasks;
    unit->baseBone = rec->baseBone;
    enemy = &g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)];
    enemy->steal.w = rec->steal;
    enemy->flags = (enemy->flags & ~BTL_ENEMYF_SCRIPT_DEATH) | ((rec->flags & 1) << BTL_ENEMYF_SCRIPT_DEATH_BIT);
    enemy->flags = (enemy->flags & ~BTL_ENEMYF_DEATH_MOTION) | ((rec->flags << 6) & BTL_ENEMYF_DEATH_MOTION);
    btl_setShadowSize(unit, rec->shadowW, rec->shadowD);
    unit->shadowBone = rec->shadowBone;
    unit->shadowDirBone = rec->shadowDirBone;
    shadow = &g_gameState->gameData->ctx.battle->work->shadows[BTL_UNIT_INDEX(unit->flags) + 9];
    shadow->offsetX = rec->shadowX;
    shadow->offsetZ = rec->shadowZ;
}

void btl_copyAbility(BtlCmd8 *src, BtlCmd8 *dst) {
    *dst = *src;
}

s32 btl_getSceneData(s32 index) {
    s16 *table = g_gameState->gameData->ctx.battle->scene.sceneData;
    return (s32)table + table[index];
}

s16 btl_getEnemyModelId(s16 index) {
    Unk800C9EE0 *placement = &g_gameState->gameData->ctx.battle->scene.group->enemies[index];
    s32 modelId;
    if (index > 0 && (placement->flags & 2)) {
        modelId = -1;
    } else {
        modelId = g_gameState->gameData->ctx.battle->scene.enemyTypes[placement->type].modelId;
    }
    return modelId;
}

u16 btl_getEntranceSound(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    return battle->scene.enemyTypes[battle->scene.group->enemies[0].type].entranceSound;
}

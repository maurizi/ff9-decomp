#include "ovl_0b.h"

void btl_addMeshRgb(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->header->meshes[arg1];
    u32 *p = g->prims;
    u32 i;
    for (i = 0; i < g->nFT4; i++) {
        p[4] = btl_addRgbClamped(p[4], arg2, arg3, arg4);
        p += 6;
    }
    for (i = 0; i < g->nFT3; i++) {
        p[2] = btl_addRgbClamped(p[2], arg2, arg3, arg4);
        p += 5;
    }
}

void btl_setMeshRgb(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->header->meshes[arg1];
    u32 *p = g->prims;
    u32 v = (arg4 << 16) | (arg3 << 8) | arg2;
    u32 i;
    for (i = 0; i < g->nFT4; i++) {
        p[4] = (p[4] & 0xFF000000) | v;
        p += 6;
    }
    for (i = 0; i < g->nFT3; i++) {
        p[2] = (p[2] & 0xFF000000) | v;
        p += 5;
    }
}

u32 btl_addRgbClamped(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 b = arg3 + ((arg0 >> 16) & 0xFF);
    s16 g = arg2 + ((arg0 >> 8) & 0xFF);
    s16 r = arg1 + (arg0 & 0xFF);
    arg0 &= 0xFF000000;
    if (r >= 0x100) {
        arg0 |= 0xFF;
    } else if (r > 0) {
        arg0 |= r;
    }
    if (g >= 0x100) {
        arg0 |= 0xFF00;
    } else if (g > 0) {
        arg0 |= g << 8;
    }
    if (b >= 0x100) {
        arg0 |= 0xFF0000;
    } else if (b > 0) {
        arg0 |= b << 16;
    }
    return arg0;
}

void btl_setModelAbr(BtlUnit9C *arg0, s32 arg1) {
    u8 *p;
    u32 i;
    u8 code;
    if (!(arg0->flags & 0x10)) {
        arg0->abr = arg1;
        p = (u8 *)arg0->prims;
        for (i = 0; i < arg0->header->meshCount; i++) {
            if (!(arg0->hiddenMeshes & (1 << i))) {
                do {
                    code = getcode(p);
                    if (code == 0xFF) {
                        p += sizeof(DR_TPAGE);
                    } else if (code == 0xE1) {
                        ((DR_TPAGE *)p)->code[0] = (u16)arg1 | 0xE1000000;
                        p += sizeof(DR_TPAGE);
                    } else {
                        code &= 0x3C;
                    }
                    switch (code) {
                    case 0x2C:
                        setSemiTrans(p, 1);
                        ((POLY_FT4 *)p)->tpage = arg1 | (((POLY_FT4 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_FT4);
                        break;
                    case 0x24:
                        setSemiTrans(p, 1);
                        ((POLY_FT3 *)p)->tpage = arg1 | (((POLY_FT3 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_FT3);
                        break;
                    case 0x3C:
                        setSemiTrans(p, 1);
                        ((POLY_GT4 *)p)->tpage = arg1 | (((POLY_GT4 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_GT4);
                        break;
                    case 0x34:
                        setSemiTrans(p, 1);
                        ((POLY_GT3 *)p)->tpage = arg1 | (((POLY_GT3 *)p)->tpage & ~0x60);
                        p += sizeof(POLY_GT3);
                        break;
                    case 0x38:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G4);
                        break;
                    case 0x30:
                        setSemiTrans(p, 1);
                        p += sizeof(POLY_G3);
                        break;
                    case 0x28:
                        p += sizeof(POLY_F4);
                        break;
                    case 0x20:
                        p += sizeof(POLY_F3);
                        break;
                    }
                } while (code != 0xFF);
            }
        }
    }
}

void btl_updateTexAnims(TexAnimSet *arg0) {
    RECT r;
    u32 i;
    u32 j;
    TexAnim *e;
    TexAnimRect *t;
    s32 pos;
    s16 frame;
    s16 last;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->count; i++) {
        e = &arg0->anims[i];
        if (!(e->flags & 1)) {
            continue;
        }
        if (e->frameCount != 0) {
            pos = e->pos;
            frame = pos >> 12;
            last = e->lastFrame;
            if (frame >= 0) {
                if (frame != last) {
                    for (j = 0; j < e->rectCount; j++) {
                        t = &e->rects[j];
                        r.x = t->src[frame][0];
                        r.y = t->src[frame][1];
                        r.w = t->w;
                        r.h = t->h;
                        MoveImage(&r, t->x, t->y);
                    }
                    e->lastFrame = frame;
                }
                pos += e->speed;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < e->frameCount) {
                e->pos = pos;
            } else if (e->waitRand != 0) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                e->pos = -((e->waitMin + (e->waitRand + 1) * ((*g_randPtr << 8) | *g_randPtr++) / 0x10000) << 12);
            } else if (e->flags & 2) {
                e->flags &= 0xFC;
            } else {
                e->pos = 0;
            }
        } else if (e->flags & 4) {
            pos = e->pos;
            frame = pos >> 12;
            for (j = 0; j < e->rectCount; j++) {
                t = &e->rects[j];
                MoveImage((RECT *)t, t->x, t->y - frame);
                r.x = t->x;
                r.y = t->y - frame;
                r.w = t->w;
                r.h = frame;
                MoveImage(&r, t->x, t->y + t->h - frame);
            }
            e->pos = (pos & 0xFFF) + e->speed;
        }
    }
}

void btl_relocTexAnims(TexAnimSet *set, u16 (*ofs)[4]) {
    u32 i;
    u32 j;
    u32 k;
    TexAnimRect *r;
    u16 *f;
    u16 *g;
    u8 idx;

    if (set == NULL) {
        return;
    }
    for (i = 0; i < set->count; i++) {
        set->anims[i].lastFrame = 1;
        f = (u16 *)set->anims[i].rects;
        if ((u32)f > 0x80000000) {
            return;
        }
        if (f == NULL) {
            continue;
        }
        idx = set->anims[i].relocIndex;
        f = (u16 *)((u8 *)f + (s32)set);
        set->anims[i].rects = (TexAnimRect *)f;
        if (ofs != NULL) {
            f[0] += ofs[idx][2];
            f[1] += ofs[idx][3];
        }
        r = (TexAnimRect *)f;
        for (j = 0; j < set->anims[i].rectCount; j++) {
            f = (u16 *)r[j].src;
            f = (u16 *)((u8 *)f + (s32)set);
            r[j].src = (void *)f;
            if (ofs != NULL) {
                for (k = 0; k < set->anims[i].frameCount; k++) {
                    g = f + 1;
                    *f += ofs[idx][0];
                    *g += ofs[idx][1];
                    f += 2;
                }
            }
        }
    }
}

Battle11C020 *btl_initScene(void) {
    Battle *b;
    Battle11C020 *s = g_gameState->gameData->ctx.battle->work->encounter;
    u16 *t = g_gameState->gameData->ctx.battle->work->sceneTables;
    s32 r;
    Battle132C *sc = &g_gameState->gameData->ctx.battle->scene;
    u8 n;
    u8 m;

    sc->seqTable = (u16 *)((u8 *)t + t[0]);
    sc->sceneData = (s16 *)((u8 *)t + t[1]);
    sc->group = btl_pickEnemyGroup(g_gameState->gameData->ctx.battle->work->encounter);
    sc->enemyTypes = (Battle1334 *)&g_gameState->gameData->ctx.battle->work->encounter->groups[s->groupCount];
    n = sc->group->cameraNo;
    b = g_gameState->gameData->ctx.battle;
    if (n >= 3) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        m = r % 3;
    } else {
        m = n;
    }
    b->cameraNo = m;
    return s;
}

Battle1330 *btl_pickEnemyGroup(Battle11C020 *arg0) {
    Battle1330 *e;
    u16 r;
    u16 sum;
    u16 i;
    s8 v;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = (*g_randPtr++ * 100) / 256;
    e = arg0->groups;
    sum = e->weight;
    i = 0;
    while (r >= sum) {
        e++;
        sum += e->weight;
        i++;
    }
    v = g_gameState->gameData->enemyGroup;
    if (v != -1) {
        i = v;
        if (i >= arg0->groupCount) {
            i = 0;
        }
        e = &arg0->groups[i];
    }
    g_gameState->gameData->ctx.battle->groupNo = i;
    return e;
}

void func_800C972C(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    b->scene.flags = (b->work->encounter->flags ^ 0x30) * 4;
    D_800FDF38 = g_gameState->gameData->ctx.battle->scene.group->ap;
}

void btl_setupEnemies(void) {
    Battle *b;
    Battle1330 *grp;
    Battle11C020 *hdr;
    s16 i;
    s16 k;
    u8 n;
    Unk800C9D1C *recs;
    Unk800C9D1C *rec;
    BtlEnemyType *en;
    U16Pair *ofs;
    BtlCmd8 *src;
    u32 *flags;
    BtlCmd8 *dst;
    BtlCmd8 *cmd;
    BtlUnit *u;
    Battle1330 *grp2;

    b = g_gameState->gameData->ctx.battle;
    flags = &b->scene.flags;
    grp = b->scene.group;
    hdr = b->work->encounter;
    b->scene.flags = (b->scene.flags & ~3) | (btl_rollAmbush(flags) & 3);
    i = 0;
    n = hdr->typeCount + hdr->abilityCount;
    grp2 = hdr->groups;
    recs = (Unk800C9D1C *)(grp2 + hdr->groupCount);
    rec = recs;
    en = g_gameState->gameData->ctx.battle->enemyTypes;
    ofs = (U16Pair *)(g_gameState->gameData->ctx.battle->work->texts + 4);
    src = (BtlCmd8 *)&recs[hdr->typeCount];
    for (; i < hdr->typeCount; i++) {
        btl_initEnemyType(rec, en);
        en->camera = btl_getSceneData(i + 5);
        en->nameOfs = ofs->unk0 + 4;
        ofs++;
        en->firstAbility = n;
        n += rec->abilityCount;
        en++;
        rec++;
    }
    dst = g_gameState->gameData->ctx.battle->enemyAbilities;
    for (i = 0, cmd = src; i < hdr->abilityCount; i++, cmd++) {
        btl_copyAbility(cmd, dst);
        dst->nameOfs = ofs->unk0 + 4;
        ofs++;
        dst++;
    }
    u = g_gameState->gameData->ctx.battle->unitList;
    for (i = 0; i < grp->enemyCount; i++) {
        if (u == NULL) {
            break;
        }
        btl_initEnemyUnit(&grp->enemies[i], u, (s32 *)flags, i);
        ((void (*)(Unk800C9D1C *, BtlUnit *, s32))btl_setEnemyStats)(&recs[grp->enemies[i].type], u, i);
        u = u->next;
    }
}

void btl_placeParty(void) {
    s16 n;
    s16 i;
    Battle *b;
    s32 x;
    BtlUnit *u;
    s16 rot;
    BtlUnit98 *p98;
    BtlUnitA0 *pa0;
    BtlUnit98 *q98;
    BtlUnitA0 *qa0;
    s32 w;
    s32 v;
    Unk800F57F0 *e;
    Unk800F57F0 *tbl;
    u32 *flags;
    u32 m;

    n = 0;
    b = g_gameState->gameData->ctx.battle;
    flags = &b->scene.flags;
    for (i = 0; i < 4; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            n++;
        }
    }
    x = (n - 1) * 0x13C;
    i = 0;
    rot = ((*flags & 3) != 0) << 11;
    u = g_gameState->gameData->ctx.battle->unitList;
    for (; i < n; i++) {
        if (!(u->flags & 1)) {
            return;
        }
        m = u->flags & ~0x300;
        m |= (g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.w >> 1) & 0x100;
        u->flags = m;
        if (!(*flags & 3)) {
            u->flags = (u->flags & ~0x300) | ((((u->flags >> 8) & 3) ^ 1) << 8);
        }
        u->pos[0] = u->home->pos[0] = u->obj->x = x << 16;
        p98 = u->home;
        pa0 = u->obj;
        if ((u8)btl_hasStatus(u, BTL_STATUS_FLOAT)) {
            v = 0xFF380000;
        } else {
            v = 0;
        }
        u->pos[1] = p98->pos[1] = pa0->y = v;
        q98 = u->home;
        qa0 = u->obj;
        w = 0xF9E80000;
        if (!((u->flags >> 8) & 3)) {
            w = 0xF8580000;
        }
        u->pos[2] = q98->pos[2] = qa0->z = w;
        u->obj->rotX = 0;
        u->rot[0] = 0;
        u->obj->rotY = rot;
        u->rot[1] = rot;
        u->obj->rotZ = 0;
        u->rot[2] = 0;
        tbl = D_800F57F0;
        e = &tbl[g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind];
        u->shadowBone = e->shadowBone;
        u->shadowDirBone = e->shadowDirBone;
        btl_setShadowSize(u, e->shadowW, e->shadowD);
        x -= 0x278;
        g_gameState->gameData->ctx.battle->work->shadows[(u->flags >> 1) & 0xF].offsetZ = e->shadowZ;
        u = u->next;
    }
}

void btl_initEnemyType(Unk800C9D1C *arg0, BtlEnemyType *arg1) {
    s16 i;
    BtlUnit9CC *h;
    arg1->radius = arg0->radius;
    arg1->category = arg0->category;
    arg1->level = arg0->level;
    arg1->eatAbility = arg0->eatAbility;
    arg1->max.maxHp = arg0->maxHp;
    arg1->max.maxMp = arg0->maxMp;
    arg1->gil = arg0->gil;
    arg1->exp = arg0->exp;
    arg1->card = arg0->card;
    arg1->drops.w = arg0->drops;
    for (i = 0; i < 6; i++) {
        h = db_lookupRegistered(4, arg0->motionIds[i] | 0x30000, 0);
        arg1->motions[i] = h;
        if (IS_OFFSET(h->rotKeys)) {
            h->rotKeys = (s32)h + h->rotKeys;
            if (h->rotKeysLo != 0) {
                h->rotKeysLo = (s32)arg1->motions[i] + h->rotKeysLo;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        arg1->bones[i] = arg0->bones[i];
    }
    arg1->deathCry = arg0->deathCry;
    arg1->confuseAbility = arg0->confuseAbility;
    for (i = 0; i < 6; i++) {
        arg1->iconBones[i] = arg0->iconBones[i];
        arg1->iconY[i] = arg0->iconY[i];
        arg1->iconZ[i] = arg0->iconZ[i];
    }
}

void btl_initEnemyUnit(Unk800C9EE0 *arg0, BtlUnit *arg1, s32 *arg2, s32 arg3) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlEnemy *s = &b->enemies[(arg1->flags >> 1) & 0xF];
    BtlEnemyType *e;
    s16 r;
    s->type = &b->enemyTypes[arg0->type];
    arg1->flags = (((arg1->flags & ~0x10000000) | ((arg0->flags & 1) << 28)) & ~0x300) | 0x200;
    e = &g_gameState->gameData->ctx.battle->enemyTypes[arg0->type];
    arg1->max = &e->max;
    arg1->hp = e->max.maxHp;
    arg1->mp = arg1->max->maxMp;
    s->flags = (s->flags & ~0x100) | ((arg0->flags << 7) & 0x100);
    if (!((s->flags >> 9) & 1)) {
        arg1->pos[0] = arg1->home->pos[0] = arg1->obj->x = arg0->x << 16;
        arg1->pos[1] = arg1->home->pos[1] = arg1->obj->y = arg0->y << 16;
        arg1->pos[2] = arg1->home->pos[2] = arg1->obj->z = arg0->z << 16;
        arg1->obj->rotX = 0;
        arg1->rot[0] = 0;
        arg1->obj->rotZ = 0;
        arg1->rot[2] = 0;
        r = (arg0->rotY + D_800F5850[*arg2 & 3]) & 0xFFF;
        arg1->obj->rotY = r;
        arg1->rot[1] = r;
    }
    arg1->motions = (s32 *)s->type->motions;
    arg1->model->motion = s->type->motions[0];
}

void btl_setEnemyStats(Unk800C9D1C *arg0, BtlUnit *arg1) {
    BtlEnemy *s;
    BtlShadow *t;

    arg1->statusImmune = arg0->statusImmune;
    arg1->autoStatus = arg0->autoStatus;
    arg1->status = arg0->status;
    arg1->hp = arg0->maxHp;
    arg1->mp = arg0->maxMp;
    arg1->defense = arg0->defence[0];
    arg1->evade = arg0->defence[1];
    arg1->magicDef = arg0->defence[2];
    arg1->magicEva = arg0->defence[3];
    arg1->speed = arg0->stats[0];
    arg1->strength = arg0->stats[1];
    arg1->magic = arg0->stats[2];
    arg1->spirit = arg0->stats[3];
    *(s32 *)&arg1->elemGuard = arg0->elements;
    *(s32 *)&arg1->hiddenMeshes = arg0->meshMasks;
    arg1->baseBone = arg0->baseBone;
    s = &g_gameState->gameData->ctx.battle->enemies[(arg1->flags >> 1) & 0xF];
    s->steal.w = arg0->steal;
    s->flags = (s->flags & ~0x40) | ((arg0->flags & 1) << 6);
    s->flags = (s->flags & ~0x80) | ((arg0->flags << 6) & 0x80);
    btl_setShadowSize(arg1, arg0->shadowW, arg0->shadowD);
    arg1->shadowBone = arg0->shadowBone;
    arg1->shadowDirBone = arg0->shadowDirBone;
    t = &g_gameState->gameData->ctx.battle->work->shadows[((arg1->flags >> 1) & 0xF) + 9];
    t->offsetX = arg0->shadowX;
    t->offsetZ = arg0->shadowZ;
}

void btl_copyAbility(BtlCmd8 *arg0, BtlCmd8 *arg1) {
    *arg1 = *arg0;
}

s32 btl_getSceneData(s32 arg0) {
    s16 *t = g_gameState->gameData->ctx.battle->scene.sceneData;
    return (s32)t + t[arg0];
}

s16 btl_getEnemyModelId(s16 arg0) {
    Unk800C9EE0 *e = &g_gameState->gameData->ctx.battle->scene.group->enemies[arg0];
    s32 r;
    if (arg0 > 0 && (e->flags & 2)) {
        r = -1;
    } else {
        r = g_gameState->gameData->ctx.battle->scene.enemyTypes[e->type].modelId;
    }
    return r;
}

u16 btl_getEntranceSound(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    return b->scene.enemyTypes[b->scene.group->enemies[0].type].entranceSound;
}

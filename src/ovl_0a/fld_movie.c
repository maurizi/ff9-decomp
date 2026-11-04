#include "ovl_0a.h"

s32 fld_queueModelBundle(s32 uid) {
    s32 id;

    id = fld_depFindByLink(uid);
    if (id == -1) {
        return 0;
    }
    id = fld_remapModelId(id);
    if (db_lookupRegistered(DB_GET_ADDR, id, 0) == NULL) {
        g_fieldModelQueue[g_gameState->gameData->ctx.field->area->scene->modelQueueCount] = id;
        g_gameState->gameData->ctx.field->area->scene->modelQueueCount++;
    }
    return 0;
}

void fld_clearModelQueue(void) {
    s32 i;

    for (i = 15; i >= 0; i--) {
        g_fieldModelQueue[i] = -1;
    }
}

s32 fld_loadModelBundle(s32 modelId) {
    FileEntry *file;
    s32 size;
    Unk800C9CCC *cache;
    s32 data;
    FieldVramPos *vp;
    s32 i;
    s32 j;
    s32 pack2;
    s32 pack1;
    u8 *hdr;
    s16 *tp;
    s16 *uv;
    FieldTexEnt *ent;
    s32 img;
    s32 x, y, cx, cy, tag;
    s32 e1;
    s32 cx2;
    s32 cy2;
    s32 x2;
    s32 y2;

    file = file_find(DIR_CHAR_MODELS, modelId);
    size = (file->link - file->sector) << 11;
    cache = fld_findModelCache(modelId);
    if (cache->addr == 0) {
        cdRead_init(g_gameState->gameData->cdRead, file->sector, size,
                      (void *)(fld_allocModelCache(modelId, size) != 0 ? cache->addr : g_modeHeap), 0, -1, 0);
        while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
        }
    }
    if (cache->addr != 0) {
        s32 copySize = file_copyViaVram(g_modeHeap, cache->addr, size, NULL);
        DrawSync(0);
        fld_invalidateModelCache(g_modeHeap + copySize);
    }
    vp = db_lookupRegistered(DB_GET_ADDR, DB_VRAM_ALLOC, 0);
    data = g_modeHeap;
    pack1 = db_lookup(data, DB_GET_AT, DB_PACK, 0);
    pack2 = db_lookup(data, DB_GET_AT, (DB_PACK | 1), 0);
    hdr = (u8 *)db_lookup(pack1, DB_GET_AT, DB_MODEL_INFO, 0) + 4;
    tp = (s16 *)(hdr + (*(u32 *)(hdr + 4) & 0xFFFFFF));
    uv = tp + hdr[7] * 2;
    ent = (FieldTexEnt *)(uv + hdr[7] * 2);
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, DB_GET_AT, (i & 0xFFFF) | DB_TIM, 0);
        e1 = ent[i].srcX;
        if (e1 & 1) {
            x = vp->pageX;
            y = vp->pageY;
            if (vp->pageY == 0x100) {
                vp->pageX += 0x40;
                vp->pageY = 0x180;
            } else {
                vp->pageY -= 0x80;
            }
            if (vp->pageX == vp->skipX) {
                vp->pageX += 0x40;
            }
            cy = vp->clutY;
            cx = vp->clutX;
            vp->clutY += 2;
            tp[i * 2] = getTPage(1, 0, x, y);
            tp[i * 2 + 1] = getClut(cx, cy);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y % 256;
            ent[i].destX = x;
            ent[i].destY = y;
            file_setTimPos(img, cx, cy, x, y);
            file_loadTim(img);
        }
    }
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, DB_GET_AT, (i & 0xFFFF) | DB_TIM, 0);
        tag = ent[i].srcX;
        if (!(tag & 1)) {
            x2 = vp->stripX;
            y2 = vp->stripY;
            if (vp->stripY >= 0x1E0) {
                vp->stripX -= 0x40;
                vp->stripY = 0x100;
            } else {
                vp->stripY += 0x20;
            }
            cy2 = vp->stripClutY;
            cx2 = vp->stripClutX;
            vp->stripClutY += 2;
            tp[i * 2] = getTPage(1, 0, x2, y2);
            tp[i * 2 + 1] = getClut(cx2, cy2);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y2 % 256 - ((tag - 2) % 8 * 32 + 0x100) % 128;
            ent[i].srcX = x2;
            ent[i].srcY = y2;
            for (j = hdr[7] - 1; j >= 0; j--) {
                if (ent[j].srcX == tag - 1) {
                    ent[i].destX = ent[j].destX;
                    ent[i].destY = ent[j].destY;
                    break;
                }
            }
            file_setTimPos(img, cx2, cy2, x2, y2);
            file_loadTim(img);
        }
    }
    DrawSync(0);
    g_gameState->gameData->ctx.field->area->scene->modelBundles[g_gameState->gameData->ctx.field->area->scene->modelBundleCount] = pack1;
    g_gameState->gameData->ctx.field->area->scene->modelBundleCount++;
    db_register(pack1, 0);
    g_modeHeap = pack1 + db_getSize(pack1);
    return 0;
}

s32 fld_freeModelBundles(void) {
    s32 i;

    for (i = g_gameState->gameData->ctx.field->area->scene->modelBundleCount - 1; i >= 0; i--) {
        db_unregister((void *)g_gameState->gameData->ctx.field->area->scene->modelBundles[i]);
    }
    g_gameState->gameData->ctx.field->area->scene->modelBundleCount = 0;
    return 0;
}

s32 fld_setupActorModel(s32 modelId, void *actor) {
    s32 *tex;
    s32 *uv;
    s32 *spr;
    s32 sprId;
    FieldObjSlot *fslot;
    s32 slot;
    u32 k;
    u32 sum;
    WmModel *mdl;
    WmModelGroup *grp;
    WmAnimTable *table;
    WmAnimTableEntry *entry;
    s32 i;
    u32 chrIdx;
    WmModel *anim;

    sprId = -1;
    chrIdx = evt_getObjectChrIndex(actor);
    if (chrIdx < 9) {
        slot = chrIdx;
        fslot = (FieldObjSlot *)game_getModel(chrIdx);
    } else {
        slot = g_gameState->gameData->npcModelSlots + 9;
        fslot = (FieldObjSlot *)game_getModel(slot);
        g_gameState->gameData->npcModelSlots++;
    }
    modelId = fld_remapModelId(modelId);
    fslot->modelId = modelId;
    fslot->actor = actor;
    fslot->model->model = db_lookupRegistered(DB_GET_ADDR, modelId, 0);
    if ((fslot->model->anim = db_lookupRegistered(DB_GET_ADDR, fld_getModelAnimId(modelId), 0)) != NULL) {
        anim = fslot->model->anim;
        if (IS_OFFSET(anim->bones)) {
            anim->bones = (s32)anim + anim->bones;
            if (anim->groups != NULL) {
                anim->groups = (WmModelGroup *)((u8 *)fslot->model->anim + (s32)anim->groups);
            }
        }
    }
    fld_getModelTables(modelId, &tex, &uv, &spr);
    mdl = fslot->model->model;
    grp = mdl->groups;
    sum = 0;
    if (IS_OFFSET(grp)) {
        grp = (WmModelGroup *)((u8 *)mdl + (s32)grp);
    }
    for (k = 0; k < mdl->groupCount; k++) {
        sum += grp->packetSize;
        grp++;
    }
    sum *= 2;
    sum += fslot->model->model->boneCount * 32 + fslot->model->model->groupCount * 12;
    fldmdl_initModel(fslot->model, (u8 *)g_gameState->gameData->ctx.field->area->scene->heapTop);
    fldmdl_buildPackets(fslot->model, (WmTexInfo *)tex, (s16(*)[2])uv);
    g_gameState->gameData->ctx.field->area->scene->heapTop += sum;
    fld_trimModelCache();
    table = PSX_NULLABLE(db_lookupRegistered(DB_GET_AT_IN, modelId, DB_MODEL_INFO));
    entry = table->entries;
    for (i = table->count - 1; i >= 0; i--) {
        if (entry->modelId == (u16)modelId) {
            sprId = entry->texAnimId | DB_TEX_ANIM;
        }
    }
    if (sprId != -1) {
        fslot->model->texAnims = db_lookupRegistered(DB_GET_ADDR, sprId, 0);
        fldmdl_initTexAnims((TexAnimRelocSet *)fslot->model->texAnims, (ModelTexOfs *)spr);
    }
    if (g_gameState->gameData->ctx.field->area->scene->walkmesh != NULL) {
        if (slot < 9) {
            bgi_bindActor(slot, fslot);
        } else {
            bgi_addActor(fslot);
        }
        bgi_setActorActive(slot, 1);
    }
    g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_ACTORS_READY;
    fslot->flags |= FLD_OBJ_LOADED;
    return slot;
}

s32 fld_remapModelId(s32 modelId) {
    if ((modelId & 0xFFFF) == 0xB9 && g_evtGlobalVars.h[0] >= 0x283C) {
        modelId = 0x2004C;
    }
    return modelId;
}

s32 fld_depFindByLink(s32 link) {
    FileTable *table = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *entries = table->entries;

    for (i = table->count - 1; i >= 0; i--) {
        if (entries[i].link == link) {
            return entries[i].id;
        }
    }
    return -1;
}

s32 fld_getModelAnimId(s32 modelId) {
    WmAnimTable *table = PSX_NULLABLE(db_lookupRegistered(DB_GET_AT_IN, modelId, DB_MODEL_INFO));
    s32 i;
    WmAnimTableEntry *entry;

    entry = table->entries;
    for (i = table->count - 1; i >= 0; i--) {
        if (entry->modelId == (u16)modelId) {
            return entry->animId | DB_ANIM;
        }
    }
    return -1;
}

/* look up the three tables of an area entry */
s32 fld_getModelTables(s32 modelId, s32 **outTex, s32 **outUv, s32 **outSpr) {
    WmAnimTable *table = PSX_NULLABLE(db_lookupRegistered(DB_GET_AT_IN, modelId, DB_MODEL_INFO));
    WmAnimTableEntry *entry = table->entries;
    s32 i;

    for (i = table->count - 1; i >= 0; i--) {
        if (entry->modelId == (u16)modelId) {
            *outTex = (s32 *)((u8 *)entry + entry->tableOffset);
            *outUv = *outTex + entry->texCount;
            *outSpr = *outUv + entry->texCount;
            break;
        }
        entry++;
    }
    return 0;
}

s32 fld_isModelTexAnimActive(ModelObj *mdl) {
    Unk800AB0B0 *texSet = mdl->texAnims;
    s32 i;
    Unk800AB0B0Entry *anims;

    if (texSet != NULL) {
        anims = texSet->anims;
        for (i = texSet->count - 1; i >= 0; i--) {
            if (anims[i].flags & TEXANIM_ACTIVE) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fld_collectVisibleModels(FieldObjSlot **out) {
    s32 count;
    EvtListNode *node;
    FieldObjSlot *slot;
    s32 i;
    WmModel *mdl;
    WmModelGroup *grp;
    u32 sum;
    u32 k;
    ModelObj *obj;
    EvtObject *actor;
    u16 *parent;

    count = 0;
    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        actor = node->obj;
        if (actor->state == EVT_STATE_RUNNING && (actor->flags & EVT_OBJ_ENABLED) && actor->kind == EVT_KIND_ACTOR) {
            slot = (FieldObjSlot *)game_getModel(actor->modelSlot);
            if (slot->model->parent == NULL) {
                out[count++] = slot;
            }
        }
    }
    for (node = evt_getObjectList(); node != NULL; node = node->next) {
        actor = node->obj;
        if (actor->state == EVT_STATE_RUNNING && (actor->flags & EVT_OBJ_ENABLED) && actor->kind == EVT_KIND_ACTOR) {
            slot = (FieldObjSlot *)game_getModel(actor->modelSlot);
            parent = slot->model->parent;
            if (parent != NULL) {
                for (i = 0; i < count; i++) {
                    if ((u16 *)out[i]->model == parent) {
                        out[count++] = slot;
                        break;
                    }
                }
            }
        }
    }
    if (g_gameState->gameData->ctx.field->area->scene->movieState != FLD_MOVIE_IDLE) {
        for (i = 0; i < count; i++) {
            obj = out[i]->model;
            mdl = obj->model;
            grp = mdl->groups;
            sum = 0;
            if (IS_OFFSET(grp)) {
                grp = (WmModelGroup *)((u8 *)mdl + (s32)grp);
            }
            for (k = 0; k < mdl->groupCount; k++) {
                sum += grp->packetSize;
                grp++;
            }
            if (obj->packets + (sum * 2 + (obj->model->boneCount * 32 + obj->model->groupCount * 12)) >= g_loadAreaEnd + 0xF1700) {
                out[i] = NULL;
            }
        }
    }
    return count;
}

void fld_drawModels(void) {
    /* block scope: fld_applyModelCommands calls it undeclared (int argument) */
    void fldmdl_projectMesh(ModelObj *, u8);
    FieldObjSlot *list[16];
    SVECTOR rot;
    DVECTOR sxy;
    s32 ofs[2];
    s16 ax;
    s16 az;
    s32 z;
    u32 *ot;
    s32 count;
    s32 i;
    s32 j;
    s32 j1;
    s32 cnt;
    s32 lim;
    s32 bits; /* the slot's FLD_OBJ_CLIP_Y flag, then the mesh group's bit (separate locals don't match) */
    ModelObj *mdl;
    EvtObject *actor;
    FieldObjSlot *slot;
    WmModel *w;

    lim = 100;
    j = 0;
    if (g_fieldMovieFlags & FLD_MVF_CAMERA) {
        mbg_getGeomOffset(&ofs[0], &ofs[1]);
        SetGeomOffset(ofs[0], ofs[1]);
    } else {
        fldbg_applyGeomOffset();
    }
    i = 0;
    ot = g_gameState->gameData->ot[g_gameState->gameData->drawBuf];
    g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot + g_gameState->gameData->ctx.field->area->scene->otBias;
    count = fld_collectVisibleModels((FieldObjSlot **)list);
    for (; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            mdl = slot->model;
            actor = slot->actor;
            if ((slot->flags & FLD_OBJ_TILT) && bgi_getActorTilt(actor->modelSlot, &ax, &az)) {
                rot.vx = ax;
                rot.vy = actor->pos.rotY;
                rot.vz = az;
                math_rotMatrixZXY(&rot, &mdl->worldMatrix);
            } else {
                rot.vx = actor->pos.rotX;
                rot.vy = (actor->pos.rotY + j) & 0xFFF;
                rot.vz = actor->rotZ;
                math_rotMatrixZYX(&rot, &mdl->worldMatrix);
            }
            mdl->worldMatrix.t[0] = actor->pos.vx;
            mdl->worldMatrix.t[1] = actor->pos.vy;
            mdl->worldMatrix.t[2] = actor->pos.vz;
            fldmdl_applyScale(mdl);
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            actor = slot->actor;
            mdl = slot->model;
            w = db_lookupRegistered(DB_GET_ADDR, actor->anim | DB_ANIM, 0);
            mdl->anim = w;
            if (w != NULL && IS_OFFSET(w->bones)) {
                w->bones = (void *)((u8 *)w + (s32)w->bones);
                if (w->groups != NULL) {
                    w->groups = (void *)((u8 *)mdl->anim + (s32)w->groups);
                }
            }
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            mdl = slot->model;
            actor = slot->actor;
            if (mdl->flags & MDL_LOOK_AT) {
                fldmdl_calcBoneMatricesLookAt(mdl, actor->animFrame);
            } else {
                fldmdl_calcBoneMatrices(mdl, actor->animFrame);
            }
        }
    }
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            mdl = slot->model;
            rot.vx = ((EvtObject *)mdl->boneMatrices)->pos.vx;
            rot.vy = ((EvtObject *)mdl->boneMatrices)->pos.vy;
            rot.vz = ((EvtObject *)mdl->boneMatrices)->pos.vz;
            gte_ldv0(&rot);
            gte_rtps();
            gte_stsxy(&sxy);
            gte_stsz(&z);
            z = z / 4 + g_gameState->gameData->ctx.field->area->scene->otBias;
            if (z < lim || z > 0x1000 - lim) {
                list[i] = NULL;
            }
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            mdl = slot->model;
            bits = slot->flags & FLD_OBJ_CLIP_Y;
            lim = mdl->model->groupCount;
            SetSpadStack12(PSX_SPAD_BASE + 0x3FC);
            if (bits) {
                for (j = 0; j < lim; j++) {
                    bits = 1 << j;
                    if (!(mdl->hiddenGroups & (u32)bits)) {
                        fldmdl_projectMeshClipY(mdl, j);
                        fldmdl_drawMeshClipY(mdl, j);
                    }
                }
            } else {
                cnt = lim;
                for (j1 = 0; j1 < cnt; j1++) {
                    if (!(mdl->hiddenGroups & (u32)(1 << j1))) {
                        fldmdl_projectMesh(mdl, j1);
                        fldmdl_drawMesh(mdl, j1);
                    }
                }
            }
            ResetSpadStack12();
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            fld_drawShadow(g_gameState->gameData->ctx.field->area->scene->shadows, slot->actor->modelSlot);
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL && !(slot->flags & FLD_OBJ_NO_IDLE_TEXANIM)) {
            mdl = slot->model;
            if (PSX_NULLABLE(mdl->texAnims)->count >= 2 && fld_isModelTexAnimActive(mdl) == 0) {
                PSX_NULLABLE(mdl->texAnims)->anims[2].flags |= TEXANIM_ACTIVE;
                PSX_NULLABLE(mdl->texAnims)->anims[2].pos = 0;
                PSX_NULLABLE(mdl->texAnims)->anims[2].lastFrame = 0x1000;
            }
        }
    }
    for (i = 0; i < count; i++) {
        slot = list[i];
        if (slot != NULL) {
            fldmdl_updateTexAnims((TexAnimSet *)((ModelObj *)slot->model)->texAnims);
        }
    }
    fld_applyLights(list, count);
    if (!(g_gameState->gameData->flags & GD_MENU_OPEN)) {
        fld_playAnimSfx(list, count);
    }
    fld_applyModelCommands(list, count);
    g_gameState->gameData->ot[g_gameState->gameData->drawBuf] = ot;
    if (!(g_fieldMovieFlags & FLD_MVF_CAMERA)) {
        fldbg_resetGeomOffset();
    }
}

/* model command cmd (FLD_MDLCMD_*) for modelSlot: semi-transparency, Y clip, mirror, animation sounds */
s32 fld_modelCommand(s32 modelSlot, s32 cmd, s32 param1, s32 param2, s32 param3) {
    FieldActorInfo *info;
    FieldObjSlot *slot;
    FieldActorExt *x;
    FieldActorExt *y;
    s32 *tex;
    s32 *uv;
    s32 *spr;
    u32 k;
    u32 sum;
    WmModel *mdl;
    WmModelGroup *grp;
    s32 id;
    FieldAnimSfx *sfx;

    info = &g_gameState->gameData->ctx.field->area->scene->actorInfo[modelSlot];
    slot = (FieldObjSlot *)game_getModel(modelSlot);
    switch (cmd) {
    case FLD_MDLCMD_SEMITRANS:
        if ((u8)param1 == 0xFF) {
            slot->flags &= ~FLD_OBJ_SEMITRANS;
        } else {
            if (param1 != (s8)info->abr) {
                slot->flags |= FLD_OBJ_ABR_CHANGED;
            }
            slot->flags |= FLD_OBJ_SEMITRANS;
        }
        info->abr = param1;
        break;
    case FLD_MDLCMD_CLIP_Y:
        if (param1 != 0) {
            ((ModelObj *)slot->model)->clipY = param2;
            slot->flags |= FLD_OBJ_CLIP_Y;
        } else {
            slot->flags &= ~FLD_OBJ_CLIP_Y;
        }
        break;
    case FLD_MDLCMD_MIRROR:
    case FLD_MDLCMD_MIRROR_POINT:
    case FLD_MDLCMD_MIRROR_NORMAL:
    case FLD_MDLCMD_MIRROR_TINT:
        x = info->mirror;
        if (x == NULL) {
            x = info->mirror = (FieldActorExt *)g_gameState->gameData->ctx.field->area->scene->heapTop;
            x->flags = 0;
            x->modelId = slot->modelId;
            x->model = (ModelObj *)x->modelBuf;
            x->actor = (EvtObject *)x->objBuf;
            x->unk10 = 0;
            x->src = slot;
            x->mirrorPoint.v.vx = 0;
            x->mirrorPoint.v.vy = 0;
            x->mirrorPoint.v.vz = 0;
            x->mirrorNormal.v.vx = 0;
            x->mirrorNormal.v.vy = 0x1000;
            x->mirrorNormal.v.vz = 0;
            x->tint.w = 0;
            g_gameState->gameData->ctx.field->area->scene->heapTop += sizeof(FieldActorExt);
        }
        y = x;
        if (!(x->flags & FLD_EXT_BUILT)) {
            *(FieldEvtObj *)y->actor = *(FieldEvtObj *)slot->actor;
            y->model->model = ((ModelObj *)slot->model)->model;
            y->model->anim = ((ModelObj *)slot->model)->anim;
            fld_getModelTables(y->modelId, &tex, &uv, &spr);
            mdl = y->model->model;
            grp = mdl->groups;
            sum = 0;
            if (IS_OFFSET(grp)) {
                grp = (WmModelGroup *)((u8 *)mdl + (s32)grp);
            }
            for (k = 0; k < mdl->groupCount; k++) {
                sum += grp->packetSize;
                grp++;
            }
            sum *= 2;
            sum += y->model->model->boneCount * 32 + y->model->model->groupCount * 12;
            fldmdl_initModel(y->model, (u8 *)g_gameState->gameData->ctx.field->area->scene->heapTop);
            fldmdl_buildPackets(y->model, (WmTexInfo *)tex, (s16(*)[2])uv);
            g_gameState->gameData->ctx.field->area->scene->heapTop += sum;
            id = (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, y->modelId, DB_TEX_ANIM);
            if (id != -1) {
                y->model->texAnims = db_lookupRegistered(DB_GET_ADDR, id, 0);
                fldmdl_initTexAnims((TexAnimRelocSet *)y->model->texAnims, (ModelTexOfs *)spr);
            }
            fld_trimModelCache();
            x->flags |= FLD_EXT_BUILT_UNREAD | FLD_EXT_BUILT;
        }
        switch (cmd) {
        case FLD_MDLCMD_MIRROR:
            if (param1 != 0) {
                y->model->mirrorSrc = (ModelObj *)slot->model;
                y->model->mirrorNormal.w[0] = x->mirrorNormal.w[0];
                y->model->mirrorNormal.w[1] = x->mirrorNormal.w[1];
                y->model->mirrorPoint.w[0] = x->mirrorPoint.w[0];
                y->model->mirrorPoint.w[1] = x->mirrorPoint.w[1];
                x->flags |= FLD_EXT_MIRROR;
            } else {
                x->flags &= ~FLD_EXT_MIRROR;
            }
            break;
        case FLD_MDLCMD_MIRROR_POINT:
            x->mirrorPoint.v.vx = param1;
            x->mirrorPoint.v.vy = param2;
            x->mirrorPoint.v.vz = param3;
            break;
        case FLD_MDLCMD_MIRROR_NORMAL:
            x->mirrorNormal.v.vx = param1;
            x->mirrorNormal.v.vy = param2;
            x->mirrorNormal.v.vz = param3;
            break;
        case FLD_MDLCMD_MIRROR_TINT:
            x->tint.c[0] = param1;
            x->tint.c[1] = param2;
            x->tint.c[2] = param3;
            x->tint.c[3] = 2;
            break;
        }
        break;
    case FLD_MDLCMD_ANIM_SE:
    case FLD_MDLCMD_ANIM_SE_ALT:
    case FLD_MDLCMD_ANIM_SE_RANDOM_PITCH:
    case FLD_MDLCMD_ANIM_SE_REMOVE:
        sfx = fld_findAnimSfx(slot, param1, param2);
        if (sfx == NULL && cmd != FLD_MDLCMD_ANIM_SE_REMOVE) {
            sfx = fld_addAnimSfx(slot, param1, param2);
        }
        switch (cmd) {
        case FLD_MDLCMD_ANIM_SE:
            sfx->sound[0] = param3;
            break;
        case FLD_MDLCMD_ANIM_SE_ALT:
            sfx->sound[1] = param3;
            break;
        case FLD_MDLCMD_ANIM_SE_RANDOM_PITCH:
            sfx->randomPitch = param3 != 0;
            break;
        case FLD_MDLCMD_ANIM_SE_REMOVE:
            fld_removeAnimSfx(slot, param1, param2);
            break;
        }
        break;
    }
    return 0;
}

void fld_applyModelCommands(FieldObjSlot **slots, s32 count) {
    s32 i;
    s32 j;
    FieldObjSlot *slot;
    FieldActorInfo *info;
    FieldActorExt *x;
    u32 vis;
    u32 chg;
    u32 f4;
    u32 f2;
    s32 upd;
    s32 grpCount;
    FieldActorExt *y;

    for (i = count - 1; i >= 0; i--) {
        slot = slots[i];
        if (slot == NULL) {
            continue;
        }
        upd = 0;
        info = &g_gameState->gameData->ctx.field->area->scene->actorInfo[slot->actor->modelSlot];
        vis = slot->flags & FLD_OBJ_SEMITRANS;
        chg = (info->flags & FLD_INFO_SEMITRANS) ^ vis;
        f4 = slot->flags & FLD_OBJ_ABR_CHANGED;
        f2 = slot->flags & FLD_OBJ_SEMITRANS_AGAIN;
        if (chg || f4 || f2) {
            upd = 1;
        }
        if (upd) {
            if (slot->flags & FLD_OBJ_SEMITRANS) {
                fldmdl_setSemiTrans(slot->model, 1);
            } else {
                fldmdl_setSemiTrans(slot->model, 0);
            }
            if (vis) {
                fldmdl_setAbr(slot->model, ((s32)(info->abr << 24) >> 19) & 0xFFE0);
            }
            if (slot->flags & FLD_OBJ_SEMITRANS_AGAIN) {
                slot->flags &= ~FLD_OBJ_SEMITRANS_AGAIN;
            }
            slot->flags &= ~FLD_OBJ_ABR_CHANGED;
            if (chg || f4) {
                slot->flags |= FLD_OBJ_SEMITRANS_AGAIN;
            }
            info->flags = (info->flags & ~FLD_INFO_SEMITRANS) | vis;
        }
    }
    for (i = count - 1; i >= 0; i--) {
        slot = slots[i];
        if (slot == NULL) {
            continue;
        }
        info = &g_gameState->gameData->ctx.field->area->scene->actorInfo[slot->actor->modelSlot];
        x = info->mirror;
        if (x != NULL && (x->flags & FLD_EXT_MIRROR)) {
            fldmdl_reflectModel(x->model);
            y = x;
            fldmdl_calcBoneMatrices(x->model, x->src->actor->animFrame);
            grpCount = x->model->model->groupCount;
            for (j = 0; j < grpCount; j++) {
                fldmdl_projectMesh(y->model, j & 0xFF);
                fldmdl_drawMeshMirrored(y->model, j & 0xFF);
            }
            if (x->tint.c[3] != 0) {
                for (j = grpCount - 1; j >= 0; j--) {
                    fldmdl_setGroupRgb(y->model, j & 0xFF, x->tint.c[0], x->tint.c[1], x->tint.c[2]);
                }
                x->tint.c[3]--;
            }
        }
    }
}

Unk800C9CCC *fld_findModelCache(s32 modelId) {
    u32 id;
    s32 i;

    id = (u16)modelId;
    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].id == id) {
            return &g_fieldModelCache[i];
        }
    }
    return NULL;
}

u32 fld_getModelCacheBottom(Unk800C9CCC **outEntry) {
    u32 min;
    s32 i;

    min = (s32)RAM_SUB_OVL(u8 *);
    if (outEntry != NULL) {
        *outEntry = NULL;
    }
    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].addr != 0 && g_fieldModelCache[i].addr < min) {
            min = g_fieldModelCache[i].addr;
            if (outEntry != NULL) {
                *outEntry = &g_fieldModelCache[i];
            }
        }
    }
    return min;
}

s32 fld_allocModelCache(s32 modelId, s32 size) {
    Unk800C9CCC *entry;
    u32 lo;
    u32 min;
    u32 addr;

    entry = fld_findModelCache(modelId);
    min = fld_getModelCacheBottom(NULL);
    lo = g_gameState->gameData->ctx.field->area->scene->heapTop;
    if (lo == 0) {
        lo = g_modeHeap;
    }
    addr = min - size;
    if (lo < addr) {
        entry->addr = addr;
        entry->size = size;
        return 1;
    }
    return 0;
}

/* drop the cached textures no longer listed and pack the rest at the top of RAM */
void fld_compactModelCache(void) {
    u32 top;
    Unk800C9CCC *entry;
    s32 i;
    s32 j;
    s32 found;
    s32 bi;
    u32 end;
    u32 best;
    u32 addr;
    s32 id;
    u32 sz;
    Unk800C9CCC *cand;

    top = (s32)RAM_SUB_OVL(u8 *);
    for (i = 9; i >= 0; i--) {
        entry = &g_fieldModelCache[i];
        id = entry->id;
        if (entry->addr != 0) {
            found = 0;
            for (j = g_gameState->gameData->ctx.field->area->scene->modelQueueCount - 1; j >= 0; j--) {
                if ((u16)g_fieldModelQueue[j] == id) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                entry->addr = 0;
                entry->size = 0;
            }
        }
    }
    for (;;) {
        best = 0;
        bi = -1;
        for (i = 9; i >= 0; i--) {
            cand = &g_fieldModelCache[i];
            sz = cand->size;
            end = cand->addr + sz;
            if (cand->addr != 0 && end <= top && best < end) {
                best = end;
                bi = i;
            }
        }
        if (bi == -1) {
            break;
        }
        entry = &g_fieldModelCache[bi];
        addr = top - entry->size;
        if (entry->addr != addr) {
            file_copyViaVram(addr, entry->addr, entry->size, NULL);
            entry->addr = addr;
        }
        top = addr;
    }
    DrawSync(0);
}

void fld_clearModelCache(void) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        g_fieldModelCache[i].unk0 = 0;
        g_fieldModelCache[i].addr = 0;
        g_fieldModelCache[i].size = 0;
    }
}

void fld_trimModelCache(void) {
    s32 top = g_gameState->gameData->ctx.field->area->scene->heapTop;

    if (top == 0) {
        top = g_modeHeap;
    }
    fld_invalidateModelCache(top);
}

void fld_invalidateModelCache(s32 addr) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].addr != 0 && (u32)addr >= g_fieldModelCache[i].addr) {
            g_fieldModelCache[i].size = 0;
            g_fieldModelCache[i].addr = 0;
        }
    }
}

void fld_drawShadow(FieldObjPlace *places, s32 slot) {
    MATRIX mtx;
    POLY_FT4 *polys[4];
    SVECTOR corner;
    s32 z[9];
    s32 bias;
    FieldObjPlace *place;
    FieldObjSlot *fslot;
    void *lt;
    Unk800B3294Bias *light;
    u32 *stpPrim;
    u32 *mk;
    s32 i;
    s32 k;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    place = &places[slot];
    fslot = (FieldObjSlot *)game_getModel(slot);
    bias = 0;
    if (fslot->flags & FLD_OBJ_NO_SHADOW) {
        return;
    }
    lt = g_gameState->gameData->ctx.field->area->scene->lights;
    if (lt != NULL) {
        light = (Unk800B3294Bias *)fld_findLightColor(lt, fslot->modelId);
        if (light == NULL) {
            light = (Unk800B3294Bias *)fld_findLightColor(lt, -1);
        }
        bias = light->shadowOtBias;
    }
    fld_calcShadowMatrix(&place->params, fslot, &mtx);
    u0 = (s16)(g_gameState->gameData->ctx.field->area->scene->shadowTexX % 64) * 2;
    v0 = (s16)(g_gameState->gameData->ctx.field->area->scene->shadowTexY % 256);
    for (i = 0; i < 4; i++) {
        stpPrim = NULL;
        if (g_gameState->gameData->ctx.field->flags & FLD_MAP_SHADOW_MASK) {
            stpPrim = place->prims[g_gameState->gameData->drawBuf][i];
            setlen(stpPrim, PRIM_LEN(DR_STP));
            stpPrim[1] = _get_mask(0, 1);
            stpPrim[2] = 0;
            polys[i] = (POLY_FT4 *)(stpPrim + 3);
        } else {
            polys[i] = place->prims[g_gameState->gameData->drawBuf][i];
        }
        setPolyFT4(polys[i]);
        setSemiTrans(polys[i], 1);
        setRGB0(polys[i], place->params.shade, place->params.shade, place->params.shade);
        polys[i]->tpage = getTPage(1, 2, g_gameState->gameData->ctx.field->area->scene->shadowTexX,
                               g_gameState->gameData->ctx.field->area->scene->shadowTexY);
        polys[i]->clut = getClut(VRAM_SHADOW_CLUT_X, VRAM_SHADOW_CLUT_Y);
        switch (i) {
        case 0:
            setUV4(polys[0], u0, v0, u0 + 31, v0, u0, v0 + 31, u0 + 31, v0 + 31);
            break;
        case 1:
            setUV4(polys[1], u0 + 31, v0, u0, v0, u0 + 31, v0 + 31, u0, v0 + 31);
            break;
        case 2:
            setUV4(polys[2], u0, v0 + 31, u0 + 31, v0 + 31, u0, v0, u0 + 31, v0);
            break;
        case 3:
            setUV4(polys[3], u0 + 31, v0 + 31, u0, v0 + 31, u0 + 31, v0, u0, v0);
            break;
        }
        if (g_gameState->gameData->ctx.field->flags & FLD_MAP_SHADOW_MASK) {
            mk = (u32 *)(polys[i] + 1);
            setlen(mk, PRIM_LEN(DR_STP));
            mk[1] = _get_mask(0, 1);
            mk[2] = 0;
            MargePrim(polys[i], mk);
            MargePrim(stpPrim, polys[i]);
        }
    }
    gte_MulMatrix0(&g_gameState->gameData->viewMatrix, &mtx, &mtx);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldlv0(mtx.t);
    gte_rtv0tr();
    gte_stir(mtx.t);
    gte_SetRotMatrix(&mtx);
    gte_SetTransMatrix(&mtx);
    FIELD_SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&polys[0]->x0);
    gte_stsz(&z[0]);
    FIELD_SHADOW_PT(0, 0x1000);
    gte_stsxy(&polys[0]->x1);
    gte_stsxy(&polys[1]->x0);
    gte_stsz(&z[1]);
    FIELD_SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&polys[1]->x1);
    gte_stsz(&z[2]);
    FIELD_SHADOW_PT(-0x1000, 0);
    gte_stsxy(&polys[0]->x2);
    gte_stsxy(&polys[2]->x0);
    gte_stsz(&z[3]);
    FIELD_SHADOW_PT(0, 0);
    gte_stsxy(&polys[0]->x3);
    gte_stsxy(&polys[1]->x2);
    gte_stsxy(&polys[2]->x1);
    gte_stsxy(&polys[3]->x0);
    gte_stsz(&z[4]);
    FIELD_SHADOW_PT(0x1000, 0);
    gte_stsxy(&polys[1]->x3);
    gte_stsxy(&polys[3]->x1);
    gte_stsz(&z[5]);
    FIELD_SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&polys[2]->x2);
    gte_stsz(&z[6]);
    FIELD_SHADOW_PT(0, -0x1000);
    gte_stsxy(&polys[2]->x3);
    gte_stsxy(&polys[3]->x2);
    gte_stsz(&z[7]);
    FIELD_SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&polys[3]->x3);
    gte_stsz(&z[8]);

    k = z[0];
    if (k < z[1]) {
        k = z[1];
    }
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[4]) {
        k = z[4];
    }
    FIELD_SHADOW_ADD(k, 0);
    k = z[1];
    if (k < z[2]) {
        k = z[2];
    }
    if (k < z[4]) {
        k = z[4];
    }
    if (k < z[5]) {
        k = z[5];
    }
    FIELD_SHADOW_ADD(k, 1);
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    FIELD_SHADOW_ADD(k, 2);
    k = z[4];
    if (k < z[5]) {
        k = z[5];
    }
    if (k < z[7]) {
        k = z[7];
    }
    if (k < z[8]) {
        k = z[8];
    }
    FIELD_SHADOW_ADD(k, 3);
}

s32 fld_initShadows(s32 heap, s32 count, s32 u, s32 v) {
    return shadow_initSlots(heap, 0x100, 0xFF, u, v, count, 3);
}

/* object placement matrix: either a plain y rotation or facing the model's joint */
void fld_calcShadowMatrix(FieldObjEnt *ent, FieldObjSlot *obj, MATRIX *out) {
    MATRIX mtx;
    SVECTOR axis;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;
    s32 *dst;
    s32 *src;
    s32 cs;
    s32 sn;

    if (obj->flags & FLD_OBJ_SHADOW_FIXED) {
        cs = math_cos(ent->angle);
        sn = math_sin(ent->angle);
        out->m[0][0] = cs * ent->scaleX >> 12;
        out->m[1][0] = 0;
        out->m[2][0] = -sn * ent->scaleX >> 12;
        out->m[0][1] = 0;
        out->m[1][1] = ONE;
        out->m[2][1] = 0;
        out->m[0][2] = sn * ent->scaleZ >> 12;
        out->m[1][2] = 0;
        out->m[2][2] = cs * ent->scaleZ >> 12;
        out->t[0] = ((cs * ent->offsetX + sn * ent->offsetZ) >> 12) + ((EvtObject *)obj->actor)->pos.vx;
        out->t[1] = ((EvtObject *)obj->actor)->pos.vy;
        out->t[2] = ((-sn * ent->offsetX + cs * ent->offsetZ) >> 12) + ((EvtObject *)obj->actor)->pos.vz;
    } else {
        /* word-wise copy of the joint matrix */
        dst = (s32 *)&mtx;
        src = (s32 *)&((MATRIX *)obj->model->boneMatrices)[1];
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        mtx.t[2] = 0;
        mtx.t[1] = 0;
        mtx.t[0] = 0;
        axis.vx = 0;
        axis.vy = -0x1000;
        axis.vz = 0;
        gte_SetRotMatrix(&mtx);
        gte_SetTransMatrix(&mtx);
        gte_ldv0(&axis);
        gte_rtv0tr();
        gte_stlvnl(&dir);
        dir.vy = 0;
        VectorNormal(&dir, &fwd);
        up.vy = 0x1000;
        up.vx = 0;
        up.vz = 0;
        fwd.vx = -fwd.vx;
        fwd.vy = -fwd.vy;
        fwd.vz = -fwd.vz;
        gte_ldopv1(&up);
        gte_ldopv2(&fwd);
        gte_op12();
        gte_stlvnl(&side);
        out->m[0][0] = side.vx * ent->scaleX >> 12;
        out->m[1][0] = side.vy * ent->scaleX >> 12;
        out->m[2][0] = side.vz * ent->scaleX >> 12;
        out->m[0][1] = up.vx;
        out->m[1][1] = up.vy;
        out->m[2][1] = up.vz;
        out->m[0][2] = fwd.vx * ent->scaleZ >> 12;
        out->m[1][2] = fwd.vy * ent->scaleZ >> 12;
        out->m[2][2] = fwd.vz * ent->scaleZ >> 12;
        out->t[0] = ((side.vx * ent->offsetX + fwd.vx * ent->offsetZ) >> 12) + ((MATRIX *)obj->model->boneMatrices)[1].t[0];
        out->t[1] = ((side.vy * ent->offsetX + fwd.vy * ent->offsetZ) >> 12) + ((EvtObject *)obj->actor)->pos.vy;
        out->t[2] = ((side.vz * ent->offsetX + fwd.vz * ent->offsetZ) >> 12) + ((MATRIX *)obj->model->boneMatrices)[1].t[2];
    }
}

/* field request dispatcher */
s32 fld_movieCommand(s32 cmd, s32 param1, s32 param2) {
    switch (cmd & 0xF) {
    case 0:
        fld_movieStart((param1 >> 8) & 0xFF, param1 & 0xFF, param2);
        return 0;
    case 1:
        fld_movieStop();
        return 0;
    case 2:
        g_fieldMovieFlags |= FLD_MVF_PLAY;
        break;
    case 3:
        return fld_movieCommand_state();
    case 4:
        return mbg_getFrame();
    case 5:
        g_gameState->gameData->ctx.field->area->scene->movieRgb24 = param1;
        mbg_setRgb24(param1);
        break;
    }
    return 0;
}

void fld_movieStart(s32 list, s32 entry, s32 rgb24) {
    g_fieldMovieFlags = 0;
    g_gameState->gameData->ctx.field->area->scene->movieList = list;
    g_gameState->gameData->ctx.field->area->scene->movieEntry = entry;
    g_gameState->gameData->ctx.field->area->scene->movieRgb24 = rgb24;
    fld_clearModelCache();
    preload_reset();
    g_gameState->gameData->ctx.field->flags |= FLD_MAP_NO_PRELOAD;
    g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_LOAD_OVL;
}

void fld_movieUpdate(void) {
    s32 queueIdx;
    s32 done;
    s32 lastFrame;
    s32 remain;

    switch (g_gameState->gameData->ctx.field->area->scene->movieState) {
    case FLD_MOVIE_LOAD_OVL:
        if (cd_getState() == 0) {
            if (cdRead_update(g_gameState->gameData->cdRead) == 0) {
                FileEntry *file = file_find(DIR_OVERLAYS, OVL_MOVIE);
                cdRead_init(g_gameState->gameData->cdRead, file->sector, (file->link - file->sector) << 11,
                              (void *)(g_loadAreaEnd + 0xF1700), 0, 0x15000, NULL);
            }
            g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_READ_OVL;
        }
        break;
    case FLD_MOVIE_READ_OVL:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0) {
            g_fieldMovieFlags |= FLD_MVF_FLUSH_CACHE;
            g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_FIND;
        }
        break;
    case FLD_MOVIE_FIND:
        if (!(g_fieldMovieFlags & FLD_MVF_FLUSH_CACHE)) {
            g_gameState->gameData->ctx.field->area->scene->movieFile = cd_findFile(
                g_mbgMovieLists[g_gameState->gameData->ctx.field->area->scene->movieList]
                    .movies[g_gameState->gameData->ctx.field->area->scene->movieEntry].path);
            if (cd_getState() == 0) {
                cd_seekStreamAsync(g_gameState->gameData->ctx.field->area->scene->movieFile);
                g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_SEEK;
            }
        }
        break;
    case FLD_MOVIE_SEEK:
        if (cd_getState() == 0) {
            g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_READY;
        }
        break;
    case FLD_MOVIE_READY:
        if (g_fieldMovieFlags & FLD_MVF_PLAY) {
            queueIdx = 1;
            if (g_gameState->gameData->drawBuf != 0) {
                queueIdx = 2;
            }
            g_fieldFlagQueue.mapFlags[0][queueIdx - 1] = FLD_MAP_CLEAR_ONLY | FLD_MAP_SHADOW_MASK;
            g_fieldFlagQueue.gameFlags[0][queueIdx - 1] = GD_HIDE_SPRITES | GD_HIDE_BG | GD_MENU_DISABLED;
            g_fieldFlagQueue.mapFlags[0][queueIdx - 1] |= FLD_MAP_NO_FADE;
            if (g_gameState->gameData->ctx.field->area->scene->movieRgb24 != 0) {
                g_fieldFlagQueue.gameFlags[0][queueIdx - 1] |= GD_HIDE_MODELS | GD_FLD_TAKEOVER;
                g_fieldFlagQueue.mapFlags[0][queueIdx - 1] |= FLD_MAP_NO_WINDOWS;
            }
            g_fieldFlagQueue.movieFlags[0][queueIdx - 1] = FLD_MVF_CAMERA;
            g_fieldFlagQueue.movieFlags[1][queueIdx] = FLD_MVF_CAMERA;
            g_fieldFlagQueue.movieFlags[0][queueIdx] = FLD_MVF_RUNNING;
            if (!(g_padPortMask[0] & BTN_START)) {
                g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_MOVIE_DISPMASK;
                g_padPortMask[0] |= BTN_START;
            }
            g_gameState->vsyncMode = 4;
            akao_sendCmdE4(0);
            akao_sendCmdE2();
            DrawSync(0);
            MBG_init(1, &g_gameState->gameData->ctx.field->area->scene->movieWork);
            mbg_setRgb24(g_gameState->gameData->ctx.field->area->scene->movieRgb24);
            mbg_setEndCallback(fld_movieStop);
            g_fieldMovieLastFrame = g_mbgFrame;
            mbg_setVolume(2, 4, g_gameState->gameData->ctx.field->area->scene->movieSndVolume);
            mbg_play(g_gameState->gameData->ctx.field->area->scene->movieList,
                          g_gameState->gameData->ctx.field->area->scene->movieEntry);
            g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_PLAYING;
        }
        break;
    case FLD_MOVIE_PLAYING:
        if (g_fieldMovieFlags & FLD_MVF_CAMERA) {
            mbg_getStartCamera((void *)&g_gameState->gameData->viewMatrix, &g_gameState->gameData->projDist);
        }
        if ((g_fieldMovieFlags & FLD_MVF_RUNNING) && (mbg_getStatus() & MBG_STATUS_PLAYING)) {
            done = mbg_update(0);
            lastFrame = mbg_getLastFrame();
            if (!(g_fieldMovieFlags & FLD_MVF_END_FADE) && (u32)lastFrame < 99999) {
                remain = lastFrame - g_mbgFrame;
                if (remain < 4) {
                    akao_sendCmdE5(remain * 4, 0);
                    g_fieldMovieFlags |= FLD_MVF_END_FADE;
                }
            }
            if (done != 0) {
                setlen(&g_fieldMovieMaskPkt[g_gameState->gameData->drawBuf ^ 1], PRIM_LEN(DR_STP));
                g_fieldMovieMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[0] = _get_mask(0, 1);
                g_fieldMovieMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[1] = 0;
                addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf ^ 1],
                        &g_fieldMovieMaskPkt[g_gameState->gameData->drawBuf ^ 1]);
                DrawOTag(g_gameState->gameData->ot[g_gameState->gameData->drawBuf ^ 1]);
            } else if (g_gameState->gameData->ctx.field->area->scene->movieRgb24 != 0 && g_gameState->gameData->drawBuf == 0) {
                g_gameState->dispEnv[g_gameState->gameData->drawBuf].disp.x = 0x1E0;
            }
            if ((g_mbgFrame >= lastFrame || g_fieldMovieLastFrame >= g_mbgFrame) && !(g_fieldMovieFlags & FLD_MVF_END_DONE)) {
                if (g_fieldMovieLastFrame >= g_mbgFrame) {
                    ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
                    g_gameState->gameData->flags &= ~(GD_MENU_DISABLED | GD_HIDE_ALL);
                    g_gameState->gameData->ctx.field->flags &= ~(FLD_MAP_NO_FADE | FLD_MAP_CLEAR_ONLY | FLD_MAP_SHADOW_MASK | FLD_MAP_NO_WINDOWS);
                    if (g_gameState->gameData->ctx.field->area->scene->movieRgb24 != 0 && g_gameState->gameData->drawBuf == 0) {
                        g_gameState->dispEnv[g_gameState->gameData->drawBuf].disp.x = 0x1E0;
                    }
                    printf("Fr: %d\n", g_fieldMovieLastFrame);
                } else {
                    g_fieldFlagQueue.gameFlags[1][0] = (GD_MENU_DISABLED | GD_HIDE_ALL);
                    g_fieldFlagQueue.mapFlags[1][0] = FLD_MAP_NO_FADE | FLD_MAP_CLEAR_ONLY | FLD_MAP_SHADOW_MASK | FLD_MAP_NO_WINDOWS;
                }
                g_fieldMovieFlags |= FLD_MVF_END_DONE;
            }
            g_fieldMovieLastFrame = g_mbgFrame;
        }
        break;
    case FLD_MOVIE_STOPPING:
        if ((g_fieldMovieFlags & FLD_MVF_STOPPED) && cd_getState() == 0) {
            fld_movieFinish();
            g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_DONE;
        }
        break;
    }
}

void fld_movieStop(void) {
    if (g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_MOVIE_DISPMASK) {
        g_padPortMask[0] &= ~BTN_START;
        g_gameState->gameData->ctx.field->area->scene->flags &= ~FLD_SCN_MOVIE_DISPMASK;
    }
    mbg_shutdown();
    cd_cancel();
    g_fieldMovieFlags |= FLD_MVF_STOPPED;
    g_gameState->gameData->ctx.field->area->scene->movieState = FLD_MOVIE_STOPPING;
}

void fld_movieFinish(void) {
    FieldMap *field;

    g_gameState->gameData->flags &= ~(GD_MENU_DISABLED | GD_HIDE_ALL | GD_FLD_TAKEOVER);
    field = g_gameState->gameData->ctx.field;
    g_fieldMovieFlags &= ~FLD_MVF_PER_MOVIE;
    field->flags &= ~(FLD_MAP_NO_PRELOAD | FLD_MAP_NO_FADE | FLD_MAP_CLEAR_ONLY | FLD_MAP_SHADOW_MASK | FLD_MAP_NO_WINDOWS);
    g_gameState->vsyncMode = 2;
    g_gameState->dispEnv[0].disp.x = 0x140;
    g_gameState->dispEnv[1].disp.x = 0;
    fld_soundCommand(SND_STREAM_VOL, -1, g_gameState->gameData->ctx.field->area->scene->sndVolume, 0, 0);
}

s32 fld_isMoviePlaying(void) {
    if ((u32)(g_gameState->gameData->ctx.field->area->scene->movieState - FLD_MOVIE_PLAYING) < 2 && (g_fieldMovieFlags & FLD_MVF_RUNNING)
        && (mbg_getStatus() & MBG_STATUS_PLAYING)) {
        return 1;
    }
    return 0;
}

void fld_moviePlay(u16 movie, s32 rgb24) {
    fld_movieCommand(0, movie, rgb24);
}

void fld_movieGo(void) {
    fld_movieCommand(2, 0, 0);
}

void fld_movieRequestStop(void) {
    fld_movieCommand(1, 0, 0);
}

void fld_beginFrame(void) {
    if (g_gameState->gameData->ctx.field->flags & FLD_MAP_SHADOW_MASK) {
        ClearOTag(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
        setlen(&g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1], PRIM_LEN(DR_STP));
        g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[0] = _get_mask(0, 0);
        g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[1] = 0;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf ^ 1][OT_LAST], &g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1]);
    } else {
        ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
    }
    msg_beginDraw();
}

/* end of frame: flip buffers, draw and apply the queued flag changes */
void fld_endFrame(void) {
    s32 i;
    s32 j;
    u32 mapFlags;

    if ((g_padPortHeld[0] & PAD_SOFT_RESET) == PAD_SOFT_RESET || (g_padPortHeld[1] & PAD_SOFT_RESET) == PAD_SOFT_RESET) {
        g_gameState->flags |= GS_RESET;
    }
    DrawSync(0);
    if (!(mbg_getStatus() & MBG_STATUS_PLAYING)) {
        if ((g_gameState->gameData->flags & GD_HIDE_ALL) == GD_HIDE_ALL && (g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
            g_gameState->vsyncMode = 0;
        } else {
            g_gameState->vsyncMode = 2;
        }
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    if (g_fieldMovieFlags & FLD_MVF_FLUSH_CACHE) {
        EnterCriticalSection();
        FlushCache();
        ExitCriticalSection();
        g_fieldMovieFlags &= ~FLD_MVF_FLUSH_CACHE;
    }
    if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_DISPENV)) {
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    }
    if (!(g_gameState->gameData->flags & GD_PAUSED)) {
        mapFlags = g_gameState->gameData->ctx.field->flags;
        if (!(mapFlags & FLD_MAP_NO_DRAW)) {
            if (mapFlags & FLD_MAP_CLEAR_ONLY) {
                PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
            } else if (mapFlags & FLD_MAP_ACTIVE) {
                DrawOTagEnv(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][OT_LAST], &g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
            } else {
                DrawOTag(g_gameState->gameData->ot[g_gameState->gameData->drawBuf]);
            }
        }
    }
    if (!(g_gameState->gameData->flags & GD_NO_FLIP)) {
        g_gameState->gameData->drawBuf ^= 1;
        gfx_setBuffer(g_gameState->gameData->drawBuf);
    }
    if (!(g_gameState->gameData->flags & GD_PAUSED)) {
        g_gameState->gameData->flags |= g_fieldFlagQueue.gameFlags[0][0];
        g_gameState->gameData->flags &= ~g_fieldFlagQueue.gameFlags[1][0];
        g_gameState->gameData->ctx.field->flags |= g_fieldFlagQueue.mapFlags[0][0];
        g_gameState->gameData->ctx.field->flags &= ~g_fieldFlagQueue.mapFlags[1][0];
        g_fieldMovieFlags |= g_fieldFlagQueue.movieFlags[0][0];
        g_fieldMovieFlags &= ~g_fieldFlagQueue.movieFlags[1][0];
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                g_fieldFlagQueue.gameFlags[i][j] = g_fieldFlagQueue.gameFlags[i][j + 1];
                g_fieldFlagQueue.mapFlags[i][j] = g_fieldFlagQueue.mapFlags[i][j + 1];
                g_fieldFlagQueue.movieFlags[i][j] = g_fieldFlagQueue.movieFlags[i][j + 1];
            }
            g_fieldFlagQueue.gameFlags[i][3] = 0;
            g_fieldFlagQueue.mapFlags[i][3] = 0;
            g_fieldFlagQueue.movieFlags[i][3] = 0;
        }
    }
}

void fld_startEncounter(void) {
    g_gameState->gameData->ctx.field->area->scene->encounterStep = 0;
    g_fieldFlagQueue.mapFlags[1][3] = FLD_MAP_ACTIVE;
    g_fieldFlagQueue.gameFlags[0][0] = GD_FLD_TAKEOVER | GD_MENU_OPEN | GD_MENU_DISABLED;
    g_fieldFlagQueue.gameFlags[0][2] = GD_NO_FLIP | GD_HIDE_ALL;
    g_fieldFlagQueue.mapFlags[0][2] = FLD_MAP_NO_PRELOAD | FLD_MAP_NO_FADE | FLD_MAP_ENCOUNTER_UNREAD | FLD_MAP_NO_VIBRATION
                                       | FLD_MAP_NO_BGI_ANIM | FLD_MAP_NO_WINDOWS; /* the flags fld_updateEncounter clears */
    g_padPortMask[0] |= BTN_START;
}

void fld_updateEncounter(void) {
    EvtObject *player;
    SVECTOR worldPos;
    RECT rect;
    PreloadEntry *entry;
    s32 i;
    s16 w;

    if (g_gameState->gameData->ctx.field->area->scene->encounterStep == 0) {
        g_swirlFrame = 0;
        g_swirlBuf = g_loadAreaEnd + 0x153F00;
        fld_clearModelCache();
        g_gameState->gameData->ctx.field->area->scene->heapTop = g_modeHeap;
        player = evt_getPlayer();
        if (player != NULL) {
            fldbg_applyGeomOffset();
            worldPos.vx = player->pos.vx;
            worldPos.vy = player->pos.vy;
            worldPos.vz = player->pos.vz;
            gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
            gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
            gte_ldv0(&worldPos);
            gte_rtps();
            gte_stsxy(&g_gameState->gameData->ctx.field->area->scene->encounterScreenX);
            if ((u16)g_gameState->gameData->ctx.field->area->scene->encounterScreenX >= 320
                || g_gameState->gameData->ctx.field->area->scene->encounterScreenY < 0
                || g_gameState->gameData->ctx.field->area->scene->encounterScreenY >= 224) {
                g_gameState->gameData->ctx.field->area->scene->encounterScreenX = 160;
                g_gameState->gameData->ctx.field->area->scene->encounterScreenY = 112;
            }
            fldbg_resetGeomOffset();
        }
        fld_findBattleMusic();
        if (g_gameState->gameData->musicId[0] != g_gameState->gameData->ctx.field->area->scene->battleMusic
            && (g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
            akao_fadeMusicVolume(0, 0xF, 0);
            g_gameState->gameData->ctx.field->area->scene->encounterMusicStep = 7;
        }
        if (g_gameState->gameData->sfxSlotId[0] != SE_NONE && (g_gameState->gameData->flags & GD_SFX0_PLAYING)) {
            akao_fadeSeVolume((s32)g_gameState->gameData->sfxSlotBuf[0], SND_SE_GROUP_SLOT(0), 0xF, 0);
        }
        if (g_gameState->gameData->sfxSlotId[1] != SE_NONE && (g_gameState->gameData->flags & GD_SFX1_PLAYING)) {
            akao_fadeSeVolume((s32)g_gameState->gameData->sfxSlotBuf[1], SND_SE_GROUP_SLOT(1), 0xF, 0);
        }
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].largeMotor = 0;
            g_padActuator[i].smallMotor = 0;
        }
        akao_sendCmdE2();
        g_gameState->gameData->ctx.field->area->scene->encounterStep++;
    } else {
        if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_ENCOUNTER_LOADED)) {
            if (preload_isReady(OVL_KEY(OVL_BATTLE)) != 0) {
                entry = preload_find(OVL_KEY(OVL_BATTLE));
                ClearOTag(&g_fieldTempOt, 1);
                setlen(&g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1], PRIM_LEN(DR_STP));
                g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[0] = _get_mask(0, 0);
                g_fieldMaskPkt[g_gameState->gameData->drawBuf ^ 1].code[1] = 0;
                addPrim(&g_fieldTempOt, &g_fieldMaskPkt[g_gameState->gameData->drawBuf]);
                DrawOTag(&g_fieldTempOt);
                rect.x = 0;
                rect.y = 0x100;
                rect.h = 0x100;
                rect.w = w = (u32)(entry->loaded / rect.h) >> 1;
                if (entry->loaded % rect.h != 0) {
                    rect.w = w + 1;
                }
                LoadImage(&rect, (u32 *)entry->dest);
                DrawSync(0);
                preload_setVramRect(entry, rect.x, rect.y, rect.w, rect.h);
            } else {
                /* preload_waitCurrent returns NULL when no preload is in flight (seen in d4_f2928), so the PS1 reads fileId from low
                 * RAM here; a port guards it via PSX_NULLABLE (ff9-ds F37). */
                preload_remove(PSX_NULLABLE(preload_waitCurrent())->fileId);
            }
            preload_discardRange((s32)g_loadAreaEnd, 0x156F00);
            fld_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_1, 0, SND_PAN_CENTER, SND_VOL_MAX);
            fld_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_2, 0, SND_PAN_CENTER, SND_VOL_MAX);
            fld_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_3, 0, SND_PAN_CENTER, SND_VOL_MAX);
            g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_ENCOUNTER_LOADED;
        }
        if (!(g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_ENCOUNTER_DONE)
            && swirl_update(g_gameState->gameData->ctx.field->area->scene->encounterScreenX,
                             g_gameState->gameData->ctx.field->area->scene->encounterScreenY)) {
            g_gameState->gameData->ctx.field->area->scene->flags |= FLD_SCN_ENCOUNTER_DONE;
        }
        if (g_gameState->gameData->ctx.field->area->scene->encounterMusicStep > 0) {
            g_gameState->gameData->ctx.field->area->scene->encounterMusicStep--;
        } else if (g_gameState->gameData->ctx.field->area->scene->encounterMusicStep == 0) {
            if (g_gameState->gameData->ctx.field->area->scene->battleMusic != MUSIC_NONE
                && g_gameState->gameData->ctx.field->area->scene->battleMusic != g_gameState->gameData->musicId[0]) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    fld_soundCommand(SND_MUSIC_SUSPEND, g_gameState->gameData->musicId[0], 0, 0, 0);
                }
                fld_soundCommand(SND_MUSIC_LOAD, g_gameState->gameData->ctx.field->area->scene->battleMusic, 0, 0, 0);
                g_gameState->gameData->ctx.field->area->scene->encounterMusicStep = -1;
            } else {
                g_gameState->gameData->ctx.field->area->scene->encounterMusicStep = -2;
            }
        } else if (g_gameState->gameData->ctx.field->area->scene->encounterMusicStep == -1) {
            if (fld_soundCommand(SND_GET_STATE, -1, 0, 0, 0) == 0) {
                g_gameState->gameData->ctx.field->area->scene->encounterMusicStep = -2;
            }
        } else if (g_gameState->gameData->ctx.field->area->scene->encounterMusicStep == -2) {
            fld_suspendResidentSfx();
            if (g_gameState->gameData->ctx.field->area->scene->battleMusic != MUSIC_NONE
                && !(g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
                fld_soundCommand(SND_MUSIC_PLAY, g_gameState->gameData->ctx.field->area->scene->battleMusic, 0, 0, 0);
                akao_setMusicVolume(0, 0x7F);
            }
            g_gameState->gameData->ctx.field->area->scene->encounterMusicStep = -3;
        }
    }
    if ((g_gameState->gameData->ctx.field->area->scene->flags & FLD_SCN_ENCOUNTER_DONE)
        && g_gameState->gameData->ctx.field->area->scene->encounterMusicStep == -3) {
        g_gameState->gameData->flags &= ~(GD_MENU_DISABLED | GD_MENU_OPEN | GD_HIDE_ALL | GD_NO_FLIP | GD_FLD_TAKEOVER);
        g_gameState->gameData->ctx.field->flags &= ~(FLD_MAP_NO_WINDOWS | FLD_MAP_NO_BGI_ANIM | FLD_MAP_NO_VIBRATION | FLD_MAP_ENCOUNTER_UNREAD | FLD_MAP_NO_FADE | FLD_MAP_NO_PRELOAD);
        g_padPortMask[0] &= ~BTN_START;
        g_gameState->flags |= GS_FLD_LEAVE;
    }
}

/* toggle the pause screen */
void fld_togglePause(void) {
    Unk4 color;
    u8 wipeRgb[8];
    s32 spriteW;
    s32 i;
    s32 bright;
    s32 subtractive;
    u32 next;
    SPRT *sp;

    g_gameState->gameData->flags ^= GD_PAUSED;
    if (g_gameState->gameData->flags & GD_PAUSED) {
        akao_pause(0);
        game_resumeClock(1);
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].largeMotor = 0;
            g_padActuator[i].smallMotor = 0;
        }
        ClearOTag(&g_fieldTempOt, 1);
        ui_buildSprite((320 - g_uiSprites[235].w) / 2, (224 - g_uiSprites[235].h) / 2, &g_fieldTempOt, 0xEB, 0, &spriteW, &g_fieldPauseSprite);
        bright = 0;
        if (fade_isActive() || fade_isBusy()) {
            bright = 1;
        }
        subtractive = fade_isSubtractive();
        if (bright && subtractive == 0) {
            if (fade_getType() == 2) {
                fade_getWipeColors(wipeRgb);
                color.b[0] = (wipeRgb[0] + wipeRgb[4]) >> 1;
                color.b[1] = (wipeRgb[1] + wipeRgb[5]) >> 1;
                color.b[2] = (wipeRgb[2] + wipeRgb[6]) >> 1;
            } else {
                color = fade_getColor();
            }
            if (color.b[0] >= 200 && color.b[1] >= 200 && color.b[2] >= 200) {
                setSemiTrans(&g_fieldPauseSprite.sprite, 1);
                sp = &g_fieldPauseSprite.sprite;
                sp++;
                SetDrawTPage(sp, 1, 1,
                             getTPage(0, 2, tpage_getX((s16)g_uiSprites[235].tpage), tpage_getY((s16)g_uiSprites[235].tpage)));
            }
        }
        setlen(&g_fieldPauseDim, PRIM_LEN(TILE));
        g_fieldPauseDim.code = GPU_TILE | GPU_ABE;
        setRGB0(&g_fieldPauseDim, 0x60, 0x60, 0x60);
        setWH(&g_fieldPauseDim, SCREEN_W, SCREEN_H);
        setXY0(&g_fieldPauseDim, 0, 0);
        setlen(&g_fieldPauseTPage, PRIM_LEN(DR_TPAGE));
        next = getaddr(&g_fieldTempOt);
        g_fieldPauseTPage.code[0] = _get_mode(0, 1, getTPage(0, 2, 0, 0));
        setaddr(&g_fieldPauseDim, next), setaddr(&g_fieldTempOt, &g_fieldPauseDim);
        addPrim(&g_fieldTempOt, &g_fieldPauseTPage);
        DrawOTag(&g_fieldTempOt);
        g_gameState->gameData->flags |= GD_NO_FLIP;
    } else {
        game_resumeClock(0);
        akao_resume(0);
        g_gameState->gameData->flags &= ~GD_NO_FLIP;
    }
}

void fld_spriteCommand(s32 spriteIdx, s32 cmd, s32 param1, s32 param2, s32 param3) {
    FieldSpriteSet *h;
    FieldSprite *spr;

    h = g_gameState->gameData->ctx.field->area->scene->sprites;
    if (h == NULL) {
        return;
    }
    spr = &h->entries[spriteIdx];
    if (cmd == FLD_SPRCMD_SET) {
        if (param1 != -1) {
            fld_resetSprite(spr);
            spr->spriteId = param1;
            spr->sprite = db_lookupRegistered(DB_GET_ADDR, (param1 & 0xFFFF) | DB_SPRITE, 0);
            spr->frameEnd = fld_getSpriteFrameCount(spr->sprite) * 16;
        } else {
            spr->sprite = NULL;
        }
    } else if (cmd == FLD_SPRCMD_FLAGS) {
        if (param2 != 0) {
            spr->flags |= param1;
        } else {
            spr->flags &= ~param1;
        }
    } else if (cmd == FLD_SPRCMD_POS) {
        spr->pos[0] = param1;
        spr->pos[1] = param2;
        spr->pos[2] = param3;
    } else if (cmd == FLD_SPRCMD_ROT) {
        spr->rot[0] = param1;
        spr->rot[1] = param2;
        spr->rot[2] = param3;
    } else if (cmd == FLD_SPRCMD_SCALE) {
        spr->scale = param1;
    } else if (cmd == FLD_SPRCMD_PARENT) {
        spr->parent = evt_getModelSlot(param1);
        spr->bone = param2;
    } else if (cmd == FLD_SPRCMD_BRIGHTNESS) {
        spr->brightness = param1;
    } else if (cmd == FLD_SPRCMD_ABR) {
        spr->abr = param1;
    } else if (cmd == FLD_SPRCMD_SPEED) {
        spr->speed = param1;
    } else if (cmd == FLD_SPRCMD_FRAME) {
        spr->frame = param1 * 16;
    } else if (cmd == FLD_SPRCMD_OFFSET) {
        spr->offset[0] = param1;
        spr->offset[1] = param2;
        spr->offset[2] = param3;
    } else if (cmd == FLD_SPRCMD_OT_OFFSET) {
        spr->otOffset = param1;
    }
}

u8 *fld_initSprites(u8 *heap) {
    s32 i;
    s32 cap;
    FieldSpriteSet *h;

    fld_getBgKey(g_gameState->gameData->ctx.field->area->scene, (s16)g_gameState->gameData->fieldId);
    i = 0;
    cap = 0x10;
    h = (FieldSpriteSet *)heap;
    heap += sizeof(FieldSpriteSet);
    h->unk4 = (u16 *)heap;
    heap += 0x20;
    h->unk0 = 1;
    h->capacity = cap;
    h->count = h->capacity;
    for (; i < cap; i++) {
        h->unk4[i] = 0xFFFF;
    }
    h->entries = (FieldSprite *)heap;
    heap += h->count * 0x24;
    for (i = 0; i < h->count; i++) {
        fld_resetSprite(&h->entries[i]);
    }
    h->primBufs[0] = heap;
    h->primBufs[1] = heap + 0x3000;
    g_gameState->gameData->ctx.field->area->scene->sprites = h;
    return heap + 0x6000;
}

void fld_drawSprites(void) {
    FieldSpriteSet *h;
    SVECTOR rot;
    s32 xy[2];
    s32 i;
    FieldSprite *spr;
    u8 *prim;
    u32 *ot;
    long *trans;

    h = g_gameState->gameData->ctx.field->area->scene->sprites;
    if (h == NULL) {
        return;
    }
    if (g_fieldMovieFlags & FLD_MVF_CAMERA) {
        mbg_getGeomOffset(&xy[0], &xy[1]);
        SetGeomOffset(xy[0], xy[1]);
    } else {
        fldbg_applyGeomOffset();
    }
    prim = h->primBufs[g_gameState->gameData->drawBuf];
    ot = &g_gameState->gameData->ot[g_gameState->gameData->drawBuf][g_gameState->gameData->ctx.field->area->scene->otBias];
    for (i = 0; i < h->count; i++) {
        spr = &h->entries[i];
        if (spr->sprite != NULL && (spr->flags & FLD_SPR_VISIBLE)) {
            if ((s8)spr->parent != -1) {
                trans = ((MATRIX *)((FieldObjSlot *)game_getModel((s8)spr->parent))->model->boneMatrices)[spr->bone].t;
                spr->pos[0] = trans[0] + spr->offset[0];
                spr->pos[1] = trans[1] + spr->offset[1];
                spr->pos[2] = trans[2] + spr->offset[2];
            }
            rot.vx = spr->rot[0];
            rot.vy = spr->rot[1];
            rot.vz = spr->rot[2];
            rot.pad = 0;
            prim = fld_drawSprite3D(prim, spr->sprite, spr->pos, &rot, spr->scale, spr->frame >> 4, spr->abr < 4 ? spr->abr : 0xFF, spr->brightness << 4, &ot[spr->otOffset]);
            spr->frame += (s8)spr->speed;
            if (spr->frame >= spr->frameEnd) {
                spr->frame = 0;
            } else if (spr->frame < 0) {
                spr->frame = ((spr->frameEnd >> 4) - 1) << 4;
            }
        }
    }
    if (!(g_fieldMovieFlags & FLD_MVF_CAMERA)) {
        fldbg_resetGeomOffset();
    }
}

/* reset a field entity */
void fld_resetSprite(FieldSprite *spr) {
    s32 i;

    spr->scale = 0x1000;
    spr->speed = 0x10;
    spr->brightness = 0x80;
    spr->flags = FLD_SPR_VISIBLE;
    spr->abr = 0xF;
    spr->spriteId = -1;
    spr->parent = -1;
    spr->bone = 0;
    for (i = 0; i < 3; i++) {
        spr->pos[i] = 0;
        spr->rot[i] = 0;
        spr->offset[i] = 0;
    }
    spr->frame = 0;
    spr->frameEnd = 0;
    spr->sprite = NULL;
    spr->otOffset = 0;
}

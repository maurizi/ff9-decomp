#include "ovl_0a.h"

s32 fld_queueModelBundle(s32 uid) {
    s32 id;

    id = fld_depFindByLink(uid);
    if (id == -1) {
        return 0;
    }
    id = fld_remapModelId(id);
    if (db_lookupRegistered(4, id, 0) == NULL) {
        g_fieldModelQueue[g_gameState->gameData->unk8E4.field->area->scene->modelQueueCount] = id;
        g_gameState->gameData->unk8E4.field->area->scene->modelQueueCount++;
    }
    return 0;
}

void fld_clearModelQueue(void) {
    s32 i;

    for (i = 15; i >= 0; i--) {
        g_fieldModelQueue[i] = -1;
    }
}

s32 fld_loadModelBundle(s32 arg0) {
    FileEntry *f;
    s32 size;
    Unk800C9CCC *c;
    s32 n;
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
    s32 x, y, cx, cy, e;
    s32 e1;
    s32 cx2;
    s32 cy2;
    s32 x2;
    s32 y2;

    f = file_find(10, arg0);
    size = (f->unkC - f->unk4) << 11;
    c = fld_findModelCache(arg0);
    if (c->addr == 0) {
        cdRead_init(g_gameState->gameData->unk854, f->unk4, size,
                      (void *)(fld_allocModelCache(arg0, size) != 0 ? c->addr : D_80067948), 0, -1, 0);
        while (cdRead_update(g_gameState->gameData->unk854) != 0) {
        }
    }
    if (c->addr != 0) {
        s32 m = file_copyViaVram(D_80067948, c->addr, size, NULL);
        DrawSync(0);
        fld_invalidateModelCache(D_80067948 + m);
    }
    vp = db_lookupRegistered(4, 0x1C0000, 0);
    n = D_80067948;
    pack1 = db_lookup(n, 2, DB_PACK, 0);
    pack2 = db_lookup(n, 2, (DB_PACK | 1), 0);
    hdr = (u8 *)db_lookup(pack1, 2, DB_MODEL_INFO, 0) + 4;
    tp = (s16 *)(hdr + (*(u32 *)(hdr + 4) & 0xFFFFFF));
    uv = tp + hdr[7] * 2;
    ent = (FieldTexEnt *)(uv + hdr[7] * 2);
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, 2, (i & 0xFFFF) | DB_TIM, 0);
        e1 = ent[i].unk0;
        if (e1 & 1) {
            x = vp->unk0;
            y = vp->unk2;
            if (vp->unk2 == 0x100) {
                vp->unk0 += 0x40;
                vp->unk2 = 0x180;
            } else {
                vp->unk2 -= 0x80;
            }
            if (vp->unk0 == vp->unk20) {
                vp->unk0 += 0x40;
            }
            cy = vp->unkA;
            cx = vp->unk8;
            vp->unkA += 2;
            tp[i * 2] = getTPage(1, 0, x, y);
            tp[i * 2 + 1] = getClut(cx, cy);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y % 256;
            ent[i].unk4 = x;
            ent[i].unk6 = y;
            file_setTimPos(img, cx, cy, x, y);
            file_loadTim(img);
        }
    }
    for (i = 0; i < hdr[7]; i++) {
        img = db_lookup(pack2, 2, (i & 0xFFFF) | DB_TIM, 0);
        e = ent[i].unk0;
        if (!(e & 1)) {
            x2 = vp->unk10;
            y2 = vp->unk12;
            if (vp->unk12 >= 0x1E0) {
                vp->unk10 -= 0x40;
                vp->unk12 = 0x100;
            } else {
                vp->unk12 += 0x20;
            }
            cy2 = vp->unk1A;
            cx2 = vp->unk18;
            vp->unk1A += 2;
            tp[i * 2] = getTPage(1, 0, x2, y2);
            tp[i * 2 + 1] = getClut(cx2, cy2);
            uv[i * 2] = 0;
            uv[i * 2 + 1] = y2 % 256 - ((e - 2) % 8 * 32 + 0x100) % 128;
            ent[i].unk0 = x2;
            ent[i].unk2 = y2;
            for (j = hdr[7] - 1; j >= 0; j--) {
                if (ent[j].unk0 == e - 1) {
                    ent[i].unk4 = ent[j].unk4;
                    ent[i].unk6 = ent[j].unk6;
                    break;
                }
            }
            file_setTimPos(img, cx2, cy2, x2, y2);
            file_loadTim(img);
        }
    }
    DrawSync(0);
    g_gameState->gameData->unk8E4.field->area->scene->modelBundles[g_gameState->gameData->unk8E4.field->area->scene->modelBundleCount] = pack1;
    g_gameState->gameData->unk8E4.field->area->scene->modelBundleCount++;
    db_register(pack1, 0);
    D_80067948 = pack1 + db_getSize(pack1);
    return 0;
}

s32 fld_freeModelBundles(void) {
    s32 i;

    for (i = g_gameState->gameData->unk8E4.field->area->scene->modelBundleCount - 1; i >= 0; i--) {
        db_unregister((void *)g_gameState->gameData->unk8E4.field->area->scene->modelBundles[i]);
    }
    g_gameState->gameData->unk8E4.field->area->scene->modelBundleCount = 0;
    return 0;
}

s32 fld_setupActorModel(s32 arg0, void *arg1) {
    s32 *tex;
    s32 *uv;
    s32 *spr;
    s32 sprId;
    FieldObjSlot *s;
    s32 slot;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    WmAnimTable *t;
    WmAnimTableEntry *e;
    s32 i;
    u32 r;
    WmModel *a;

    sprId = -1;
    r = func_8004AB64(arg1);
    if (r < 9) {
        slot = r;
        s = (FieldObjSlot *)game_getModel(r);
    } else {
        slot = g_gameState->gameData->unk39 + 9;
        s = (FieldObjSlot *)game_getModel(slot);
        g_gameState->gameData->unk39++;
    }
    arg0 = fld_remapModelId(arg0);
    s->modelId = arg0;
    s->actor = arg1;
    s->model->model = db_lookupRegistered(4, arg0, 0);
    if ((s->model->anim = db_lookupRegistered(4, fld_getModelAnimId(arg0), 0)) != NULL) {
        a = s->model->anim;
        if (IS_OFFSET(a->bones)) {
            a->bones = (s32)a + a->bones;
            if (a->groups != NULL) {
                a->groups = (WmModelGroup *)((u8 *)s->model->anim + (s32)a->groups);
            }
        }
    }
    fld_getModelTables(arg0, &tex, &uv, &spr);
    m = s->model->model;
    g = m->groups;
    sum = 0;
    if (IS_OFFSET(g)) {
        g = (WmModelGroup *)((u8 *)m + (s32)g);
    }
    for (k = 0; k < m->groupCount; k++) {
        sum += g->packetSize;
        g++;
    }
    sum *= 2;
    sum += s->model->model->boneCount * 32 + s->model->model->groupCount * 12;
    fldmdl_initModel(s->model, (u8 *)g_gameState->gameData->unk8E4.field->area->scene->heapTop);
    fldmdl_buildPackets(s->model, (WmTexInfo *)tex, (s16(*)[2])uv);
    g_gameState->gameData->unk8E4.field->area->scene->heapTop += sum;
    fld_trimModelCache();
    t = db_lookupRegistered(8, arg0, DB_MODEL_INFO);
    e = t->entries;
    for (i = t->count - 1; i >= 0; i--) {
        if (e->modelId == (u16)arg0) {
            sprId = e->unk8 | DB_TEX_ANIM;
        }
    }
    if (sprId != -1) {
        s->model->texAnims = db_lookupRegistered(4, sprId, 0);
        fldmdl_initTexAnims((TexAnimRelocSet *)s->model->texAnims, (ModelTexOfs *)spr);
    }
    if (g_gameState->gameData->unk8E4.field->area->scene->walkmesh != NULL) {
        if (slot < 9) {
            bgi_bindActor(slot, s);
        } else {
            bgi_addActor(s);
        }
        bgi_setActorActive(slot, 1);
    }
    g_gameState->gameData->unk8E4.field->area->scene->flags |= FLD_SCN_ACTORS_READY;
    s->flags |= 1;
    return slot;
}

s32 fld_remapModelId(s32 arg0) {
    if ((arg0 & 0xFFFF) == 0xB9 && g_evtGlobalVars.h[0] >= 0x283C) {
        arg0 = 0x2004C;
    }
    return arg0;
}

s32 fld_depFindByLink(s32 arg0) {
    FileTable *t = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unkC == arg0) {
            return e[i].unk0;
        }
    }
    return -1;
}

s32 fld_getModelAnimId(s32 arg0) {
    WmAnimTable *t = db_lookupRegistered(8, arg0, DB_MODEL_INFO);
    s32 i;
    WmAnimTableEntry *e;

    e = t->entries;
    for (i = t->count - 1; i >= 0; i--) {
        if (e->modelId == (u16)arg0) {
            return e->animId | DB_ANIM;
        }
    }
    return -1;
}

/* look up the three tables of an area entry */
s32 fld_getModelTables(s32 arg0, s32 **arg1, s32 **arg2, s32 **arg3) {
    WmAnimTable *t = db_lookupRegistered(8, arg0, DB_MODEL_INFO);
    WmAnimTableEntry *e = t->entries;
    s32 i;

    for (i = t->count - 1; i >= 0; i--) {
        if (e->modelId == (u16)arg0) {
            *arg1 = (s32 *)((u8 *)e + e->unk4);
            *arg2 = *arg1 + e->unk7;
            *arg3 = *arg2 + e->unk7;
            break;
        }
        e++;
    }
    return 0;
}

s32 fld_isModelTexAnimActive(ModelObj *arg0) {
    Unk800AB0B0 *t = arg0->texAnims;
    s32 i;
    Unk800AB0B0Entry *e;

    if (t != NULL) {
        e = t->unk4;
        for (i = t->unk0 - 1; i >= 0; i--) {
            if (e[i].unk0 & 1) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fld_collectVisibleModels(FieldObjSlot **arg0) {
    s32 n;
    Unk16D8 *e;
    FieldObjSlot *s;
    s32 i;
    WmModel *m;
    WmModelGroup *g;
    u32 sum;
    u32 k;
    ModelObj *o;
    Unk8004AB64 *a;
    u16 *p;

    n = 0;
    for (e = evt_getObjectList(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)game_getModel(a->unk3B);
            if (s->model->parent == NULL) {
                arg0[n++] = s;
            }
        }
    }
    for (e = evt_getObjectList(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)game_getModel(a->unk3B);
            p = s->model->parent;
            if (p != NULL) {
                for (i = 0; i < n; i++) {
                    if ((u16 *)arg0[i]->model == p) {
                        arg0[n++] = s;
                        break;
                    }
                }
            }
        }
    }
    if (g_gameState->gameData->unk8E4.field->area->scene->movieState != 0) {
        for (i = 0; i < n; i++) {
            o = arg0[i]->model;
            m = o->model;
            g = m->groups;
            sum = 0;
            if (IS_OFFSET(g)) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->groupCount; k++) {
                sum += g->packetSize;
                g++;
            }
            if (o->packets + (sum * 2 + (o->model->boneCount * 32 + o->model->groupCount * 12)) >= g_loadAreaEnd + 0xF1700) {
                arg0[i] = NULL;
            }
        }
    }
    return n;
}

void fld_drawModels(void) {
    /* block scope: fld_applyModelCommands calls it undeclared (int argument) */
    void fldmdl_projectMesh(ModelObj *, u8);
    FieldEvtCtx *list[16];
    SVECTOR rot;
    DVECTOR sxy;
    s32 ofs[2];
    s16 ax;
    s16 az;
    s32 z;
    u32 *ot;
    s32 n;
    s32 i;
    s32 j;
    s32 j1;
    s32 cnt;
    s32 lim;
    s32 f;
    ModelObj *m;
    FieldEvtObj *o;
    FieldEvtCtx *c;
    WmModel *w;

    lim = 100;
    j = 0;
    if (g_fieldMovieFlags & 0x40) {
        mbg_getGeomOffset(&ofs[0], &ofs[1]);
        SetGeomOffset(ofs[0], ofs[1]);
    } else {
        fldbg_applyGeomOffset();
    }
    i = 0;
    ot = g_gameState->gameData->unk10[g_gameState->gameData->unk8];
    g_gameState->gameData->unk10[g_gameState->gameData->unk8] = ot + g_gameState->gameData->unk8E4.field->area->scene->otBias;
    n = fld_collectVisibleModels((FieldObjSlot **)list);
    for (; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->model;
            o = c->obj;
            if ((c->flags & FLD_OBJ_TILT) && bgi_getActorTilt(o->uid, &ax, &az)) {
                rot.vx = ax;
                rot.vy = o->rotY;
                rot.vz = az;
                math_rotMatrixZXY(&rot, &m->worldMatrix);
            } else {
                rot.vx = o->rotX;
                rot.vy = (o->rotY + j) & 0xFFF;
                rot.vz = o->rotZ;
                math_rotMatrixZYX(&rot, &m->worldMatrix);
            }
            m->worldMatrix.t[0] = o->pos.vx;
            m->worldMatrix.t[1] = o->pos.vy;
            m->worldMatrix.t[2] = o->pos.vz;
            fldmdl_applyScale(m);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            o = c->obj;
            m = c->model;
            w = db_lookupRegistered(4, o->anim | DB_ANIM, 0);
            m->anim = w;
            if (w != NULL && IS_OFFSET(w->bones)) {
                w->bones = (void *)((u8 *)w + (s32)w->bones);
                if (w->groups != NULL) {
                    w->groups = (void *)((u8 *)m->anim + (s32)w->groups);
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->model;
            o = c->obj;
            if (m->flags & 1) {
                fldmdl_calcBoneMatricesLookAt(m, o->animFrame);
            } else {
                fldmdl_calcBoneMatrices(m, o->animFrame);
            }
        }
    }
    gte_SetRotMatrix(&g_gameState->gameData->unk18);
    gte_SetTransMatrix(&g_gameState->gameData->unk18);
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->model;
            rot.vx = ((Unk8004AB64 *)m->boneMatrices)->pos.vx;
            rot.vy = ((Unk8004AB64 *)m->boneMatrices)->pos.vy;
            rot.vz = ((Unk8004AB64 *)m->boneMatrices)->pos.vz;
            gte_ldv0(&rot);
            gte_rtps();
            gte_stsxy(&sxy);
            gte_stsz(&z);
            z = z / 4 + g_gameState->gameData->unk8E4.field->area->scene->otBias;
            if (z < lim || z > 0x1000 - lim) {
                list[i] = NULL;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->model;
            f = c->flags & FLD_OBJ_CLIP_Y;
            lim = m->model->groupCount;
            SetSpadStack12(0x1F8003FC);
            if (f) {
                for (j = 0; j < lim; j++) {
                    f = 1 << j;
                    if (!(m->hiddenGroups & (u32)f)) {
                        fldmdl_projectMeshClipY(m, j);
                        fldmdl_drawMeshClipY(m, j);
                    }
                }
            } else {
                cnt = lim;
                for (j1 = 0; j1 < cnt; j1++) {
                    if (!(m->hiddenGroups & (u32)(1 << j1))) {
                        fldmdl_projectMesh(m, j1);
                        fldmdl_drawMesh(m, j1);
                    }
                }
            }
            ResetSpadStack12();
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            fld_drawShadow(g_gameState->gameData->unk8E4.field->area->scene->shadows, c->obj->uid);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL && !(c->flags & 0x1000)) {
            m = c->model;
            if (m->texAnims->unk0 >= 2 && fld_isModelTexAnimActive(m) == 0) {
                m->texAnims->unk4[2].unk0 |= 1;
                m->texAnims->unk4[2].unk8 = 0;
                m->texAnims->unk4[2].unkE = 0x1000;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            fldmdl_updateTexAnims((TexAnimSet *)((ModelObj *)c->model)->texAnims);
        }
    }
    fld_applyLights(list, n);
    if (!(g_gameState->gameData->unk0 & GD_MENU_OPEN)) {
        fld_playAnimSfx(list, n);
    }
    fld_applyModelCommands(list, n);
    g_gameState->gameData->unk10[g_gameState->gameData->unk8] = ot;
    if (!(g_fieldMovieFlags & 0x40)) {
        fldbg_resetGeomOffset();
    }
}

/* object command arg1 for object arg0 (palette, mirror, triggers) */
s32 fld_modelCommand(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FieldActorInfo *info;
    FieldEvtCtx *c;
    FieldActorExt *x;
    FieldActorExt *y;
    s32 *tex;
    s32 *uv;
    s32 *spr;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    s32 id;
    FieldAnimSfx *t;

    info = &g_gameState->gameData->unk8E4.field->area->scene->actorInfo[arg0];
    c = (FieldEvtCtx *)game_getModel(arg0);
    switch (arg1) {
    case 0:
        if ((u8)arg2 == 0xFF) {
            c->flags &= ~FLD_OBJ_SEMITRANS;
        } else {
            if (arg2 != (s8)info->abr) {
                c->flags |= 0x40000;
            }
            c->flags |= FLD_OBJ_SEMITRANS;
        }
        info->abr = arg2;
        break;
    case 4:
        if (arg2 != 0) {
            ((ModelObj *)c->model)->clipY = arg3;
            c->flags |= FLD_OBJ_CLIP_Y;
        } else {
            c->flags &= ~FLD_OBJ_CLIP_Y;
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        x = info->mirror;
        if (x == NULL) {
            x = info->mirror = (FieldActorExt *)g_gameState->gameData->unk8E4.field->area->scene->heapTop;
            x->flags = 0;
            x->modelId = c->modelId;
            x->model = (ModelObj *)x->modelBuf;
            x->obj = (FieldEvtObj *)x->objBuf;
            x->unk10 = 0;
            x->src = c;
            x->mirrorPoint.v.vx = 0;
            x->mirrorPoint.v.vy = 0;
            x->mirrorPoint.v.vz = 0;
            x->mirrorNormal.v.vx = 0;
            x->mirrorNormal.v.vy = 0x1000;
            x->mirrorNormal.v.vz = 0;
            x->tint.w = 0;
            g_gameState->gameData->unk8E4.field->area->scene->heapTop += sizeof(FieldActorExt);
        }
        y = x;
        if (!(x->flags & 1)) {
            *y->obj = *c->obj;
            y->model->model = ((ModelObj *)c->model)->model;
            y->model->anim = ((ModelObj *)c->model)->anim;
            fld_getModelTables(y->modelId, &tex, &uv, &spr);
            m = y->model->model;
            g = m->groups;
            sum = 0;
            if (IS_OFFSET(g)) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->groupCount; k++) {
                sum += g->packetSize;
                g++;
            }
            sum *= 2;
            sum += y->model->model->boneCount * 32 + y->model->model->groupCount * 12;
            fldmdl_initModel(y->model, (u8 *)g_gameState->gameData->unk8E4.field->area->scene->heapTop);
            fldmdl_buildPackets(y->model, (WmTexInfo *)tex, (s16(*)[2])uv);
            g_gameState->gameData->unk8E4.field->area->scene->heapTop += sum;
            id = (s32)db_lookupRegistered(9, y->modelId, DB_TEX_ANIM);
            if (id != -1) {
                y->model->texAnims = db_lookupRegistered(4, id, 0);
                fldmdl_initTexAnims((TexAnimRelocSet *)y->model->texAnims, (ModelTexOfs *)spr);
            }
            fld_trimModelCache();
            x->flags |= 0x02000001;
        }
        switch (arg1) {
        case 8:
            if (arg2 != 0) {
                y->model->mirrorSrc = (ModelObj *)c->model;
                y->model->mirrorNormal.w[0] = x->mirrorNormal.w[0];
                y->model->mirrorNormal.w[1] = x->mirrorNormal.w[1];
                y->model->mirrorPoint.w[0] = x->mirrorPoint.w[0];
                y->model->mirrorPoint.w[1] = x->mirrorPoint.w[1];
                x->flags |= 0x01000000;
            } else {
                x->flags &= ~0x01000000;
            }
            break;
        case 9:
            x->mirrorPoint.v.vx = arg2;
            x->mirrorPoint.v.vy = arg3;
            x->mirrorPoint.v.vz = arg4;
            break;
        case 10:
            x->mirrorNormal.v.vx = arg2;
            x->mirrorNormal.v.vy = arg3;
            x->mirrorNormal.v.vz = arg4;
            break;
        case 11:
            x->tint.c[0] = arg2;
            x->tint.c[1] = arg3;
            x->tint.c[2] = arg4;
            x->tint.c[3] = 2;
            break;
        }
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        t = fld_findAnimSfx(c, arg2, arg3);
        if (t == NULL && arg1 != 19) {
            t = fld_addAnimSfx(c, arg2, arg3);
        }
        switch (arg1) {
        case 16:
            t->sound[0] = arg4;
            break;
        case 17:
            t->sound[1] = arg4;
            break;
        case 18:
            t->randomPitch = arg4 != 0;
            break;
        case 19:
            fld_removeAnimSfx(c, arg2, arg3);
            break;
        }
        break;
    }
    return 0;
}

void fld_applyModelCommands(FieldEvtCtx **arg0, s32 arg1) {
    s32 i;
    s32 j;
    FieldEvtCtx *c;
    FieldActorInfo *info;
    FieldActorExt *x;
    u32 vis;
    u32 chg;
    u32 f4;
    u32 f2;
    s32 upd;
    s32 n;
    FieldActorExt *y;

    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        upd = 0;
        info = &g_gameState->gameData->unk8E4.field->area->scene->actorInfo[c->obj->uid];
        vis = c->flags & FLD_OBJ_SEMITRANS;
        chg = (info->flags & 0x10000) ^ vis;
        f4 = c->flags & 0x40000;
        f2 = c->flags & 0x20000;
        if (chg || f4 || f2) {
            upd = 1;
        }
        if (upd) {
            if (c->flags & FLD_OBJ_SEMITRANS) {
                fldmdl_setSemiTrans(c->model, 1);
            } else {
                fldmdl_setSemiTrans(c->model, 0);
            }
            if (vis) {
                fldmdl_setAbr(c->model, ((s32)(info->abr << 24) >> 19) & 0xFFE0);
            }
            if (c->flags & 0x20000) {
                c->flags &= ~0x20000;
            }
            c->flags &= ~0x40000;
            if (chg || f4) {
                c->flags |= 0x20000;
            }
            info->flags = (info->flags & ~0x10000) | vis;
        }
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        info = &g_gameState->gameData->unk8E4.field->area->scene->actorInfo[c->obj->uid];
        x = info->mirror;
        if (x != NULL && (x->flags & 0x01000000)) {
            fldmdl_reflectModel(x->model);
            y = x;
            fldmdl_calcBoneMatrices(x->model, x->src->obj->animFrame);
            n = x->model->model->groupCount;
            for (j = 0; j < n; j++) {
                fldmdl_projectMesh(y->model, j & 0xFF);
                fldmdl_drawMeshMirrored(y->model, j & 0xFF);
            }
            if (x->tint.c[3] != 0) {
                for (j = n - 1; j >= 0; j--) {
                    fldmdl_setGroupRgb(y->model, j & 0xFF, x->tint.c[0], x->tint.c[1], x->tint.c[2]);
                }
                x->tint.c[3]--;
            }
        }
    }
}

Unk800C9CCC *fld_findModelCache(s32 arg0) {
    u32 id;
    s32 i;

    id = (u16)arg0;
    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].id == id) {
            return &g_fieldModelCache[i];
        }
    }
    return NULL;
}

u32 fld_getModelCacheBottom(Unk800C9CCC **arg0) {
    u32 min;
    s32 i;

    min = 0x801EDF00;
    if (arg0 != NULL) {
        *arg0 = NULL;
    }
    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].addr != 0 && g_fieldModelCache[i].addr < min) {
            min = g_fieldModelCache[i].addr;
            if (arg0 != NULL) {
                *arg0 = &g_fieldModelCache[i];
            }
        }
    }
    return min;
}

s32 fld_allocModelCache(s32 arg0, s32 arg1) {
    Unk800C9CCC *e;
    u32 lo;
    u32 min;
    u32 a;

    e = fld_findModelCache(arg0);
    min = fld_getModelCacheBottom(NULL);
    lo = g_gameState->gameData->unk8E4.field->area->scene->heapTop;
    if (lo == 0) {
        lo = D_80067948;
    }
    a = min - arg1;
    if (lo < a) {
        e->addr = a;
        e->size = arg1;
        return 1;
    }
    return 0;
}

/* drop the cached textures no longer listed and pack the rest at the top of RAM */
void fld_compactModelCache(void) {
    u32 top;
    Unk800C9CCC *s;
    s32 i;
    s32 j;
    s32 found;
    s32 bi;
    u32 end;
    u32 best;
    u32 addr;
    s32 id;
    u32 sz;
    Unk800C9CCC *t;

    top = 0x801EDF00;
    for (i = 9; i >= 0; i--) {
        s = &g_fieldModelCache[i];
        id = s->id;
        if (s->addr != 0) {
            found = 0;
            for (j = g_gameState->gameData->unk8E4.field->area->scene->modelQueueCount - 1; j >= 0; j--) {
                if ((u16)g_fieldModelQueue[j] == id) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                s->addr = 0;
                s->size = 0;
            }
        }
    }
    for (;;) {
        best = 0;
        bi = -1;
        for (i = 9; i >= 0; i--) {
            t = &g_fieldModelCache[i];
            sz = t->size;
            end = t->addr + sz;
            if (t->addr != 0 && end <= top && best < end) {
                best = end;
                bi = i;
            }
        }
        if (bi == -1) {
            break;
        }
        s = &g_fieldModelCache[bi];
        addr = top - s->size;
        if (s->addr != addr) {
            file_copyViaVram(addr, s->addr, s->size, NULL);
            s->addr = addr;
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
    s32 v = g_gameState->gameData->unk8E4.field->area->scene->heapTop;

    if (v == 0) {
        v = D_80067948;
    }
    fld_invalidateModelCache(v);
}

void fld_invalidateModelCache(s32 arg0) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        if (g_fieldModelCache[i].addr != 0 && (u32)arg0 >= g_fieldModelCache[i].addr) {
            g_fieldModelCache[i].size = 0;
            g_fieldModelCache[i].addr = 0;
        }
    }
}

void fld_drawShadow(FieldObjPlace *arg0, s32 arg1) {
    MATRIX m;
    POLY_FT4 *p[4];
    SVECTOR v;
    s32 z[9];
    s32 bias;
    FieldObjPlace *e;
    FieldObjSlot *s;
    void *lt;
    Unk800B3294Bias *t;
    u32 *mask;
    u32 *mk;
    s32 i;
    s32 k;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    e = &arg0[arg1];
    s = (FieldObjSlot *)game_getModel(arg1);
    bias = 0;
    if (s->flags & 0x10) {
        return;
    }
    lt = g_gameState->gameData->unk8E4.field->area->scene->lights;
    if (lt != NULL) {
        t = (Unk800B3294Bias *)fld_findLightColor(lt, s->modelId);
        if (t == NULL) {
            t = (Unk800B3294Bias *)fld_findLightColor(lt, -1);
        }
        bias = t->unk7;
    }
    fld_calcShadowMatrix(&e->params, s, &m);
    u0 = (s16)(g_gameState->gameData->unk8E4.field->area->scene->shadowTexX % 64) * 2;
    v0 = (s16)(g_gameState->gameData->unk8E4.field->area->scene->shadowTexY % 256);
    for (i = 0; i < 4; i++) {
        mask = NULL;
        if (g_gameState->gameData->unk8E4.field->flags & FLD_MAP_SHADOW_MASK) {
            mask = e->prims[g_gameState->gameData->unk8][i];
            setlen(mask, 2);
            mask[1] = _get_mask(0, 1);
            mask[2] = 0;
            p[i] = (POLY_FT4 *)(mask + 3);
        } else {
            p[i] = e->prims[g_gameState->gameData->unk8][i];
        }
        setPolyFT4(p[i]);
        setSemiTrans(p[i], 1);
        setRGB0(p[i], e->params.shade, e->params.shade, e->params.shade);
        p[i]->tpage = getTPage(1, 2, g_gameState->gameData->unk8E4.field->area->scene->shadowTexX,
                               g_gameState->gameData->unk8E4.field->area->scene->shadowTexY);
        p[i]->clut = getClut(256, 255);
        switch (i) {
        case 0:
            setUV4(p[0], u0, v0, u0 + 31, v0, u0, v0 + 31, u0 + 31, v0 + 31);
            break;
        case 1:
            setUV4(p[1], u0 + 31, v0, u0, v0, u0 + 31, v0 + 31, u0, v0 + 31);
            break;
        case 2:
            setUV4(p[2], u0, v0 + 31, u0 + 31, v0 + 31, u0, v0, u0 + 31, v0);
            break;
        case 3:
            setUV4(p[3], u0 + 31, v0 + 31, u0, v0 + 31, u0 + 31, v0, u0, v0);
            break;
        }
        if (g_gameState->gameData->unk8E4.field->flags & FLD_MAP_SHADOW_MASK) {
            mk = (u32 *)(p[i] + 1);
            setlen(mk, 2);
            mk[1] = _get_mask(0, 1);
            mk[2] = 0;
            MargePrim(p[i], mk);
            MargePrim(mask, p[i]);
        }
    }
    gte_MulMatrix0(&g_gameState->gameData->unk18, &m, &m);
    gte_SetTransMatrix(&g_gameState->gameData->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    FIELD_SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&p[0]->x0);
    gte_stsz(&z[0]);
    FIELD_SHADOW_PT(0, 0x1000);
    gte_stsxy(&p[0]->x1);
    gte_stsxy(&p[1]->x0);
    gte_stsz(&z[1]);
    FIELD_SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&p[1]->x1);
    gte_stsz(&z[2]);
    FIELD_SHADOW_PT(-0x1000, 0);
    gte_stsxy(&p[0]->x2);
    gte_stsxy(&p[2]->x0);
    gte_stsz(&z[3]);
    FIELD_SHADOW_PT(0, 0);
    gte_stsxy(&p[0]->x3);
    gte_stsxy(&p[1]->x2);
    gte_stsxy(&p[2]->x1);
    gte_stsxy(&p[3]->x0);
    gte_stsz(&z[4]);
    FIELD_SHADOW_PT(0x1000, 0);
    gte_stsxy(&p[1]->x3);
    gte_stsxy(&p[3]->x1);
    gte_stsz(&z[5]);
    FIELD_SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&p[2]->x2);
    gte_stsz(&z[6]);
    FIELD_SHADOW_PT(0, -0x1000);
    gte_stsxy(&p[2]->x3);
    gte_stsxy(&p[3]->x2);
    gte_stsz(&z[7]);
    FIELD_SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&p[3]->x3);
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

s32 fld_initShadows(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_80062E3C(arg0, 0x100, 0xFF, arg2, arg3, arg1, 3);
}

/* object placement matrix: either a plain y rotation or facing the model's joint */
void fld_calcShadowMatrix(FieldObjEnt *arg0, FieldObjSlot *arg1, MATRIX *arg2) {
    MATRIX m;
    SVECTOR v;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;
    s32 *d;
    s32 *s;
    s32 c;
    s32 sn;

    if (arg1->flags & FLD_OBJ_SHADOW_FIXED) {
        c = math_cos(arg0->angle);
        sn = math_sin(arg0->angle);
        arg2->m[0][0] = c * arg0->scaleX >> 12;
        arg2->m[1][0] = 0;
        arg2->m[2][0] = -sn * arg0->scaleX >> 12;
        arg2->m[0][1] = 0;
        arg2->m[1][1] = 0x1000;
        arg2->m[2][1] = 0;
        arg2->m[0][2] = sn * arg0->scaleZ >> 12;
        arg2->m[1][2] = 0;
        arg2->m[2][2] = c * arg0->scaleZ >> 12;
        arg2->t[0] = ((c * arg0->offsetX + sn * arg0->offsetZ) >> 12) + ((Unk8004AB64 *)arg1->actor)->pos.vx;
        arg2->t[1] = ((Unk8004AB64 *)arg1->actor)->pos.vy;
        arg2->t[2] = ((-sn * arg0->offsetX + c * arg0->offsetZ) >> 12) + ((Unk8004AB64 *)arg1->actor)->pos.vz;
    } else {
        /* word-wise copy of the joint matrix */
        d = (s32 *)&m;
        s = (s32 *)&((MATRIX *)arg1->model->boneMatrices)[1];
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
        d[3] = s[3];
        d[4] = s[4];
        d[5] = s[5];
        d[6] = s[6];
        d[7] = s[7];
        m.t[2] = 0;
        m.t[1] = 0;
        m.t[0] = 0;
        v.vx = 0;
        v.vy = -0x1000;
        v.vz = 0;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&v);
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
        arg2->m[0][0] = side.vx * arg0->scaleX >> 12;
        arg2->m[1][0] = side.vy * arg0->scaleX >> 12;
        arg2->m[2][0] = side.vz * arg0->scaleX >> 12;
        arg2->m[0][1] = up.vx;
        arg2->m[1][1] = up.vy;
        arg2->m[2][1] = up.vz;
        arg2->m[0][2] = fwd.vx * arg0->scaleZ >> 12;
        arg2->m[1][2] = fwd.vy * arg0->scaleZ >> 12;
        arg2->m[2][2] = fwd.vz * arg0->scaleZ >> 12;
        arg2->t[0] = ((side.vx * arg0->offsetX + fwd.vx * arg0->offsetZ) >> 12) + ((MATRIX *)arg1->model->boneMatrices)[1].t[0];
        arg2->t[1] = ((side.vy * arg0->offsetX + fwd.vy * arg0->offsetZ) >> 12) + ((Unk8004AB64 *)arg1->actor)->pos.vy;
        arg2->t[2] = ((side.vz * arg0->offsetX + fwd.vz * arg0->offsetZ) >> 12) + ((MATRIX *)arg1->model->boneMatrices)[1].t[2];
    }
}

/* field request dispatcher */
s32 fld_movieCommand(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg0 & 0xF) {
    case 0:
        fld_movieStart((arg1 >> 8) & 0xFF, arg1 & 0xFF, arg2);
        return 0;
    case 1:
        fld_movieStop();
        return 0;
    case 2:
        g_fieldMovieFlags |= 1;
        break;
    case 3:
        return fld_movieCommand_state();
    case 4:
        return mbg_getFrame();
    case 5:
        g_gameState->gameData->unk8E4.field->area->scene->movieRgb24 = arg1;
        mbg_setRgb24(arg1);
        break;
    }
    return 0;
}

void fld_movieStart(s32 arg0, s32 arg1, s32 arg2) {
    g_fieldMovieFlags = 0;
    g_gameState->gameData->unk8E4.field->area->scene->movieList = arg0;
    g_gameState->gameData->unk8E4.field->area->scene->movieEntry = arg1;
    g_gameState->gameData->unk8E4.field->area->scene->movieRgb24 = arg2;
    fld_clearModelCache();
    preload_reset();
    g_gameState->gameData->unk8E4.field->flags |= FLD_MAP_NO_PRELOAD;
    g_gameState->gameData->unk8E4.field->area->scene->movieState = 1;
}

void fld_movieUpdate(void) {
    s32 b;
    s32 done;
    s32 t;
    s32 d;

    switch (g_gameState->gameData->unk8E4.field->area->scene->movieState) {
    case 1:
        if (cd_getState() == 0) {
            if (cdRead_update(g_gameState->gameData->unk854) == 0) {
                FileEntry *f = file_find(0, 0x64);
                cdRead_init(g_gameState->gameData->unk854, f->unk4, (f->unkC - f->unk4) << 11,
                              (void *)(g_loadAreaEnd + 0xF1700), 0, 0x15000, NULL);
            }
            g_gameState->gameData->unk8E4.field->area->scene->movieState = 2;
        }
        break;
    case 2:
        if (cdRead_update(g_gameState->gameData->unk854) == 0) {
            g_fieldMovieFlags |= 8;
            g_gameState->gameData->unk8E4.field->area->scene->movieState = 3;
        }
        break;
    case 3:
        if (!(g_fieldMovieFlags & 8)) {
            g_gameState->gameData->unk8E4.field->area->scene->movieFile = cd_findFile(
                g_mbgMovieLists[g_gameState->gameData->unk8E4.field->area->scene->movieList]
                    .unk4[g_gameState->gameData->unk8E4.field->area->scene->movieEntry].unk0);
            if (cd_getState() == 0) {
                cd_seekStreamAsync(g_gameState->gameData->unk8E4.field->area->scene->movieFile);
                g_gameState->gameData->unk8E4.field->area->scene->movieState = 4;
            }
        }
        break;
    case 4:
        if (cd_getState() == 0) {
            g_gameState->gameData->unk8E4.field->area->scene->movieState = 5;
        }
        break;
    case 5:
        if (g_fieldMovieFlags & 1) {
            b = 1;
            if (g_gameState->gameData->unk8 != 0) {
                b = 2;
            }
            g_fieldFlagQueue.mapFlags[0][b - 1] = 0x600;
            g_fieldFlagQueue.gameFlags[0][b - 1] = 0x19;
            g_fieldFlagQueue.mapFlags[0][b - 1] |= 0x800;
            if (g_gameState->gameData->unk8E4.field->area->scene->movieRgb24 != 0) {
                g_fieldFlagQueue.gameFlags[0][b - 1] |= 0x44;
                g_fieldFlagQueue.mapFlags[0][b - 1] |= 1;
            }
            g_fieldFlagQueue.movieFlags[0][b - 1] = 0x40;
            g_fieldFlagQueue.movieFlags[1][b] = 0x40;
            g_fieldFlagQueue.movieFlags[0][b] = 2;
            if (!(g_padPortMask[0] & BTN_START)) {
                g_gameState->gameData->unk8E4.field->area->scene->flags |= FLD_SCN_MOVIE_DISPMASK;
                g_padPortMask[0] |= BTN_START;
            }
            g_gameState->vsyncMode = 4;
            akao_sendCmdE4(0);
            akao_sendCmdE2();
            DrawSync(0);
            MBG_init(1, &g_gameState->gameData->unk8E4.field->area->scene->movieWork);
            mbg_setRgb24(g_gameState->gameData->unk8E4.field->area->scene->movieRgb24);
            mbg_setEndCallback(fld_movieStop);
            g_fieldMovieLastFrame = g_mbgFrame;
            func_8019AB98(2, 4, g_gameState->gameData->unk8E4.field->area->scene->movieSndVolume);
            mbg_play(g_gameState->gameData->unk8E4.field->area->scene->movieList,
                          g_gameState->gameData->unk8E4.field->area->scene->movieEntry);
            g_gameState->gameData->unk8E4.field->area->scene->movieState = 6;
        }
        break;
    case 6:
        if (g_fieldMovieFlags & 0x40) {
            mbg_getStartCamera((void *)&g_gameState->gameData->unk18, &g_gameState->gameData->unkA);
        }
        if ((g_fieldMovieFlags & 2) && (func_80012AD4() & 2)) {
            done = mbg_update(0);
            t = mbg_getLastFrame();
            if (!(g_fieldMovieFlags & 0x20) && (u32)t < 99999) {
                d = t - g_mbgFrame;
                if (d < 4) {
                    akao_sendCmdE5(d * 4, 0);
                    g_fieldMovieFlags |= 0x20;
                }
            }
            if (done != 0) {
                setlen(&g_fieldMovieMaskPkt[g_gameState->gameData->unk8 ^ 1], 2);
                g_fieldMovieMaskPkt[g_gameState->gameData->unk8 ^ 1].code[0] = _get_mask(0, 1);
                g_fieldMovieMaskPkt[g_gameState->gameData->unk8 ^ 1].code[1] = 0;
                addPrim(g_gameState->gameData->unk10[g_gameState->gameData->unk8 ^ 1],
                        &g_fieldMovieMaskPkt[g_gameState->gameData->unk8 ^ 1]);
                DrawOTag(g_gameState->gameData->unk10[g_gameState->gameData->unk8 ^ 1]);
            } else if (g_gameState->gameData->unk8E4.field->area->scene->movieRgb24 != 0 && g_gameState->gameData->unk8 == 0) {
                g_gameState->dispEnv[g_gameState->gameData->unk8].disp.x = 0x1E0;
            }
            if ((g_mbgFrame >= t || g_fieldMovieLastFrame >= g_mbgFrame) && !(g_fieldMovieFlags & 0x100)) {
                if (g_fieldMovieLastFrame >= g_mbgFrame) {
                    ClearOTagR(g_gameState->gameData->unk10[g_gameState->gameData->unk8], 0x1000);
                    g_gameState->gameData->unk0 &= ~(GD_MENU_DISABLED | GD_HIDE_ALL);
                    g_gameState->gameData->unk8E4.field->flags &= ~0xE01;
                    if (g_gameState->gameData->unk8E4.field->area->scene->movieRgb24 != 0 && g_gameState->gameData->unk8 == 0) {
                        g_gameState->dispEnv[g_gameState->gameData->unk8].disp.x = 0x1E0;
                    }
                    printf("Fr: %d\n", g_fieldMovieLastFrame);
                } else {
                    g_fieldFlagQueue.gameFlags[1][0] = (GD_MENU_DISABLED | GD_HIDE_ALL);
                    g_fieldFlagQueue.mapFlags[1][0] = 0xE01;
                }
                g_fieldMovieFlags |= 0x100;
            }
            g_fieldMovieLastFrame = g_mbgFrame;
        }
        break;
    case 7:
        if ((g_fieldMovieFlags & 4) && cd_getState() == 0) {
            fld_movieFinish();
            g_gameState->gameData->unk8E4.field->area->scene->movieState = 8;
        }
        break;
    }
}

void fld_movieStop(void) {
    if (g_gameState->gameData->unk8E4.field->area->scene->flags & FLD_SCN_MOVIE_DISPMASK) {
        g_padPortMask[0] &= ~BTN_START;
        g_gameState->gameData->unk8E4.field->area->scene->flags &= ~FLD_SCN_MOVIE_DISPMASK;
    }
    mbg_shutdown();
    cd_cancel();
    g_fieldMovieFlags |= 4;
    g_gameState->gameData->unk8E4.field->area->scene->movieState = 7;
}

void fld_movieFinish(void) {
    FieldMap *f;

    g_gameState->gameData->unk0 &= ~0x5D;
    f = g_gameState->gameData->unk8E4.field;
    g_fieldMovieFlags &= 0xFE00;
    f->flags &= ~0x1E01;
    g_gameState->vsyncMode = 2;
    g_gameState->dispEnv[0].disp.x = 0x140;
    g_gameState->dispEnv[1].disp.x = 0;
    fld_soundCommand(0x6201, -1, g_gameState->gameData->unk8E4.field->area->scene->sndVolume, 0, 0);
}

s32 fld_isMoviePlaying(void) {
    if ((u32)(g_gameState->gameData->unk8E4.field->area->scene->movieState - 6) < 2 && (g_fieldMovieFlags & 2)
        && (func_80012AD4() & 2)) {
        return 1;
    }
    return 0;
}

void fld_moviePlay(u16 arg0, s32 arg1) {
    fld_movieCommand(0, arg0, arg1);
}

void fld_movieGo(void) {
    fld_movieCommand(2, 0, 0);
}

void fld_movieRequestStop(void) {
    fld_movieCommand(1, 0, 0);
}

void fld_beginFrame(void) {
    if (g_gameState->gameData->unk8E4.field->flags & FLD_MAP_SHADOW_MASK) {
        ClearOTag(g_gameState->gameData->unk10[g_gameState->gameData->unk8], 0x1000);
        setlen(&g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1], 2);
        g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1].code[0] = _get_mask(0, 0);
        g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1].code[1] = 0;
        addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8 ^ 1][0xFFF], &g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1]);
    } else {
        ClearOTagR(g_gameState->gameData->unk10[g_gameState->gameData->unk8], 0x1000);
    }
    msg_beginDraw();
}

/* end of frame: flip buffers, draw and apply the queued flag changes */
void fld_endFrame(void) {
    s32 i;
    s32 j;
    u32 f;

    if ((g_padPortHeld[0] & PAD_SOFT_RESET) == PAD_SOFT_RESET || (g_padPortHeld[1] & PAD_SOFT_RESET) == PAD_SOFT_RESET) {
        g_gameState->unk0 |= GS_RESET;
    }
    DrawSync(0);
    if (!(func_80012AD4() & 2)) {
        if ((g_gameState->gameData->unk0 & GD_HIDE_ALL) == GD_HIDE_ALL && (g_gameState->gameData->unk8E4.field->flags & 4)) {
            g_gameState->vsyncMode = 0;
        } else {
            g_gameState->vsyncMode = 2;
        }
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    if (g_fieldMovieFlags & 8) {
        EnterCriticalSection();
        FlushCache();
        ExitCriticalSection();
        g_fieldMovieFlags &= ~8;
    }
    if (!(g_gameState->gameData->unk8E4.field->flags & FLD_MAP_NO_DISPENV)) {
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->unk8]);
    }
    if (!(g_gameState->gameData->unk0 & GD_PAUSED)) {
        f = g_gameState->gameData->unk8E4.field->flags;
        if (!(f & FLD_MAP_NO_DRAW)) {
            if (f & FLD_MAP_CLEAR_ONLY) {
                PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->unk8]);
            } else if (f & 4) {
                DrawOTagEnv(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][0xFFF], &g_gameState->drawEnv[g_gameState->gameData->unk8]);
            } else {
                DrawOTag(g_gameState->gameData->unk10[g_gameState->gameData->unk8]);
            }
        }
    }
    if (!(g_gameState->gameData->unk0 & GD_NO_FLIP)) {
        g_gameState->gameData->unk8 ^= 1;
        gfx_setBuffer(g_gameState->gameData->unk8);
    }
    if (!(g_gameState->gameData->unk0 & GD_PAUSED)) {
        g_gameState->gameData->unk0 |= g_fieldFlagQueue.gameFlags[0][0];
        g_gameState->gameData->unk0 &= ~g_fieldFlagQueue.gameFlags[1][0];
        g_gameState->gameData->unk8E4.field->flags |= g_fieldFlagQueue.mapFlags[0][0];
        g_gameState->gameData->unk8E4.field->flags &= ~g_fieldFlagQueue.mapFlags[1][0];
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
    g_gameState->gameData->unk8E4.field->area->scene->encounterStep = 0;
    g_fieldFlagQueue.mapFlags[1][3] = 4;
    g_fieldFlagQueue.gameFlags[0][0] = 0x43;
    g_fieldFlagQueue.gameFlags[0][2] = 0x3C;
    g_fieldFlagQueue.mapFlags[0][2] = 0x1919;
    g_padPortMask[0] |= BTN_START;
}

void fld_updateEncounter(void) {
    Unk8004AB64 *a;
    SVECTOR v;
    RECT r;
    PreloadEntry *e;
    s32 i;
    s16 w;

    if (g_gameState->gameData->unk8E4.field->area->scene->encounterStep == 0) {
        g_swirlFrame = 0;
        g_swirlBuf = g_loadAreaEnd + 0x153F00;
        fld_clearModelCache();
        g_gameState->gameData->unk8E4.field->area->scene->heapTop = D_80067948;
        a = evt_getPlayer();
        if (a != NULL) {
            fldbg_applyGeomOffset();
            v.vx = a->pos.vx;
            v.vy = a->pos.vy;
            v.vz = a->pos.vz;
            gte_SetRotMatrix(&g_gameState->gameData->unk18);
            gte_SetTransMatrix(&g_gameState->gameData->unk18);
            gte_ldv0(&v);
            gte_rtps();
            gte_stsxy(&g_gameState->gameData->unk8E4.field->area->scene->encounterScreenX);
            if ((u16)g_gameState->gameData->unk8E4.field->area->scene->encounterScreenX >= 320
                || g_gameState->gameData->unk8E4.field->area->scene->encounterScreenY < 0
                || g_gameState->gameData->unk8E4.field->area->scene->encounterScreenY >= 224) {
                g_gameState->gameData->unk8E4.field->area->scene->encounterScreenX = 160;
                g_gameState->gameData->unk8E4.field->area->scene->encounterScreenY = 112;
            }
            fldbg_resetGeomOffset();
        }
        fld_findBattleMusic();
        if (g_gameState->gameData->unk81C[0] != g_gameState->gameData->unk8E4.field->area->scene->battleMusic
            && (g_gameState->gameData->unk0 & GD_MUSIC_PLAYING)) {
            akao_fadeMusicVolume(0, 0xF, 0);
            g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep = 7;
        }
        if (g_gameState->gameData->unk830[0] != 0xFFFF && (g_gameState->gameData->unk0 & GD_SFX0_PLAYING)) {
            akao_fadeSeVolume((s32)g_gameState->gameData->unk828, 0x100000, 0xF, 0);
        }
        if (g_gameState->gameData->unk830[1] != 0xFFFF && (g_gameState->gameData->unk0 & GD_SFX1_PLAYING)) {
            akao_fadeSeVolume((s32)g_gameState->gameData->unk82C, 0x200000, 0xF, 0);
        }
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].unk1 = 0;
            g_padActuator[i].unk0 = 0;
        }
        akao_sendCmdE2();
        g_gameState->gameData->unk8E4.field->area->scene->encounterStep++;
    } else {
        if (!(g_gameState->gameData->unk8E4.field->area->scene->flags & FLD_SCN_ENCOUNTER_LOADED)) {
            if (preload_isReady(0x1000B) != 0) {
                e = preload_find(0x1000B);
                ClearOTag(&g_fieldTempOt, 1);
                setlen(&g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1], 2);
                g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1].code[0] = _get_mask(0, 0);
                g_fieldMaskPkt[g_gameState->gameData->unk8 ^ 1].code[1] = 0;
                addPrim(&g_fieldTempOt, &g_fieldMaskPkt[g_gameState->gameData->unk8]);
                DrawOTag(&g_fieldTempOt);
                r.x = 0;
                r.y = 0x100;
                r.h = 0x100;
                r.w = w = (u32)(e->unkC / r.h) >> 1;
                if (e->unkC % r.h != 0) {
                    r.w = w + 1;
                }
                LoadImage(&r, (u32 *)e->unk8);
                DrawSync(0);
                preload_setVramRect(e, r.x, r.y, r.w, r.h);
            } else {
                preload_remove(preload_waitCurrent()->unk4);
            }
            preload_discardRange((s32)g_loadAreaEnd, 0x156F00);
            fld_soundCommand(0xD000, 0x27C, 0, 0x80, 0x7F);
            fld_soundCommand(0xD000, 0x27B, 0, 0x80, 0x7F);
            fld_soundCommand(0xD000, 0x27A, 0, 0x80, 0x7F);
            g_gameState->gameData->unk8E4.field->area->scene->flags |= FLD_SCN_ENCOUNTER_LOADED;
        }
        if (!(g_gameState->gameData->unk8E4.field->area->scene->flags & FLD_SCN_ENCOUNTER_DONE)
            && swirl_update(g_gameState->gameData->unk8E4.field->area->scene->encounterScreenX,
                             g_gameState->gameData->unk8E4.field->area->scene->encounterScreenY)) {
            g_gameState->gameData->unk8E4.field->area->scene->flags |= FLD_SCN_ENCOUNTER_DONE;
        }
        if (g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep > 0) {
            g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep--;
        } else if (g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep == 0) {
            if (g_gameState->gameData->unk8E4.field->area->scene->battleMusic != 0xFFFF
                && g_gameState->gameData->unk8E4.field->area->scene->battleMusic != g_gameState->gameData->unk81C[0]) {
                if (g_gameState->gameData->unk0 & GD_MUSIC_PLAYING) {
                    fld_soundCommand(0x800, g_gameState->gameData->unk81C[0], 0, 0, 0);
                }
                fld_soundCommand(0x700, g_gameState->gameData->unk8E4.field->area->scene->battleMusic, 0, 0, 0);
                g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep = -1;
            } else {
                g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep = -2;
            }
        } else if (g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep == -1) {
            if (fld_soundCommand(0xC00, -1, 0, 0, 0) == 0) {
                g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep = -2;
            }
        } else if (g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep == -2) {
            fld_suspendResidentSfx();
            if (g_gameState->gameData->unk8E4.field->area->scene->battleMusic != 0xFFFF
                && !(g_gameState->gameData->unk0 & GD_MUSIC_PLAYING)) {
                fld_soundCommand(0, g_gameState->gameData->unk8E4.field->area->scene->battleMusic, 0, 0, 0);
                akao_setMusicVolume(0, 0x7F);
            }
            g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep = -3;
        }
    }
    if ((g_gameState->gameData->unk8E4.field->area->scene->flags & FLD_SCN_ENCOUNTER_DONE)
        && g_gameState->gameData->unk8E4.field->area->scene->encounterMusicStep == -3) {
        g_gameState->gameData->unk0 &= ~0x7F;
        g_gameState->gameData->unk8E4.field->flags &= ~0x1919;
        g_padPortMask[0] &= ~BTN_START;
        g_gameState->unk0 |= GS_FLD_LEAVE;
    }
}

/* toggle the pause screen */
void fld_togglePause(void) {
    Unk4 c;
    u8 buf[8];
    s32 tmp;
    s32 i;
    s32 bright;
    s32 v;
    u32 a;
    SPRT *sp;

    g_gameState->gameData->unk0 ^= GD_PAUSED;
    if (g_gameState->gameData->unk0 & GD_PAUSED) {
        akao_sendCmd99(0);
        game_resumeClock(1);
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].unk1 = 0;
            g_padActuator[i].unk0 = 0;
        }
        ClearOTag(&g_fieldTempOt, 1);
        ui_buildSprite((320 - g_uiSprites[235].unk4) / 2, (224 - g_uiSprites[235].unk5) / 2, &g_fieldTempOt, 0xEB, 0, &tmp, &g_fieldPauseSprite);
        bright = 0;
        if (fade_isActive() || fade_isBusy()) {
            bright = 1;
        }
        v = fade_isSubtractive();
        if (bright && v == 0) {
            if (fade_getType() == 2) {
                fade_getWipeColors(buf);
                c.unk0[0] = (buf[0] + buf[4]) >> 1;
                c.unk0[1] = (buf[1] + buf[5]) >> 1;
                c.unk0[2] = (buf[2] + buf[6]) >> 1;
            } else {
                c = fade_getColor();
            }
            if (c.unk0[0] >= 200 && c.unk0[1] >= 200 && c.unk0[2] >= 200) {
                setSemiTrans(&g_fieldPauseSprite.unk0, 1);
                sp = &g_fieldPauseSprite.unk0;
                sp++;
                SetDrawTPage(sp, 1, 1,
                             getTPage(0, 2, tpage_getX((s16)g_uiSprites[235].unk6), tpage_getY((s16)g_uiSprites[235].unk6)));
            }
        }
        setlen(&g_fieldPauseDim, 3);
        g_fieldPauseDim.code = 0x62;
        setRGB0(&g_fieldPauseDim, 0x60, 0x60, 0x60);
        setWH(&g_fieldPauseDim, 320, 224);
        setXY0(&g_fieldPauseDim, 0, 0);
        setlen(&g_fieldPauseTPage, 1);
        a = getaddr(&g_fieldTempOt);
        g_fieldPauseTPage.code[0] = _get_mode(0, 1, getTPage(0, 2, 0, 0));
        setaddr(&g_fieldPauseDim, a), setaddr(&g_fieldTempOt, &g_fieldPauseDim);
        addPrim(&g_fieldTempOt, &g_fieldPauseTPage);
        DrawOTag(&g_fieldTempOt);
        g_gameState->gameData->unk0 |= GD_NO_FLIP;
    } else {
        game_resumeClock(0);
        akao_sendCmd98(0);
        g_gameState->gameData->unk0 &= ~GD_NO_FLIP;
    }
}

void fld_spriteCommand(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FieldSpriteSet *h;
    FieldSprite *e;

    h = g_gameState->gameData->unk8E4.field->area->scene->sprites;
    if (h == NULL) {
        return;
    }
    e = &h->entries[arg0];
    if (arg1 == 0x82) {
        if (arg2 != -1) {
            fld_resetSprite(e);
            e->spriteId = arg2;
            e->sprite = db_lookupRegistered(4, (arg2 & 0xFFFF) | DB_SPRITE, 0);
            e->frameEnd = fld_getSpriteFrameCount(e->sprite) * 16;
        } else {
            e->sprite = NULL;
        }
    } else if (arg1 == 0x83) {
        if (arg3 != 0) {
            e->flags |= arg2;
        } else {
            e->flags &= ~arg2;
        }
    } else if (arg1 == 0x87) {
        e->pos[0] = arg2;
        e->pos[1] = arg3;
        e->pos[2] = arg4;
    } else if (arg1 == 0x8C) {
        e->rot[0] = arg2;
        e->rot[1] = arg3;
        e->rot[2] = arg4;
    } else if (arg1 == 0x91) {
        e->scale = arg2;
    } else if (arg1 == 0x96) {
        e->parent = evt_getModelSlot(arg2);
        e->bone = arg3;
    } else if (arg1 == 0x9B) {
        e->unk7 = arg2;
    } else if (arg1 == 0x9C) {
        e->abr = arg2;
    } else if (arg1 == 0xA0) {
        e->speed = arg2;
    } else if (arg1 == 0xA1) {
        e->frame = arg2 * 16;
    } else if (arg1 == 0xA5) {
        e->offset[0] = arg2;
        e->offset[1] = arg3;
        e->offset[2] = arg4;
    } else if (arg1 == 0xAA) {
        e->otOffset = arg2;
    }
}

u8 *fld_initSprites(u8 *arg0) {
    s32 i;
    s32 n;
    FieldSpriteSet *h;

    fld_getBgKey(g_gameState->gameData->unk8E4.field->area->scene, (s16)g_gameState->gameData->fieldId);
    i = 0;
    n = 0x10;
    h = (FieldSpriteSet *)arg0;
    arg0 += sizeof(FieldSpriteSet);
    h->unk4 = (u16 *)arg0;
    arg0 += 0x20;
    h->unk0 = 1;
    h->capacity = n;
    h->count = h->capacity;
    for (; i < n; i++) {
        h->unk4[i] = 0xFFFF;
    }
    h->entries = (FieldSprite *)arg0;
    arg0 += h->count * 0x24;
    for (i = 0; i < h->count; i++) {
        fld_resetSprite(&h->entries[i]);
    }
    h->primBufs[0] = arg0;
    h->primBufs[1] = arg0 + 0x3000;
    g_gameState->gameData->unk8E4.field->area->scene->sprites = h;
    return arg0 + 0x6000;
}

void fld_drawSprites(void) {
    FieldSpriteSet *h;
    SVECTOR rot;
    s32 xy[2];
    s32 i;
    FieldSprite *e;
    u8 *prim;
    u32 *ot;
    long *t;

    h = g_gameState->gameData->unk8E4.field->area->scene->sprites;
    if (h == NULL) {
        return;
    }
    if (g_fieldMovieFlags & 0x40) {
        mbg_getGeomOffset(&xy[0], &xy[1]);
        SetGeomOffset(xy[0], xy[1]);
    } else {
        fldbg_applyGeomOffset();
    }
    prim = h->primBufs[g_gameState->gameData->unk8];
    ot = &g_gameState->gameData->unk10[g_gameState->gameData->unk8][g_gameState->gameData->unk8E4.field->area->scene->otBias];
    for (i = 0; i < h->count; i++) {
        e = &h->entries[i];
        if (e->sprite != NULL && (e->flags & 1)) {
            if ((s8)e->parent != -1) {
                t = ((MATRIX *)((FieldObjSlot *)game_getModel((s8)e->parent))->model->boneMatrices)[e->bone].t;
                e->pos[0] = t[0] + e->offset[0];
                e->pos[1] = t[1] + e->offset[1];
                e->pos[2] = t[2] + e->offset[2];
            }
            rot.vx = e->rot[0];
            rot.vy = e->rot[1];
            rot.vz = e->rot[2];
            rot.pad = 0;
            prim = fld_drawSprite3D(prim, e->sprite, e->pos, &rot, e->scale, e->frame >> 4, e->abr < 4 ? e->abr : 0xFF, e->unk7 << 4, &ot[e->otOffset]);
            e->frame += (s8)e->speed;
            if (e->frame >= e->frameEnd) {
                e->frame = 0;
            } else if (e->frame < 0) {
                e->frame = ((e->frameEnd >> 4) - 1) << 4;
            }
        }
    }
    if (!(g_fieldMovieFlags & 0x40)) {
        fldbg_resetGeomOffset();
    }
}

/* reset a field entity */
void fld_resetSprite(FieldSprite *arg0) {
    s32 i;

    arg0->scale = 0x1000;
    arg0->speed = 0x10;
    arg0->unk7 = 0x80;
    arg0->flags = 1;
    arg0->abr = 0xF;
    arg0->spriteId = -1;
    arg0->parent = -1;
    arg0->bone = 0;
    for (i = 0; i < 3; i++) {
        arg0->pos[i] = 0;
        arg0->rot[i] = 0;
        arg0->offset[i] = 0;
    }
    arg0->frame = 0;
    arg0->frameEnd = 0;
    arg0->sprite = NULL;
    arg0->otOffset = 0;
}

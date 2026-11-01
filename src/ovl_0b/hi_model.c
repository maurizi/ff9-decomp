#include "ovl_0b.h"

u16 Hi_RegisterSolidEffModel(BtlModelHeader *header) {
    BtlObjSlot *slot = btlfx_allocEffModel();
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->slice = 0;
    slot->kind = HI_EFFMODEL_SOLID;
    slot->otOffset = 0;
    slot->sliceY = 0;
    slot->model->header = header;
    slot->model->motion = NULL;
    btl_applyModelTextures(slot->model, NULL, NULL);
    return slot->id;
}

u16 Hi_RegisterGouEffModel(BtlModelHeader *header) {
    BtlObjSlot *slot = btlfx_allocEffModel();
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->slice = 0;
    slot->kind = HI_EFFMODEL_GOU;
    slot->otOffset = 0;
    slot->sliceY = 0;
    slot->model->header = header;
    slot->model->motion = NULL;
    btl_applyModelTextures(slot->model, NULL, NULL);
    return slot->id;
}

u16 Hi_RegisterTexEffModel(BtlModelHeader *header, s16 tpage, s16 clut, s32 uOfs, s32 vOfs) {
    s16 uvOffset[2];
    BtlObjSlot *slot = btlfx_allocEffModel();
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->slice = 0;
    slot->kind = HI_EFFMODEL_TEX;
    slot->otOffset = 0;
    slot->sliceY = 0;
    slot->model->header = header;
    slot->model->motion = NULL;
    slot->tex.pages[0][0] = tpage;
    slot->tex.pages[0][1] = clut;
    uvOffset[0] = uOfs;
    uvOffset[1] = vOfs;
    btl_applyModelTextures(slot->model, slot->tex.pages[0], uvOffset);
    return slot->id;
}

u16 Hi_RegisterTexListModel(BtlModelHeader *header, BtlTexInfo *texInfo, s16 *uvOffsets, s32 texCount) {
    BtlObjSlot *slot = btlfx_allocEffModel();
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->slice = 0;
    slot->kind = HI_EFFMODEL_TEX;
    slot->otOffset = 0;
    slot->sliceY = 0;
    slot->model->header = header;
    slot->model->motion = NULL;
    slot->tex = *texInfo;
    btl_applyModelTextures(slot->model, slot->tex.pages[0], uvOffsets);
    return slot->id;
}

u16 Hi_RegisterTexPtrModel(BtlModelHeader *header, s16 *texPages, s16 *uvOffsets) {
    BtlObjSlot *slot = btlfx_allocEffModel();
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->slice = 0;
    slot->kind = HI_EFFMODEL_TEX;
    slot->otOffset = 0;
    slot->sliceY = 0;
    slot->model->header = header;
    slot->model->motion = NULL;
    btl_applyModelTextures(slot->model, texPages, uvOffsets);
    return slot->id;
}

s32 btlfx_modelNopZero(void) {
    return 0;
}

s32 Hi_RegisterSummonModel(Unk80100DC054 *summon, s16 *texPos) {
    s16 uvOffsets[6][2];
    Unk8010141C *slot = btlfx_allocSummonModel();
    s32 i;
    if (slot == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    slot->used = 1;
    slot->frame = 0;
    slot->model->header = (BtlModelHeader *)summon->modelHdr;
    slot->model->motion = (BtlMotion *)summon->motions[0];
    if (texPos != NULL) {
        slot->texAnimOn = 1;
    } else {
        slot->texAnimOn = 0;
    }
    for (i = 0; i < summon->texCount; i++) {
        slot->tex[i][0] = summon->texTpage[i];
        slot->tex[i][1] = summon->texClut[i];
        uvOffsets[i][0] = 0;
        uvOffsets[i][1] = summon->texY[i];
        if (texPos != NULL) {
            slot->texPos[i][0] = texPos[0];
            slot->texPos[i][1] = texPos[1];
            slot->texPos[i][2] = texPos[2];
            slot->texPos[i][3] = texPos[3];
            texPos += 4;
        }
    }
    btl_applyModelTextures(slot->model, slot->tex[0], uvOffsets[0]);
    slot->model->texAnims = summon->texAnims;
    btl_relocTexAnims((TexAnimSet *)slot->model->texAnims, (u16 (*)[4])slot->texPos);
    return (u16)slot->id;
}

void Hi_DrawEffModel(SVECTOR *rot, VECTOR *pos, s32 scale, s32 modelId) {
    BtlObjSlot *slot = btlfx_getEffModel(modelId, "Hi_DrawEffModel()\nid:%d cannot use!");
    s32 meshCount;
    s32 i;
    s32 unused;
    btl_setModelTransform(slot->model, rot, pos, scale);
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, 0, g_btlfxEnv->prim);
    slot->model->prims = 0;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        btl_projectMesh(slot->model, i);
        if (slot->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, slot->otOffset);
        } else {
            switch ((u16)slot->kind) {
            case HI_EFFMODEL_SOLID:
            case HI_EFFMODEL_TEX:
                g_btlfxEnv->prim = btl_drawMesh(slot->model, i, g_btlfxEnv->prim);
                break;
            case HI_EFFMODEL_GOU:
                g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawSliceEffModel(SVECTOR *rot, VECTOR *pos, s32 scale, s32 modelId) {
    BtlObjSlot *slot = btlfx_getEffModel(modelId, "Hi_DrawSliceEffModel()\nid:%d cannot use!");
    s32 meshCount;
    s32 i;
    btl_setModelTransform(slot->model, rot, pos, scale);
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, 0, g_btlfxEnv->prim);
    slot->model->prims = 0;
    slot->model->sliceY = pos->vy - slot->sliceY;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        btlfx_projectSliceMesh(slot->model, i);
        g_btlfxEnv->prim = btlfx_drawSliceMesh(slot->model, i, g_btlfxEnv->prim);
    }
}

void Hi_DrawEffModelByBone(s32 scale, s32 modelId, s32 summonId, s32 bone) {
    BtlObjSlot *slot = btlfx_getEffModel(modelId, "Hi_DrawEffModelByBone()\nid:%d cannot use!");
    s32 meshCount;
    s32 i;
    s32 unused;
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(summonId, bone, (BtlBone *)&slot->model->matrix);
    if (scale != 0) {
        btl_scaleModelMatrix(slot->model, (VECTOR *)scale);
    }
    slot->model->prims = 0;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        btl_projectMesh(slot->model, i);
        if (slot->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, slot->otOffset);
        } else {
            switch ((u16)slot->kind) {
            case HI_EFFMODEL_SOLID:
            case HI_EFFMODEL_TEX:
                g_btlfxEnv->prim = btl_drawMesh(slot->model, i, g_btlfxEnv->prim);
                break;
            case HI_EFFMODEL_GOU:
                g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphEffModel(SVECTOR *rot, VECTOR *pos, s32 scale, s32 modelId, s32 morphId, s32 weight) {
    char *errFmt = "Hi_DrawMorphEffModel()\nid:%d cannot use!";
    BtlObjSlot *slot = btlfx_getEffModel(modelId, errFmt);
    s32 meshCount;
    s32 i;
    slot->model->morph = btlfx_getEffModel(morphId, errFmt)->model->header;
    btl_setModelTransform(slot->model, rot, pos, scale);
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, 0, g_btlfxEnv->prim);
    slot->model->prims = 0;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        btlfx_projectMorphMesh(slot->model, i, weight);
        if (slot->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, slot->otOffset);
        } else {
            switch ((u16)slot->kind) {
            case HI_EFFMODEL_SOLID:
            case HI_EFFMODEL_TEX:
                g_btlfxEnv->prim = btl_drawMesh(slot->model, i, g_btlfxEnv->prim);
                break;
            case HI_EFFMODEL_GOU:
                g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphModelByBone(s32 scale, s32 modelId, s32 morphId, s32 weight, s32 summonId, s32 bone) {
    char *errFmt = "Hi_DrawMorphModelByBone()\nid:%d cannot use!";
    BtlObjSlot *slot = btlfx_getEffModel(modelId, errFmt);
    s32 meshCount;
    s32 i;
    slot->model->morph = btlfx_getEffModel(morphId, errFmt)->model->header;
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(summonId, bone, (BtlBone *)&slot->model->matrix);
    if (scale != 0) {
        btl_scaleModelMatrix(slot->model, (VECTOR *)scale);
    }
    slot->model->prims = 0;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        btlfx_projectMorphMesh(slot->model, i, weight);
        if (slot->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, slot->otOffset);
        } else {
            switch ((u16)slot->kind) {
            case HI_EFFMODEL_SOLID:
            case HI_EFFMODEL_TEX:
                g_btlfxEnv->prim = btl_drawMesh(slot->model, i, g_btlfxEnv->prim);
                break;
            case HI_EFFMODEL_GOU:
                g_btlfxEnv->prim = btl_drawMeshOffset(slot->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

s32 Hi_DrawSummonModel(SVECTOR *rot, VECTOR *pos, s32 scale, s32 summonId, s32 flags) {
    Unk8010141C *slot = btlfx_getSummonModel(summonId, "Hi_DrawSummonModel()\nid:%d cannot use!");
    BtlMotion *motion;
    s32 frameCount;
    s32 frame;
    s32 meshCount;
    s32 i;
    btl_setModelTransform(slot->model, rot, pos, scale);
    motion = slot->model->motion;
    if (IS_OFFSET(motion->rotKeys)) {
        motion->rotKeys = (s32)motion + motion->rotKeys;
        if (motion->rotKeysLo != 0) {
            motion->rotKeysLo = (s32)slot->model->motion + motion->rotKeysLo;
        }
    }
    frameCount = slot->model->motion->frameCount;
    if (slot->frame >= frameCount) {
        if (flags & 1) {
            slot->frame = 0;
        } else {
            slot->frame = frameCount - 1;
        }
    }
    if (slot->texAnimOn != 0) {
        btl_updateTexAnims((TexAnimSet *)slot->model->texAnims);
    }
    frame = slot->frame;
    g_btlfxEnv->prim = btl_calcBoneMatrices(slot->model, frame, g_btlfxEnv->prim);
    slot->frame++;
    slot->model->prims = 0;
    meshCount = slot->model->header->meshCount;
    for (i = 0; i < meshCount; i++) {
        if (!(slot->model->hiddenMeshes & (1 << i))) {
            btl_projectMesh(slot->model, i);
            g_btlfxEnv->prim = btl_drawMesh(slot->model, i, g_btlfxEnv->prim);
        }
    }
    return frame;
}

void Hi_SetSummonMotion(BtlMotion *motion, s32 summonId) {
    Unk8010141C *slot = btlfx_getSummonModel(summonId, "Hi_SetSummonMotion()\nid:%d cannot use!");
    slot->frame = 0;
    slot->model->motion = motion;
}

void Hi_SetSummonMotFrame(s32 summonId, s32 frame) {
    Unk8010141C *slot = btlfx_getSummonModel(summonId, s_hiSetSummonMotFrameErr);
    if (slot->model->motion->frameCount < frame) {
        slot->frame = 0;
    } else {
        slot->frame = frame;
    }
}

BtlObjSlot *btlfx_allocEffModel(void) {
    BtlObjSlot *slot = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, slot++) {
        if (slot->used == 0) {
            return slot;
        }
    }
    return NULL;
}

Unk8010141C *btlfx_allocSummonModel(void) {
    Unk8010141C *slot = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, slot++) {
        if (slot->used == 0) {
            return slot;
        }
    }
    return NULL;
}

void btlfx_freeEffModel(void) {
}

void btlfx_freeSummonModel(void) {
}

void btlfx_halt(char *msg) {
    g_btlfxHaltBufs.cur = &g_btlfxHaltBufs.bufs[1];
    g_btlfxHaltBufs.bufs[0].index = 0;
    g_btlfxHaltBufs.bufs[1].index = 1;
    SetDefDrawEnv(&g_btlfxHaltBufs.bufs[0].draw, 0, 0, SCREEN_W, SCREEN_H_NTSC);
    SetDefDrawEnv(&g_btlfxHaltBufs.bufs[1].draw, 0, SCREEN_H_NTSC, SCREEN_W, SCREEN_H_NTSC);
    SetDefDispEnv(&g_btlfxHaltBufs.bufs[0].disp, 0, SCREEN_H_NTSC, SCREEN_W, SCREEN_H_NTSC);
    SetDefDispEnv(&g_btlfxHaltBufs.bufs[1].disp, 0, 0, SCREEN_W, SCREEN_H_NTSC);
    g_btlfxHaltBufs.bufs[1].draw.isbg = 1;
    g_btlfxHaltBufs.bufs[0].draw.isbg = 1;
    setRGB0(&g_btlfxHaltBufs.bufs[0].draw, 0, 0, 0);
    setRGB0(&g_btlfxHaltBufs.bufs[1].draw, 0, 0, 0);
    EnterCriticalSection();
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    ExitCriticalSection();
    SetDispMask(1);
    while (1) {
        g_btlfxHaltBufs.cur = (g_btlfxHaltBufs.cur == &g_btlfxHaltBufs.bufs[0]) ? &g_btlfxHaltBufs.bufs[1] : &g_btlfxHaltBufs.bufs[0];
        DrawSync(0);
        VSync(0);
        PutDispEnv(&g_btlfxHaltBufs.cur->disp);
        PutDrawEnv(&g_btlfxHaltBufs.cur->draw);
    }
}

void btlfx_error(char *msg) {
    char text[0x100];
    sprintf(text, s_hiraishiErrorFmt, msg);
    btlfx_halt(text);
}

void btlfx_initEffModels(BtlModelBuf *bufs, s32 count) {
    BtlObjSlot *slot = g_btlfxEffModels;
    s32 i;
    s32 j;
    for (i = 0; i < 32; i++, slot++) {
        slot->used = 0;
        slot->id = i;
        slot->model = NULL;
        for (j = 0; j < 6; j++) {
            slot->tex.pages[j][0] = 0;
            slot->tex.pages[j][1] = 0;
        }
    }
    slot = g_btlfxEffModels;
    for (i = 0; i < count; i++, slot++, bufs++) {
        slot->model = &bufs->model;
    }
}

void Hi_FreeEffModel(s32 modelId) {
    btlfx_getEffModel(modelId, s_hiFreeEffModelErr)->used = 0;
}

void btlfx_clearEffModels(void) {
    BtlObjSlot *slot = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, slot++) {
        slot->used = 0;
        slot->id = i;
        slot->model = NULL;
    }
}

void btlfx_initSummonModels(BtlModelBuf *bufs, s32 count) {
    Unk8010141C *slot = g_btlfxSummonModels;
    s32 i;
    s32 j;
    for (i = 0; i < 1; i++, slot++) {
        slot->used = 0;
        slot->texAnimOn = 0;
        slot->id = i;
        slot->model = NULL;
        for (j = 0; j < 6; j++) {
            slot->tex[j][0] = 0;
            slot->tex[j][1] = 0;
            slot->texPos[j][0] = 0;
            slot->texPos[j][1] = 0;
            slot->texPos[j][2] = 0;
            slot->texPos[j][3] = 0;
        }
    }
    slot = g_btlfxSummonModels;
    for (i = 0; i < count; i++, slot++, bufs++) {
        slot->model = &bufs->model;
    }
}

void btlfx_clearSummonModels(void) {
    Unk8010141C *slot = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, slot++) {
        slot->used = 0;
        slot->texAnimOn = 0;
        slot->id = i;
        slot->model = NULL;
    }
}

void btlfx_getUnitBonePos(s32 unitId, s32 bone, u16 *out) {
    btl_getBonePos(btl_getUnit(unitId)->model, bone, out);
}

void btlfx_getUnitArgBonePos(s32 unitId, s32 argIndex, u16 *out) {
    btl_getBonePos(btl_getUnit(unitId)->model, g_btlfxEnv->targets->fxArgs[argIndex], out);
}

void btlfx_getUnitBoneMatrix(s32 unitId, s32 bone, BtlBone *out) {
    btl_getBoneMatrix(btl_getUnit(unitId)->model, bone, out);
}

void btlfx_getUnitArgBoneMatrix(s32 unitId, s32 argIndex, BtlBone *out) {
    btl_getBoneMatrix(btl_getUnit(unitId)->model, g_btlfxEnv->targets->fxArgs[argIndex], out);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", s_hiSetSummonMotFrameErr);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", s_hiraishiErrorFmt);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", s_hiFreeEffModelErr);

void Hi_GetSummonBonePos(s32 summonId, s32 bone, u16 *out) {
    btl_getBonePos(btlfx_getSummonModel(summonId, "Hi_GetSummonBonePos()\nid:%d cannot use!")->model, bone, out);
}

void Hi_GetSummonBoneMatrix(s32 summonId, s32 bone, BtlBone *out) {
    btl_getBoneMatrix(btlfx_getSummonModel(summonId, "Hi_GetSummonBoneMatrix()\nid:%d cannot use!")->model, bone, out);
}

void btl_getBonePos(BtlModel *model, s32 bone, u16 *out) {
    out[0] = model->bones[bone].tx;
    out[1] = model->bones[bone].ty;
    out[2] = model->bones[bone].tz;
}

void btl_getBoneMatrix(BtlModel *model, s32 bone, BtlBone *out) {
    *out = model->bones[bone];
}

void btl_setModelTransform(BtlModel *model, SVECTOR *rot, VECTOR *pos, VECTOR *scale) {
    SVECTOR zeroRot;
    VECTOR zeroPos;
    if (rot != NULL) {
        math_rotMatrixYXZ(rot, &model->matrix);
    } else {
        zeroRot.vx = 0;
        zeroRot.vy = 0;
        zeroRot.vz = 0;
        math_rotMatrixYXZ(&zeroRot, &model->matrix);
    }
    if (pos != NULL) {
        TransMatrix(&model->matrix, pos);
    } else {
        zeroPos.vx = 0;
        zeroPos.vy = 0;
        zeroPos.vz = 0;
        TransMatrix(&model->matrix, &zeroPos);
    }
    if (scale != 0) {
        btl_scaleModelMatrix(model, scale);
    } else {
        model->scaleX = 0x1000;
        model->scaleY = 0x1000;
        model->scaleZ = 0x1000;
        btl_applyModelScale(model);
    }
}

/* scales the model matrix by scale (MulMatrix0 with a diagonal matrix built word by word) */
void btl_scaleModelMatrix(BtlModel *model, VECTOR *scale) {
    MATRIX mat;
    s32 *words;
    s16 x, y, z;

    words = (s32 *)&mat;
    x = scale->vx;
    words[0] = 0x1000;
    mat.m[0][0] = x;
    y = scale->vy;
    words[2] = 0x1000;
    mat.m[1][1] = y;
    z = scale->vz;
    words[1] = 0;
    words[3] = 0;
    words[4] = 0x1000;
    words[5] = 0;
    words[6] = 0;
    words[7] = 0;
    mat.m[2][2] = z;
    gte_MulMatrix0(&model->matrix, &mat, &model->matrix);
}

void Hi_ShowSummonModelMesh(s32 summonId, s32 mesh) {
    btlfx_getSummonModel(summonId, "Hi_ShowSummonModelMesh()\nid:%d cannot use!")->model->hiddenMeshes &= ~(1 << mesh);
}

void Hi_HideSummonModelMesh(s32 summonId, s32 mesh) {
    btlfx_getSummonModel(summonId, "Hi_HideSummonModelMesh()\nid:%d cannot use!")->model->hiddenMeshes |= 1 << mesh;
}

/* starts texture animation anim of summon model summonId (once: also set flag 2) */
void Hi_StartSummonTexAnim(s32 summonId, s32 anim, u8 once) {
    Unk8010141C *slot = btlfx_getSummonModel(summonId, "Hi_StartSummonTexAnim()\nid:%d cannot use!");
    s32 off;

    if (once) {
        off = anim * 4;
        off = (off + anim) * 4; /* anim * sizeof(BtlUnit9C50) */
        ((BtlUnit9C50 *)((u8 *)PSX_NULLABLE(slot->model->texAnims) + off))->flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    } else {
        off = anim * 4;
        off = (off + anim) * 4;
        ((BtlUnit9C50 *)((u8 *)PSX_NULLABLE(slot->model->texAnims) + off))->flags |= BTL_TEXANIM_PLAY;
    }
    ((BtlUnit9C50 *)((u8 *)PSX_NULLABLE(slot->model->texAnims) + off))->pos = 0;
    ((BtlUnit9C50 *)((u8 *)PSX_NULLABLE(slot->model->texAnims) + off))->lastFrame = 0x1000;
}

void Hi_StopSummonTexAnim(s32 summonId, s32 anim) {
    BtlUnit9C50 *texAnim = &PSX_NULLABLE(btlfx_getSummonModel(summonId, "Hi_StopSummonTexAnim()\nid:%d cannot use!")->model->texAnims)[anim];
    texAnim->flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
}

void Hi_ModifyEffModelAbr(s32 modelId, s32 abr) {
    if (abr != 0xFF) {
        btl_setModelAbr(btlfx_getEffModel(modelId, "Hi_ModifyEffModelAbr()\nid:%d cannot use!")->model, (u16)(abr << 5));
    }
}

void Hi_ModifyEffModelRGB(s32 modelId, s16 r, s16 g, s16 b) {
    btl_setModelRgb(btlfx_getEffModel(modelId, "Hi_ModifyEffModelRGB()\nid:%d cannot use!")->model, r, g, b);
}

void Hi_SetEffModelOffset(s32 modelId, s16 otOffset) {
    btlfx_getEffModel(modelId, "Hi_SetEffModelOffset()\nid:%d cannot use!")->otOffset = otOffset;
}

void Hi_SetEffModelSlice(s32 modelId, s16 sliceY) {
    BtlObjSlot *slot = btlfx_getEffModel(modelId, "Hi_SetEffModelSlice()\nid:%d cannot use!");
    slot->slice = 1;
    slot->sliceY = sliceY;
}

void Hi_ModifySummonModelAbr(s32 summonId, s32 abr) {
    if (abr != 0xFF) {
        btl_setModelAbr(btlfx_getSummonModel(summonId, "Hi_ModifySummonModelAbr()\nid:%d cannot use!")->model, (u16)(abr << 5));
    }
}

void Hi_ModifySummonModelRGB(s32 summonId, s16 r, s16 g, s16 b) {
    btl_setModelRgb(btlfx_getSummonModel(summonId, "Hi_ModifySummonModelRGB()\nid:%d cannot use!")->model, r, g, b);
}

BtlObjSlot *btlfx_getEffModel(s32 modelId, char *errFmt) {
    BtlObjSlot *slot = &g_btlfxEffModels[modelId];
    if (slot->used == 0 || slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, errFmt, modelId);
        btlfx_error(g_btlfxErrorMsg);
    }
    return slot;
}

Unk8010141C *btlfx_getSummonModel(s32 summonId, char *errFmt) {
    Unk8010141C *slot = &g_btlfxSummonModels[summonId];
    if (slot->used == 0 || slot->model == NULL) {
        sprintf(g_btlfxErrorMsg, errFmt, summonId);
        btlfx_error(g_btlfxErrorMsg);
    }
    return slot;
}

void btlfx_getUnitMeshCount(s32 unitId) {
    btl_getMeshCount(btl_getUnit(unitId)->model);
}

/* Void in the original; effect modules read $v0 after it (export 0x318): btl_getMeshCount's u8 result.
   Ports must return it (docs/effect-modules.md). */
void btlfx_getEffModelMeshCount(s32 modelId) {
    btl_getMeshCount(btlfx_getEffModel(modelId, s_hiSplitMdlVertexErr)->model);
}

u8 btl_getMeshCount(BtlModel *model) {
    return model->header->meshCount;
}

void btlfx_getCasterTexInfo(BtlTexInfo *texPages, BtlTexInfo *uvOffsets, s32 texId) {
    s32 key;
    BtlTexInfo *pages;
    BtlTexInfo *uvs;
    u16 index;
    ModelSlot *npc;

    index = BTL_UNIT_INDEX(btl_getUnit(BTLFX_UNIT_CASTER)->flags);
    npc = &g_gameState->gameData->npcModels[index];
    key = texId & 0xFFFF;
    key |= 0x20000;
    key |= npc->actor->modelSlot << 24;
    pages = (BtlTexInfo *)db_getModelTexPages(key);
    uvs = (BtlTexInfo *)db_getModelUvOffsets(key);
    *texPages = *pages;
    *uvOffsets = *uvs;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", s_hiSplitMdlVertexErr);

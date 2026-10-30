#include "ovl_0b.h"

u16 Hi_RegisterSolidEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->slice = 0;
    p->kind = 0;
    p->otOffset = 0;
    p->sliceY = 0;
    p->model->header = arg0;
    p->model->motion = NULL;
    btl_applyModelTextures(p->model, NULL, NULL);
    return p->id;
}

u16 Hi_RegisterGouEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->slice = 0;
    p->kind = 1;
    p->otOffset = 0;
    p->sliceY = 0;
    p->model->header = arg0;
    p->model->motion = NULL;
    btl_applyModelTextures(p->model, NULL, NULL);
    return p->id;
}

u16 Hi_RegisterTexEffModel(BtlUnit9C8 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    s16 v[2];
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->slice = 0;
    p->kind = 2;
    p->otOffset = 0;
    p->sliceY = 0;
    p->model->header = arg0;
    p->model->motion = NULL;
    p->tex.unk0[0][0] = arg1;
    p->tex.unk0[0][1] = arg2;
    v[0] = arg3;
    v[1] = arg4;
    btl_applyModelTextures(p->model, p->tex.unk0[0], v);
    return p->id;
}

u16 Hi_RegisterTexListModel(BtlUnit9C8 *arg0, BtlTexInfo *arg1, s16 *arg2, s32 arg3) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->slice = 0;
    p->kind = 2;
    p->otOffset = 0;
    p->sliceY = 0;
    p->model->header = arg0;
    p->model->motion = NULL;
    p->tex = *arg1;
    btl_applyModelTextures(p->model, p->tex.unk0[0], arg2);
    return p->id;
}

u16 Hi_RegisterTexPtrModel(BtlUnit9C8 *arg0, s16 *arg1, s16 *arg2) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->slice = 0;
    p->kind = 2;
    p->otOffset = 0;
    p->sliceY = 0;
    p->model->header = arg0;
    p->model->motion = NULL;
    btl_applyModelTextures(p->model, arg1, arg2);
    return p->id;
}

s32 func_800DB8D8(void) {
    return 0;
}

s32 Hi_RegisterSummonModel(Unk80100DC054 *arg0, s16 *arg1) {
    s16 t[6][2];
    Unk8010141C *p = btlfx_allocSummonModel();
    s32 i;
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->model == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->used = 1;
    p->frame = 0;
    p->model->header = (BtlUnit9C8 *)arg0->modelHdr;
    p->model->motion = (BtlUnit9CC *)arg0->motions[0];
    if (arg1 != NULL) {
        p->texAnimOn = 1;
    } else {
        p->texAnimOn = 0;
    }
    for (i = 0; i < arg0->texCount; i++) {
        p->tex[i][0] = arg0->texTpage[i];
        p->tex[i][1] = arg0->texClut[i];
        t[i][0] = 0;
        t[i][1] = arg0->texY[i];
        if (arg1 != NULL) {
            p->texPos[i][0] = arg1[0];
            p->texPos[i][1] = arg1[1];
            p->texPos[i][2] = arg1[2];
            p->texPos[i][3] = arg1[3];
            arg1 += 4;
        }
    }
    btl_applyModelTextures(p->model, p->tex[0], t[0]);
    p->model->texAnims = arg0->texAnims;
    btl_relocTexAnims((TexAnimSet *)p->model->texAnims, (u16 (*)[4])p->texPos);
    return (u16)p->id;
}

void Hi_DrawEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    btl_setModelTransform(s->model, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, 0, g_btlfxEnv->prim);
    s->model->prims = 0;
    n = s->model->header->meshCount;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->model, i);
        if (s->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, s->otOffset);
        } else {
            switch ((u16)s->kind) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->model, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawSliceEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawSliceEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    btl_setModelTransform(s->model, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, 0, g_btlfxEnv->prim);
    s->model->prims = 0;
    s->model->sliceY = arg1->vy - s->sliceY;
    n = s->model->header->meshCount;
    for (i = 0; i < n; i++) {
        btlfx_projectSliceMesh(s->model, i);
        g_btlfxEnv->prim = btlfx_drawSliceMesh(s->model, i, g_btlfxEnv->prim);
    }
}

void Hi_DrawEffModelByBone(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg1, "Hi_DrawEffModelByBone()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(arg2, arg3, (BtlBone *)&s->model->matrix);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->model, (VECTOR *)arg0);
    }
    s->model->prims = 0;
    n = s->model->header->meshCount;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->model, i);
        if (s->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, s->otOffset);
        } else {
            switch ((u16)s->kind) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->model, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *str = "Hi_DrawMorphEffModel()\nid:%d cannot use!";
    BtlObjSlot *s = btlfx_getEffModel(arg3, str);
    s32 n;
    s32 i;
    s->model->morph = btlfx_getEffModel(arg4, str)->model->header;
    btl_setModelTransform(s->model, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, 0, g_btlfxEnv->prim);
    s->model->prims = 0;
    n = s->model->header->meshCount;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->model, i, arg5);
        if (s->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, s->otOffset);
        } else {
            switch ((u16)s->kind) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->model, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawMorphModelByBone(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *str = "Hi_DrawMorphModelByBone()\nid:%d cannot use!";
    BtlObjSlot *s = btlfx_getEffModel(arg1, str);
    s32 n;
    s32 i;
    s->model->morph = btlfx_getEffModel(arg2, str)->model->header;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(arg4, arg5, (BtlBone *)&s->model->matrix);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->model, (VECTOR *)arg0);
    }
    s->model->prims = 0;
    n = s->model->header->meshCount;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->model, i, arg3);
        if (s->otOffset != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, s->otOffset);
        } else {
            switch ((u16)s->kind) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->model, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->model, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

s32 Hi_DrawSummonModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3, s32 arg4) {
    Unk8010141C *s = btlfx_getSummonModel(arg3, "Hi_DrawSummonModel()\nid:%d cannot use!");
    BtlUnit9CC *a;
    s32 n;
    s32 f;
    s32 c;
    s32 i;
    btl_setModelTransform(s->model, arg0, arg1, arg2);
    a = s->model->motion;
    if (IS_OFFSET(a->rotKeys)) {
        a->rotKeys = (s32)a + a->rotKeys;
        if (a->rotKeysLo != 0) {
            a->rotKeysLo = (s32)s->model->motion + a->rotKeysLo;
        }
    }
    n = s->model->motion->frameCount;
    if (s->frame >= n) {
        if (arg4 & 1) {
            s->frame = 0;
        } else {
            s->frame = n - 1;
        }
    }
    if (s->texAnimOn != 0) {
        btl_updateTexAnims((TexAnimSet *)s->model->texAnims);
    }
    f = s->frame;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->model, f, g_btlfxEnv->prim);
    s->frame++;
    s->model->prims = 0;
    c = s->model->header->meshCount;
    for (i = 0; i < c; i++) {
        if (!(s->model->hiddenMeshes & (1 << i))) {
            btl_projectMesh(s->model, i);
            g_btlfxEnv->prim = btl_drawMesh(s->model, i, g_btlfxEnv->prim);
        }
    }
    return f;
}

void Hi_SetSummonMotion(BtlUnit9CC *arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg1, "Hi_SetSummonMotion()\nid:%d cannot use!");
    p->frame = 0;
    p->model->motion = arg0;
}

void Hi_SetSummonMotFrame(s32 arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, D_800A7F34);
    if (p->model->motion->frameCount < arg1) {
        p->frame = 0;
    } else {
        p->frame = arg1;
    }
}

BtlObjSlot *btlfx_allocEffModel(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        if (p->used == 0) {
            return p;
        }
    }
    return NULL;
}

Unk8010141C *btlfx_allocSummonModel(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        if (p->used == 0) {
            return p;
        }
    }
    return NULL;
}

void func_800DC500(void) {
}

void func_800DC508(void) {
}

void btlfx_halt(char *arg0) {
    g_btlfxHaltBufs.unkE8 = &g_btlfxHaltBufs.unk0[1];
    g_btlfxHaltBufs.unk0[0].unk70 = 0;
    g_btlfxHaltBufs.unk0[1].unk70 = 1;
    SetDefDrawEnv(&g_btlfxHaltBufs.unk0[0].unk0, 0, 0, 320, 240);
    SetDefDrawEnv(&g_btlfxHaltBufs.unk0[1].unk0, 0, 240, 320, 240);
    SetDefDispEnv(&g_btlfxHaltBufs.unk0[0].unk5C, 0, 240, 320, 240);
    SetDefDispEnv(&g_btlfxHaltBufs.unk0[1].unk5C, 0, 0, 320, 240);
    g_btlfxHaltBufs.unk0[1].unk0.isbg = 1;
    g_btlfxHaltBufs.unk0[0].unk0.isbg = 1;
    setRGB0(&g_btlfxHaltBufs.unk0[0].unk0, 0, 0, 0);
    setRGB0(&g_btlfxHaltBufs.unk0[1].unk0, 0, 0, 0);
    EnterCriticalSection();
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    ExitCriticalSection();
    SetDispMask(1);
    while (1) {
        g_btlfxHaltBufs.unkE8 = (g_btlfxHaltBufs.unkE8 == &g_btlfxHaltBufs.unk0[0]) ? &g_btlfxHaltBufs.unk0[1] : &g_btlfxHaltBufs.unk0[0];
        DrawSync(0);
        VSync(0);
        PutDispEnv(&g_btlfxHaltBufs.unkE8->unk5C);
        PutDrawEnv(&g_btlfxHaltBufs.unkE8->unk0);
    }
}

void btlfx_error(char *arg0) {
    char buf[0x100];
    sprintf(buf, D_800A7F60, arg0);
    btlfx_halt(buf);
}

void btlfx_initEffModels(BtlModel *arg0, s32 arg1) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    s32 j;
    for (i = 0; i < 32; i++, p++) {
        p->used = 0;
        p->id = i;
        p->model = NULL;
        for (j = 0; j < 6; j++) {
            p->tex.unk0[j][0] = 0;
            p->tex.unk0[j][1] = 0;
        }
    }
    p = g_btlfxEffModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->model = &arg0->unk0;
    }
}

void Hi_FreeEffModel(s32 arg0) {
    btlfx_getEffModel(arg0, D_800A7F78)->used = 0;
}

void btlfx_clearEffModels(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        p->used = 0;
        p->id = i;
        p->model = NULL;
    }
}

void btlfx_initSummonModels(BtlModel *arg0, s32 arg1) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    s32 j;
    for (i = 0; i < 1; i++, p++) {
        p->used = 0;
        p->texAnimOn = 0;
        p->id = i;
        p->model = NULL;
        for (j = 0; j < 6; j++) {
            p->tex[j][0] = 0;
            p->tex[j][1] = 0;
            p->texPos[j][0] = 0;
            p->texPos[j][1] = 0;
            p->texPos[j][2] = 0;
            p->texPos[j][3] = 0;
        }
    }
    p = g_btlfxSummonModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->model = &arg0->unk0;
    }
}

void btlfx_clearSummonModels(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        p->used = 0;
        p->texAnimOn = 0;
        p->id = i;
        p->model = NULL;
    }
}

void btlfx_getUnitBonePos(s32 arg0, s32 arg1, u16 *arg2) {
    btl_getBonePos(btl_getUnit(arg0)->model, arg1, arg2);
}

void btlfx_getUnitArgBonePos(s32 arg0, s32 arg1, u16 *arg2) {
    btl_getBonePos(btl_getUnit(arg0)->model, g_btlfxEnv->targets->unk25[arg1], arg2);
}

void btlfx_getUnitBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2) {
    btl_getBoneMatrix(btl_getUnit(arg0)->model, arg1, arg2);
}

void btlfx_getUnitArgBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2) {
    btl_getBoneMatrix(btl_getUnit(arg0)->model, g_btlfxEnv->targets->unk25[arg1], arg2);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F34);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F60);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F78);

void Hi_GetSummonBonePos(s32 arg0, s32 arg1, u16 *arg2) {
    btl_getBonePos(btlfx_getSummonModel(arg0, "Hi_GetSummonBonePos()\nid:%d cannot use!")->model, arg1, arg2);
}

void Hi_GetSummonBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2) {
    btl_getBoneMatrix(btlfx_getSummonModel(arg0, "Hi_GetSummonBoneMatrix()\nid:%d cannot use!")->model, arg1, arg2);
}

void btl_getBonePos(BtlUnit9C *arg0, s32 arg1, u16 *arg2) {
    arg2[0] = arg0->bones[arg1].tx;
    arg2[1] = arg0->bones[arg1].ty;
    arg2[2] = arg0->bones[arg1].tz;
}

void btl_getBoneMatrix(BtlUnit9C *arg0, s32 arg1, BtlBone *arg2) {
    *arg2 = arg0->bones[arg1];
}

void btl_setModelTransform(BtlUnit9C *arg0, SVECTOR *arg1, VECTOR *arg2, VECTOR *arg3) {
    SVECTOR r;
    VECTOR t;
    if (arg1 != NULL) {
        math_rotMatrixYXZ(arg1, &arg0->matrix);
    } else {
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        math_rotMatrixYXZ(&r, &arg0->matrix);
    }
    if (arg2 != NULL) {
        TransMatrix(&arg0->matrix, arg2);
    } else {
        t.vx = 0;
        t.vy = 0;
        t.vz = 0;
        TransMatrix(&arg0->matrix, &t);
    }
    if (arg3 != 0) {
        btl_scaleModelMatrix(arg0, arg3);
    } else {
        arg0->scaleX = 0x1000;
        arg0->scaleY = 0x1000;
        arg0->scaleZ = 0x1000;
        btl_applyModelScale(arg0);
    }
}

/* scales the model matrix by arg1 (MulMatrix0 with a diagonal matrix built word by word) */
void btl_scaleModelMatrix(BtlUnit9C *arg0, VECTOR *arg1) {
    MATRIX m;
    s32 *d;
    s16 x, y, z;

    d = (s32 *)&m;
    x = arg1->vx;
    d[0] = 0x1000;
    m.m[0][0] = x;
    y = arg1->vy;
    d[2] = 0x1000;
    m.m[1][1] = y;
    z = arg1->vz;
    d[1] = 0;
    d[3] = 0;
    d[4] = 0x1000;
    d[5] = 0;
    d[6] = 0;
    d[7] = 0;
    m.m[2][2] = z;
    gte_MulMatrix0(&arg0->matrix, &m, &arg0->matrix);
}

void Hi_ShowSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_ShowSummonModelMesh()\nid:%d cannot use!")->model->hiddenMeshes &= ~(1 << arg1);
}

void Hi_HideSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_HideSummonModelMesh()\nid:%d cannot use!")->model->hiddenMeshes |= 1 << arg1;
}

/* starts texture animation arg1 of summon model arg0 (arg2: also set flag 2) */
void Hi_StartSummonTexAnim(s32 arg0, s32 arg1, u8 arg2) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, "Hi_StartSummonTexAnim()\nid:%d cannot use!");
    s32 off;

    if (arg2) {
        off = arg1 * 4;
        off = (off + arg1) * 4; /* arg1 * sizeof(BtlUnit9C50) */
        ((BtlUnit9C50 *)((u8 *)p->model->texAnims + off))->flags |= 3;
    } else {
        off = arg1 * 4;
        off = (off + arg1) * 4;
        ((BtlUnit9C50 *)((u8 *)p->model->texAnims + off))->flags |= 1;
    }
    ((BtlUnit9C50 *)((u8 *)p->model->texAnims + off))->pos = 0;
    ((BtlUnit9C50 *)((u8 *)p->model->texAnims + off))->lastFrame = 0x1000;
}

void Hi_StopSummonTexAnim(s32 arg0, s32 arg1) {
    BtlUnit9C50 *e = &btlfx_getSummonModel(arg0, "Hi_StopSummonTexAnim()\nid:%d cannot use!")->model->texAnims[arg1];
    e->flags &= ~3;
}

void Hi_ModifyEffModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getEffModel(arg0, "Hi_ModifyEffModelAbr()\nid:%d cannot use!")->model, (u16)(arg1 << 5));
    }
}

void Hi_ModifyEffModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getEffModel(arg0, "Hi_ModifyEffModelRGB()\nid:%d cannot use!")->model, arg1, arg2, arg3);
}

void Hi_SetEffModelOffset(s32 arg0, s16 arg1) {
    btlfx_getEffModel(arg0, "Hi_SetEffModelOffset()\nid:%d cannot use!")->otOffset = arg1;
}

void Hi_SetEffModelSlice(s32 arg0, s16 arg1) {
    BtlObjSlot *p = btlfx_getEffModel(arg0, "Hi_SetEffModelSlice()\nid:%d cannot use!");
    p->slice = 1;
    p->sliceY = arg1;
}

void Hi_ModifySummonModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelAbr()\nid:%d cannot use!")->model, (u16)(arg1 << 5));
    }
}

void Hi_ModifySummonModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelRGB()\nid:%d cannot use!")->model, arg1, arg2, arg3);
}

BtlObjSlot *btlfx_getEffModel(s32 arg0, char *arg1) {
    BtlObjSlot *p = &g_btlfxEffModels[arg0];
    if (p->used == 0 || p->model == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

Unk8010141C *btlfx_getSummonModel(s32 arg0, char *arg1) {
    Unk8010141C *p = &g_btlfxSummonModels[arg0];
    if (p->used == 0 || p->model == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

void btlfx_getUnitMeshCount(s32 arg0) {
    btl_getMeshCount(btl_getUnit(arg0)->model);
}

/* Void in the original; effect modules read $v0 after it (export 0x318): btl_getMeshCount's u8 result.
   Ports must return it (docs/effect-modules.md). */
void btlfx_getEffModelMeshCount(s32 arg0) {
    btl_getMeshCount(btlfx_getEffModel(arg0, D_800A81A8)->model);
}

u8 btl_getMeshCount(BtlUnit9C *arg0) {
    return arg0->header->meshCount;
}

void btlfx_getCasterTexInfo(BtlTexInfo *arg0, BtlTexInfo *arg1, s32 arg2) {
    s32 k;
    BtlTexInfo *a;
    BtlTexInfo *b;
    u16 i;
    Unk8E0 *e;

    i = (btl_getUnit(0x10)->flags >> 1) & 0xF;
    e = &g_gameState->gameData->unk8E0[i];
    k = arg2 & 0xFFFF;
    k |= 0x20000;
    k |= e->unkC->unk3B << 24;
    a = (BtlTexInfo *)db_getModelTexPages(k);
    b = (BtlTexInfo *)db_getModelUvOffsets(k);
    *arg0 = *a;
    *arg1 = *b;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A81A8);

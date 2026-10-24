#include "ovl_0b.h"

u16 Hi_RegisterSolidEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSolidEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 0;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    btl_applyModelTextures(p->unk0, NULL, NULL);
    return p->unk1E;
}

u16 Hi_RegisterGouEffModel(BtlUnit9C8 *arg0) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterGouEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 1;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    btl_applyModelTextures(p->unk0, NULL, NULL);
    return p->unk1E;
}

u16 Hi_RegisterTexEffModel(BtlUnit9C8 *arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4) {
    s16 v[2];
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexEffModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    p->unk4.unk0[0][0] = arg1;
    p->unk4.unk0[0][1] = arg2;
    v[0] = arg3;
    v[1] = arg4;
    btl_applyModelTextures(p->unk0, p->unk4.unk0[0], v);
    return p->unk1E;
}

u16 Hi_RegisterTexListModel(BtlUnit9C8 *arg0, BtlTexInfo *arg1, s16 *arg2, s32 arg3) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexListModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    p->unk4 = *arg1;
    btl_applyModelTextures(p->unk0, p->unk4.unk0[0], arg2);
    return p->unk1E;
}

u16 Hi_RegisterTexPtrModel(BtlUnit9C8 *arg0, s16 *arg1, s16 *arg2) {
    BtlObjSlot *p = btlfx_allocEffModel();
    if (p == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterTexPtrModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk1C = 1;
    p->unk1D = 0;
    p->unk20 = 2;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk0->unk8 = arg0;
    p->unk0->unkC = NULL;
    btl_applyModelTextures(p->unk0, arg1, arg2);
    return p->unk1E;
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
    if (p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, "Hi_RegisterSummonModel()\nmemory not enough!");
        btlfx_error(g_btlfxErrorMsg);
    }
    p->unk4C = 1;
    p->unk50 = 0;
    p->unk0->unk8 = (BtlUnit9C8 *)arg0->unk3C;
    p->unk0->unkC = (BtlUnit9CC *)arg0->unk180[0];
    if (arg1 != NULL) {
        p->unk4D = 1;
    } else {
        p->unk4D = 0;
    }
    for (i = 0; i < arg0->unk4; i++) {
        p->unk4[i][0] = arg0->unk18[i];
        p->unk4[i][1] = arg0->unk24[i];
        t[i][0] = 0;
        t[i][1] = arg0->unk30[i];
        if (arg1 != NULL) {
            p->unk1C[i][0] = arg1[0];
            p->unk1C[i][1] = arg1[1];
            p->unk1C[i][2] = arg1[2];
            p->unk1C[i][3] = arg1[3];
            arg1 += 4;
        }
    }
    btl_applyModelTextures(p->unk0, p->unk4[0], t[0]);
    p->unk0->unk50 = arg0->unk40;
    btl_relocTexAnims((TexAnimSet *)p->unk0->unk50, (u16 (*)[4])p->unk1C);
    return (u16)p->unk4E;
}

void Hi_DrawEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->prim);
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->unk0, i);
        if (s->unk22 != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->unk0, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, 0);
                break;
            }
        }
    }
}

void Hi_DrawSliceEffModel(SVECTOR *arg0, VECTOR *arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg3, "Hi_DrawSliceEffModel()\nid:%d cannot use!");
    s32 n;
    s32 i;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->prim);
    s->unk0->unk10 = 0;
    s->unk0->unk5A = arg1->vy - s->unk24;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectSliceMesh(s->unk0, i);
        g_btlfxEnv->prim = func_800DE11C(s->unk0, i, g_btlfxEnv->prim);
    }
}

void Hi_DrawEffModelByBone(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    BtlObjSlot *s = btlfx_getEffModel(arg1, "Hi_DrawEffModelByBone()\nid:%d cannot use!");
    s32 n;
    s32 i;
    s32 v;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(arg2, arg3, (BtlBone *)&s->unk0->unk24);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->unk0, (VECTOR *)arg0);
    }
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btl_projectMesh(s->unk0, i);
        if (s->unk22 != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->unk0, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, 0);
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
    s->unk0->unk5C = btlfx_getEffModel(arg4, str)->unk0->unk8;
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->prim);
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->unk0, i, arg5);
        if (s->unk22 != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->unk0, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, 0);
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
    s->unk0->unk5C = btlfx_getEffModel(arg2, str)->unk0->unk8;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, 0, g_btlfxEnv->prim);
    Hi_GetSummonBoneMatrix(arg4, arg5, (BtlBone *)&s->unk0->unk24);
    if (arg0 != 0) {
        btl_scaleModelMatrix(s->unk0, (VECTOR *)arg0);
    }
    s->unk0->unk10 = 0;
    n = s->unk0->unk8->unk3;
    for (i = 0; i < n; i++) {
        btlfx_projectMorphMesh(s->unk0, i, arg3);
        if (s->unk22 != 0) {
            g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, s->unk22);
        } else {
            switch ((u16)s->unk20) {
            case 0:
            case 2:
                g_btlfxEnv->prim = btl_drawMesh(s->unk0, i, g_btlfxEnv->prim);
                break;
            case 1:
                g_btlfxEnv->prim = btl_drawMeshOffset(s->unk0, i, g_btlfxEnv->prim, 0);
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
    btl_setModelTransform(s->unk0, arg0, arg1, arg2);
    a = s->unk0->unkC;
    if (IS_OFFSET(a->unkC)) {
        a->unkC = (s32)a + a->unkC;
        if (a->unk10 != 0) {
            a->unk10 = (s32)s->unk0->unkC + a->unk10;
        }
    }
    n = s->unk0->unkC->unk2;
    if (s->unk50 >= n) {
        if (arg4 & 1) {
            s->unk50 = 0;
        } else {
            s->unk50 = n - 1;
        }
    }
    if (s->unk4D != 0) {
        btl_updateTexAnims((TexAnimSet *)s->unk0->unk50);
    }
    f = s->unk50;
    g_btlfxEnv->prim = btl_calcBoneMatrices(s->unk0, f, g_btlfxEnv->prim);
    s->unk50++;
    s->unk0->unk10 = 0;
    c = s->unk0->unk8->unk3;
    for (i = 0; i < c; i++) {
        if (!(s->unk0->unk14 & (1 << i))) {
            btl_projectMesh(s->unk0, i);
            g_btlfxEnv->prim = btl_drawMesh(s->unk0, i, g_btlfxEnv->prim);
        }
    }
    return f;
}

void Hi_SetSummonMotion(BtlUnit9CC *arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg1, "Hi_SetSummonMotion()\nid:%d cannot use!");
    p->unk50 = 0;
    p->unk0->unkC = arg0;
}

void Hi_SetSummonMotFrame(s32 arg0, s32 arg1) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, D_800A7F34);
    if (p->unk0->unkC->unk2 < arg1) {
        p->unk50 = 0;
    } else {
        p->unk50 = arg1;
    }
}

BtlObjSlot *btlfx_allocEffModel(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        if (p->unk1C == 0) {
            return p;
        }
    }
    return NULL;
}

Unk8010141C *btlfx_allocSummonModel(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        if (p->unk4C == 0) {
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
    D_80100E30.unkE8 = &D_80100E30.unk0[1];
    D_80100E30.unk0[0].unk70 = 0;
    D_80100E30.unk0[1].unk70 = 1;
    SetDefDrawEnv(&D_80100E30.unk0[0].unk0, 0, 0, 320, 240);
    SetDefDrawEnv(&D_80100E30.unk0[1].unk0, 0, 240, 320, 240);
    SetDefDispEnv(&D_80100E30.unk0[0].unk5C, 0, 240, 320, 240);
    SetDefDispEnv(&D_80100E30.unk0[1].unk5C, 0, 0, 320, 240);
    D_80100E30.unk0[1].unk0.isbg = 1;
    D_80100E30.unk0[0].unk0.isbg = 1;
    setRGB0(&D_80100E30.unk0[0].unk0, 0, 0, 0);
    setRGB0(&D_80100E30.unk0[1].unk0, 0, 0, 0);
    EnterCriticalSection();
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    ExitCriticalSection();
    SetDispMask(1);
    while (1) {
        D_80100E30.unkE8 = (D_80100E30.unkE8 == &D_80100E30.unk0[0]) ? &D_80100E30.unk0[1] : &D_80100E30.unk0[0];
        DrawSync(0);
        VSync(0);
        PutDispEnv(&D_80100E30.unkE8->unk5C);
        PutDrawEnv(&D_80100E30.unkE8->unk0);
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
        p->unk1C = 0;
        p->unk1E = i;
        p->unk0 = NULL;
        for (j = 0; j < 6; j++) {
            p->unk4.unk0[j][0] = 0;
            p->unk4.unk0[j][1] = 0;
        }
    }
    p = g_btlfxEffModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->unk0 = &arg0->unk0;
    }
}

void Hi_FreeEffModel(s32 arg0) {
    btlfx_getEffModel(arg0, D_800A7F78)->unk1C = 0;
}

void btlfx_clearEffModels(void) {
    BtlObjSlot *p = g_btlfxEffModels;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        p->unk1C = 0;
        p->unk1E = i;
        p->unk0 = NULL;
    }
}

void btlfx_initSummonModels(BtlModel *arg0, s32 arg1) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    s32 j;
    for (i = 0; i < 1; i++, p++) {
        p->unk4C = 0;
        p->unk4D = 0;
        p->unk4E = i;
        p->unk0 = NULL;
        for (j = 0; j < 6; j++) {
            p->unk4[j][0] = 0;
            p->unk4[j][1] = 0;
            p->unk1C[j][0] = 0;
            p->unk1C[j][1] = 0;
            p->unk1C[j][2] = 0;
            p->unk1C[j][3] = 0;
        }
    }
    p = g_btlfxSummonModels;
    for (i = 0; i < arg1; i++, p++, arg0++) {
        p->unk0 = &arg0->unk0;
    }
}

void btlfx_clearSummonModels(void) {
    Unk8010141C *p = g_btlfxSummonModels;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        p->unk4C = 0;
        p->unk4D = 0;
        p->unk4E = i;
        p->unk0 = NULL;
    }
}

void func_800DC818(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btl_getUnit(arg0)->model, arg1, arg2);
}

void func_800DC858(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btl_getUnit(arg0)->model, g_btlfxEnv->targets->unk25[arg1], arg2);
}

void func_800DC8AC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btl_getUnit(arg0)->model, arg1, arg2);
}

void func_800DC8EC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btl_getUnit(arg0)->model, g_btlfxEnv->targets->unk25[arg1], arg2);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F34);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F60);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A7F78);

void Hi_GetSummonBonePos(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(btlfx_getSummonModel(arg0, "Hi_GetSummonBonePos()\nid:%d cannot use!")->unk0, arg1, arg2);
}

void Hi_GetSummonBoneMatrix(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(btlfx_getSummonModel(arg0, "Hi_GetSummonBoneMatrix()\nid:%d cannot use!")->unk0, arg1, arg2);
}

void func_800DC9D0(BtlUnit9C *arg0, s32 arg1, u16 *arg2) {
    arg2[0] = arg0->unk20[arg1].unk14;
    arg2[1] = arg0->unk20[arg1].unk18;
    arg2[2] = arg0->unk20[arg1].unk1C;
}

void func_800DCA18(BtlUnit9C *arg0, s32 arg1, BtlBone *arg2) {
    *arg2 = arg0->unk20[arg1];
}

void btl_setModelTransform(BtlUnit9C *arg0, SVECTOR *arg1, VECTOR *arg2, VECTOR *arg3) {
    SVECTOR r;
    VECTOR t;
    if (arg1 != NULL) {
        func_800316D8(arg1, &arg0->unk24);
    } else {
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        func_800316D8(&r, &arg0->unk24);
    }
    if (arg2 != NULL) {
        TransMatrix(&arg0->unk24, arg2);
    } else {
        t.vx = 0;
        t.vy = 0;
        t.vz = 0;
        TransMatrix(&arg0->unk24, &t);
    }
    if (arg3 != 0) {
        btl_scaleModelMatrix(arg0, arg3);
    } else {
        arg0->unk54 = 0x1000;
        arg0->unk56 = 0x1000;
        arg0->unk58 = 0x1000;
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
    gte_MulMatrix0(&arg0->unk24, &m, &arg0->unk24);
}

void Hi_ShowSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_ShowSummonModelMesh()\nid:%d cannot use!")->unk0->unk14 &= ~(1 << arg1);
}

void Hi_HideSummonModelMesh(s32 arg0, s32 arg1) {
    btlfx_getSummonModel(arg0, "Hi_HideSummonModelMesh()\nid:%d cannot use!")->unk0->unk14 |= 1 << arg1;
}

/* starts texture animation arg1 of summon model arg0 (arg2: also set flag 2) */
void Hi_StartSummonTexAnim(s32 arg0, s32 arg1, u8 arg2) {
    Unk8010141C *p = btlfx_getSummonModel(arg0, "Hi_StartSummonTexAnim()\nid:%d cannot use!");
    s32 off;

    if (arg2) {
        off = arg1 * 4;
        off = (off + arg1) * 4; /* arg1 * sizeof(BtlUnit9C50) */
        ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk4 |= 3;
    } else {
        off = arg1 * 4;
        off = (off + arg1) * 4;
        ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk4 |= 1;
    }
    ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unkC = 0;
    ((BtlUnit9C50 *)((u8 *)p->unk0->unk50 + off))->unk12 = 0x1000;
}

void Hi_StopSummonTexAnim(s32 arg0, s32 arg1) {
    BtlUnit9C50 *e = &btlfx_getSummonModel(arg0, "Hi_StopSummonTexAnim()\nid:%d cannot use!")->unk0->unk50[arg1];
    e->unk4 &= ~3;
}

void Hi_ModifyEffModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getEffModel(arg0, "Hi_ModifyEffModelAbr()\nid:%d cannot use!")->unk0, (u16)(arg1 << 5));
    }
}

void Hi_ModifyEffModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getEffModel(arg0, "Hi_ModifyEffModelRGB()\nid:%d cannot use!")->unk0, arg1, arg2, arg3);
}

void Hi_SetEffModelOffset(s32 arg0, s16 arg1) {
    btlfx_getEffModel(arg0, "Hi_SetEffModelOffset()\nid:%d cannot use!")->unk22 = arg1;
}

void Hi_SetEffModelSlice(s32 arg0, s16 arg1) {
    BtlObjSlot *p = btlfx_getEffModel(arg0, "Hi_SetEffModelSlice()\nid:%d cannot use!");
    p->unk1D = 1;
    p->unk24 = arg1;
}

void Hi_ModifySummonModelAbr(s32 arg0, s32 arg1) {
    if (arg1 != 0xFF) {
        btl_setModelAbr(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelAbr()\nid:%d cannot use!")->unk0, (u16)(arg1 << 5));
    }
}

void Hi_ModifySummonModelRGB(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    btl_setModelRgb(btlfx_getSummonModel(arg0, "Hi_ModifySummonModelRGB()\nid:%d cannot use!")->unk0, arg1, arg2, arg3);
}

BtlObjSlot *btlfx_getEffModel(s32 arg0, char *arg1) {
    BtlObjSlot *p = &g_btlfxEffModels[arg0];
    if (p->unk1C == 0 || p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

Unk8010141C *btlfx_getSummonModel(s32 arg0, char *arg1) {
    Unk8010141C *p = &g_btlfxSummonModels[arg0];
    if (p->unk4C == 0 || p->unk0 == NULL) {
        sprintf(g_btlfxErrorMsg, arg1, arg0);
        btlfx_error(g_btlfxErrorMsg);
    }
    return p;
}

void func_800DD0B0(s32 arg0) {
    func_800DD10C(btl_getUnit(arg0)->model);
}

/* Void in the original; effect modules read $v0 after it (export 0x318): func_800DD10C's u8 result.
   Ports must return it (docs/effect-modules.md). */
void func_800DD0DC(s32 arg0) {
    func_800DD10C(btlfx_getEffModel(arg0, D_800A81A8)->unk0);
}

u8 func_800DD10C(BtlUnit9C *arg0) {
    return arg0->unk8->unk3;
}

void func_800DD120(BtlTexInfo *arg0, BtlTexInfo *arg1, s32 arg2) {
    s32 k;
    BtlTexInfo *a;
    BtlTexInfo *b;
    u16 i;
    Unk8E0 *e;

    i = (btl_getUnit(0x10)->unk68 >> 1) & 0xF;
    e = &g_gameState->unk1C->unk8E0[i];
    k = arg2 & 0xFFFF;
    k |= 0x20000;
    k |= e->unkC->unk3B << 24;
    a = (BtlTexInfo *)func_800634A8(k);
    b = (BtlTexInfo *)func_800634D8(k);
    *arg0 = *a;
    *arg1 = *b;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_model", D_800A81A8);

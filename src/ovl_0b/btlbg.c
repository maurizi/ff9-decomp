#include "ovl_0b.h"

void btlbg_init(BtlBgHeader *header, BtlUnit9C50 *texAnims) {
    s32 unused[4];
    s32 i;
    g_btlbgHeader = header;
    g_btlbgScrolls = header->scrolls;
    g_btlbgId = header->id;
    btlbg_setOtShift(0);
    g_btlbgSkyOwnProj = 0;
    if (g_btlbgHeader->skyOwnProj != 0) {
        g_btlbgSkyOwnProj = 1;
    }
    g_btlbgSkySpeed = g_btlbgHeader->skySpeed;
    g_btlbgTexAnims = texAnims;
    g_btlbgTexAnimCount = g_btlbgHeader->texAnimCount;
    if (g_btlbgTexAnimCount != 0) {
        btl_relocTexAnims((TexAnimSet *)texAnims, NULL);
        for (i = 0; i < g_btlbgTexAnimCount; i++) {
            BtlUnit9C50 *anim;
            BtlUnit9C50 *animRef;
            anim = g_btlbgTexAnims + i;
            anim->flags |= BTL_TEXANIM_PLAY;
            animRef = g_btlbgTexAnims + i;
            animRef->pos = 0;
            animRef->lastFrame = 0x1000;
        }
    }
    g_btlbgScrollCount = g_btlbgHeader->scrollCount;
    btlbg_setLayerMask(BTLBG_SHOW_ALL);
    g_btlbgSkyAngle = 0;
    g_btlbgRainCount = g_gameState->gameData->rainCount;
    if (g_btlbgRainCount >= 0x1F) {
        g_btlbgRainCount = 0x1F;
    }
}

u8 *btlbg_draw(MATRIX *view, s32 bgData, s32 prim, u32 *ot, s32 bgOt) {
    MATRIX mat;
    SVECTOR rot;
    VECTOR pivotOfs;
    VECTOR trans;
    MATRIX skyMatrix;
    SVECTOR groupRot;
    SVECTOR pivot;
    s32 done;
    u8 *out;
    BtlBgGroup *grp;
    BtlBgRec *rec;
    s32 uvAddr;
    s32 angle;
    s32 tilt;
    s32 i;
    s32 groupIdx;
    s32 flag;

    out = (u8 *)prim;
    memset(&rot, 0, 8);
    memset(&groupRot, 0, 8);
    memset(&pivot, 0, 8);
    done = 0;
    g_btlbgFrame++;
    if (!(u8)btlbg_getBrightness()) {
        return out;
    }
    trans.vx = view->t[0];
    trans.vy = view->t[1];
    trans.vz = view->t[2];
    tilt = view->m[2][1];
    if (tilt < 0) {
        tilt = -tilt;
    }
    trans.vz = ((ONE - tilt) * 12500 + view->t[2] * tilt) / ONE;
    trans.vx = 0;
    TransMatrix(&skyMatrix, &trans);
    if (g_btlbgScrollPaused == 0) {
        for (i = 0; i < g_btlbgScrollCount; i++) {
            uvAddr = bgData + g_btlbgScrolls[i].uvOfs;
            ((u8 *)uvAddr)[0] += g_btlbgScrolls[i].du;
            ((u8 *)uvAddr)[1] += g_btlbgScrolls[i].dv;
            g_btlbgScrolls[i].step++;
            if (g_btlbgScrolls[i].step >= g_btlbgScrolls[i].period) {
                switch (g_btlbgScrolls[i].mode) {
                case BTLBG_SCROLL_WRAP:
                    ((u8 *)uvAddr)[0] -= g_btlbgScrolls[i].period * g_btlbgScrolls[i].du;
                    ((u8 *)uvAddr)[1] -= g_btlbgScrolls[i].period * g_btlbgScrolls[i].dv;
                    break;
                case BTLBG_SCROLL_BOUNCE:
                    g_btlbgScrolls[i].du = -g_btlbgScrolls[i].du;
                    g_btlbgScrolls[i].dv = -g_btlbgScrolls[i].dv;
                    break;
                }
                g_btlbgScrolls[i].step = 0;
            }
        }
    }
    if (g_btlbgTexAnimCount != 0) {
        btl_updateTexAnims((TexAnimSet *)g_btlbgTexAnims);
    }
    SetRotMatrix(view);
    SetTransMatrix(view);
    if (g_btlbgRainCount != 0) {
        out = btlbg_drawRain(out, ot, view);
    }
    grp = (BtlBgGroup *)bgData;
    groupIdx = 0;
    for (;;) {
        rec = (BtlBgRec *)((u8 *)grp + grp->recOfs);
        for (i = 0; i < grp->recCount; rec++, i++) {
            switch (rec->layer) {
            case BTLBG_LAYER_SKY:
                if (!done) {
                    g_btlbgSkyAngle += g_btlbgSkySpeed;
                    rot.vy = g_btlbgSkyAngle >> 3;
                    math_rotMatrixXYZ(&rot, &mat);
                    done = 1;
                    MulMatrix2(view, &mat);
                    SetRotMatrix(&mat);
                }
                if (g_btlbgLayerMask & BTLBG_SHOW_SKY) {
                    if (g_btlbgSkyOwnProj != 0) {
                        SetTransMatrix(&skyMatrix);
                        SetGeomScreen(0x200);
                        out = btlbg_drawRecord(rec, out, (u32 *)bgOt, grp);
                        SetGeomScreen(g_gameState->gameData->projDist);
                    } else {
                        out = btlbg_drawRecord(rec, out, (u32 *)bgOt, grp);
                    }
                }
                break;
            case BTLBG_LAYER_MID:
                if (g_btlbgLayerMask & BTLBG_SHOW_MID) {
                    out = btlbg_drawRecord(rec, out, (u32 *)bgOt, grp);
                }
                break;
            case BTLBG_LAYER_FRONT:
                if (g_btlbgLayerMask & BTLBG_SHOW_FRONT) {
                    out = btlbg_drawRecord(rec, out, (u32 *)bgOt, grp);
                }
                break;
            case BTLBG_LAYER_SCENE:
                if (g_btlbgLayerMask & BTLBG_SHOW_SCENE) {
                    out = btlbg_drawRecord(rec, out, ot, grp);
                }
                break;
            }
        }
        if (groupIdx == g_btlbgHeader->lastGroup) {
            return out;
        }
        groupIdx++;
        grp = (BtlBgGroup *)((u8 *)grp + grp->size);
        flag = 0;
        switch (g_btlbgHeader->id) {
        case BTLBG_SCENE_FOSSIL_ROO:
            angle = math_sin((VSync(-1) * 26) & 0xFFE);
            pivot.vx = 0x429;
            pivot.vy = -0x541;
            pivot.vz = 0xEA6;
            angle /= 5;
            groupRot.vz = angle;
            break;
        case BTLBG_SCENE_MEMORIA_OBLIVION:
            switch (groupIdx) {
            case 1:
                angle = (VSync(-1) * 5) & 0xFFF;
                groupRot.vz = 0xFFF - angle;
                angle = (VSync(-1) * 3) & 0xFFF;
                groupRot.vy = 0xFFF - angle;
                angle = math_sin(((VSync(-1) + 8) * 22) & 0xFFE);
                pivot.vx = -0x834;
                pivot.vz = -0x352;
                pivot.vy = angle / 45 - 250;
                break;
            case 2:
                groupRot.vy = 0;
                groupRot.vz = 0;
                angle = math_sin((VSync(-1) * 20) & 0xFFC);
                pivot.vx = 0x6BD;
                pivot.vy = angle / 64 - 0x5DC;
                pivot.vz = -0x4B;
                break;
            case 3:
                angle = (VSync(-1) * 4) & 0xFFF;
                groupRot.vz = angle;
                angle = (VSync(-1) * 3) & 0xFFF;
                groupRot.vy = angle;
                angle = math_sin(((VSync(-1) + 16) * 21) & 0xFFF);
                pivot.vx = 0x6D6;
                pivot.vz = 0x401;
                pivot.vy = angle / 50 - 0x307;
                break;
            }
            break;
        case BTLBG_SCENE_MEMORIA_STAIRS:
            angle = math_sin((VSync(-1) * 12) & 0xFFC);
            angle /= 64;
            groupRot.vy = 0x200;
            pivot.vx = 0x5DC;
            pivot.vy = -0x1B58;
            groupRot.vz = angle;
            pivot.vz = 0xEA6;
            break;
        case BTLBG_SCENE_MEMORIA_ORIGIN:
            groupRot.vx = 0;
            groupRot.vz = 0;
            if (groupIdx == 1) {
                groupRot.vy = (VSync(-1) / 16) & 0xFFF;
            } else {
                groupRot.vy = (VSync(-1) / 8) & 0xFFF;
            }
            break;
        case BTLBG_SCENE_CRYSTAL_WORLD:
            if (groupIdx == 1) {
                angle = -(VSync(-1) * 12) & 0xFFF;
                pivot.vy = -0x947;
                groupRot.vz = angle;
                pivot.vx = 0;
                pivot.vz = 0xEA6;
            } else {
                angle = -(VSync(-1) * 22) & 0xFFF;
                flag = 1;
                groupRot.vz = 0xE00;
                pivot.vy = -0x8CA;
                groupRot.vy = angle;
                pivot.vx = 0;
                pivot.vz = 0x1DC9;
            }
            break;
        case BTLBG_SCENE_NOVA_DRAGON:
            if (groupIdx == 1) {
                if (!((g_btlbgFrame + 0x1F) & 0x3F)) {
                    g_btlbgJitterBase1 = rand() & 0x1FF;
                }
                groupRot.vy = g_btlbgJitterBase1 + (rand() & 0x3F);
            } else {
                if (!(g_btlbgFrame & 0x3F)) {
                    g_btlbgJitterBase2 = rand() & 0x3FF;
                }
                groupRot.vy = g_btlbgJitterBase2 + (rand() & 0x7F);
            }
            break;
        default:
            pivot.vx = 0;
            pivot.vy = -10;
            pivot.vz = 0;
            groupRot.vy += 3;
            break;
        }
        ApplyMatrix(view, &pivot, &pivotOfs);
        mat.t[0] = view->t[0] + pivotOfs.vx;
        mat.t[1] = view->t[1] + pivotOfs.vy;
        mat.t[2] = view->t[2] + pivotOfs.vz;
        if (flag == 0) {
            math_rotMatrixXYZ(&groupRot, &mat);
        } else {
            math_rotMatrixZYX(&groupRot, &mat);
        }
        MulMatrix2(view, &mat);
        SetRotMatrix(&mat);
        SetTransMatrix(&mat);
    }
}

void btlbg_startRain(void) {
    g_btlbgRainCount = 0x1F;
}

void btlbg_stopRain(void) {
    g_btlbgRainCount = 0;
}

u8 *btlbg_drawRain(u8 *prim, u32 *ot, MATRIX *view) {
    SVECTOR drop;
    MATRIX mat;
    long interp;
    long flag;
    s32 i;

    SetGeomScreen(0x200);
    i = 0;
    PushMatrix();
    SetDrawStp((DR_STP *)prim, 0);
    AddPrim(&ot[2], prim);
    prim += sizeof(DR_STP);
    mat = *view;
    mat.t[0] = 0;
    mat.t[1] = 0;
    mat.t[2] = 1000;
    SetTransMatrix(&mat);
    for (; i < g_btlbgRainCount; i++) {
        SetLineG2((LINE_G2 *)prim);
        SetSemiTrans(prim, 1);
        drop.vx = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        drop.vy = ((rand() & 0xFF) * 50) / 32 - 0xDC;
        drop.vz = (((rand() & 0x1FF) - 0x100) * 41) / 32;
        RotTransPers(&drop, (long *)&((LINE_G2 *)prim)->x0, &interp, &flag);
        drop.vy += 0x55 + (rand() & 0x1F);
        RotTransPers(&drop, (long *)&((LINE_G2 *)prim)->x1, &interp, &flag);
        setRGB0((LINE_G2 *)prim, 0x19, 0x19, 0x32);
        setRGB1((LINE_G2 *)prim, 0x50, 0x50, 0x82);
        AddPrim(&ot[2], prim);
        prim += sizeof(LINE_G2);
    }
    SetDrawStp((DR_STP *)prim, 1);
    AddPrim(&ot[2], prim);
    prim += sizeof(DR_STP);
    SetDrawTPage(prim, 0, 0, GetTPage(0, 3, 0, 0));
    AddPrim(&ot[2], prim);
    PopMatrix();
    prim += sizeof(DR_TPAGE);
    SetGeomScreen(g_gameState->gameData->projDist);
    return prim;
}

BtlBgHeader *btlbg_getHeader(void) {
    return g_btlbgHeader;
}

void btlbg_pauseScroll(s32 paused) {
    g_btlbgScrollPaused = paused;
}

s32 btlbg_getId(void) {
    return g_btlbgId;
}

void btlbg_addSkySpeed(s32 delta) {
    g_btlbgSkySpeed += delta;
}

s32 btlbg_getSkySpeed(void) {
    return g_btlbgSkySpeed;
}

void btlbg_setLayerMask(s32 layers) {
    g_btlbgLayerMask = layers;
}

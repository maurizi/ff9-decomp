#include "fxmod/fx_1f2.h"

static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot) {
    Unk800F78A4 *env = fx_1f2_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bright = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f2_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

/* Matching levers: variables shared between distant blocks become long global pseudos that take callee-saved
 * registers, so most blocks use their own copies of i, j, tr, sv, alpha, lo, md and tint. Trail point stores go
 * through an SVECTOR pointer (sv->vx ... sv->pad; sv++) so loop.c combines the vy/vz/pad stores onto one giv.
 * The timer4C block: the GTE path's header pointer pt is assigned in both paths, so it is no giv (find_givs wants
 * a single set): it stays off + base, with sv = pt + 4, as in the original. The loop's insn count decides
 * move_movables' threshold test (threshold * savings * lifetime >= insn_count, threshold 29 - 3 per move,
 * insn_count doubled for a register already moved by an inner loop); the GTE x offset written j * 0x60 (a giv
 * whose multiply is deleted after loop.c) and one drawTrail call in the pad path keep 0x1000 and the pad's 0x80
 * in the loop. The second sparks loop passes &fx_1f2_trails[i] (not tr_sp2) to drawTrail, which puts the
 * hoisted fx_1f2_trails in the tr register and copies it to the base register. */
#define FX1F2_DRAW(m) ex->Hi_DrawEffModel(&(m).rot8, &(m).pos10, (s32)&(m).scale20, (m).model0)

s32 fx_1f2_op80(s32 mode, Fx1f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    MATRIX m;
    SVECTOR in[2];
    VECTOR out[2];
    VECTOR step;
    s32 flag;
    s32 flag2;
    s32 frame;
    Pool6CEntry *emitter;
    Fx1f2Trail *tr;
    Fx1f2Ring *rg;
    Fx1f2Model *md;
    SVECTOR *sv;
    s32 i;
    s32 j;
    s32 a;
    s32 r;
    s32 c;
    s32 lo;
    s32 hi;
    s32 alpha;
    s32 n;
    s32 tint;
    s32 j_m1s;
    s32 lo_m1s;
    s32 a_m1s;
    s32 lo_r0;
    s32 lo_md;
    Fx1f2Model *md_r28;
    Fx1f2Trail *tr_sp;
    s32 lo_sp;
    Fx1f2Trail *tr_sp2;
    SVECTOR *sv_sp2;
    s32 alpha_u4c;
    s32 j_u4ce;
    s32 alpha_u4cn;
    s32 j_u58;
    s32 alpha_u58;
    s32 a_u58;
    SVECTOR *sv_u58;
    s32 n_u58;
    s32 k8;
    s32 fd;
    Fx1f2Model *mm;
    VECTOR *q;
    VECTOR *q2;

    if (mode == 0) {
        *args = 0x128;
    } else if (mode == 1) {
        env->camera->points[1].vx = 0;
        env->camera->points[1].vz = -0x4000;
        env->camera->points[1].vy = -0x6000;
        fx_1f2_env = env;
        ex->btlfx_initEffModels(fx_1f2_effModels, 0x20);
        for (i = 0; i < 0x20; i++) {
            ((s32 *)work)[i] = 0;
            ((s32 *)work)[i + 0x20] = 0;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&fx_1f2_unitPos[i]);
            fx_1f2_unitPos[i].vy = 0;
        }
        ex->btlfx_getUnitBonePos(0x10, 5, (u16 *)&work->bonePos110[0]);
        ex->btlfx_getUnitBonePos(0x10, 9, (u16 *)&work->bonePos110[1]);
        work->pos120.vx = work->bonePos110[0].vx;
        work->pos120.vz = work->bonePos110[0].vz - 0x200;
        work->pos120.vy = (work->bonePos110[0].vy + work->bonePos110[1].vy) >> 1;
        fx_1f2_emitPos0.vx = 0;
        fx_1f2_emitPos0.vy = 0;
        fx_1f2_emitPos0.vz = 0;
        fx_1f2_emitPos1.vx = 0;
        fx_1f2_emitPos1.vy = 0;
        fx_1f2_emitPos1.vz = 0;
        for (i = 0; i < 0x10; i++) {
            fx_1f2_trails[i].timer4C = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_1f2_trails[i], 0);
        }
        for (i = 0; i < 8; i++) {
            fx_1f2_trails[i].timer4C = ex->btlfx_rand() & 0xF;
            for (j_m1s = 0; j_m1s < 0x10;) {
                if (i & 1) {
                    a_m1s = j_m1s << 8;
                    c = 1;
                    r = (ex->btlfx_rand() & 0xFF) + 0x17E;
                } else {
                    a_m1s = 0x1000 - (j_m1s << 8);
                    c = 0;
                    r = (ex->btlfx_rand() & 0xFF) + 0x200;
                }
                lo_m1s = (ex->math_sin(a_m1s) * r) >> 12;
                hi = ex->math_cos(a_m1s) * r;
                fx_1f2_trails[i].pos50[j_m1s].vx = work->bonePos110[c].vx - lo_m1s;
                fx_1f2_trails[i].pos50[j_m1s].vz = work->bonePos110[c].vz - (hi >> 12);
                fx_1f2_trails[i].pos50[j_m1s].vy = work->bonePos110[c].vy + (ex->btlfx_rand() & 0xFF) - 0x80;
                fx_1f2_trails[i].pos50[j_m1s].pad = j_m1s + 1;
                j_m1s++;
            }
            fx_1f2_trails[i].pos50[15].pad = 0;
        }
        fx_1f2_model0.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x84), 0xFF);
        setVector(&fx_1f2_model0.rot8, 0, 0, 0);
        setVector(&fx_1f2_model0.pos10, 0, 0, 0);
        setVector(&fx_1f2_model0.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model1.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 0xFF);
        setVector(&fx_1f2_model1.rot8, 0, 0, 0);
        setVector(&fx_1f2_model1.pos10, 0, 0, 0);
        setVector(&fx_1f2_model1.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model2.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x89), 1);
        setVector(&fx_1f2_model2.rot8, 0, 0, 0);
        setVector(&fx_1f2_model2.pos10, 0, 0, 0);
        setVector(&fx_1f2_model2.scale20, 0xD00, 0xD00, 0xD00);
        fx_1f2_model3.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x88), 1);
        setVector(&fx_1f2_model3.rot8, 0, 0, 0);
        setVector(&fx_1f2_model3.pos10, 0, 0, 0);
        setVector(&fx_1f2_model3.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model4[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 0xFF);
        fx_1f2_model4[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8A), 0xFF);
        fx_1f2_model4[0].radius4 = 0xC00;
        fx_1f2_model4[1].radius4 = 0xC00;
        setVector(&fx_1f2_model4[0].rot8, 0, 0, 0);
        setVector(&fx_1f2_model4[1].rot8, 0, 0, 0x800);
        setVector(&fx_1f2_model4[0].pos10, 0, 0, 0);
        setVector(&fx_1f2_model4[1].pos10, 0, 0, 0);
        setVector(&fx_1f2_model4[0].scale20, 0xC00, 0xC00, 0xC00);
        setVector(&fx_1f2_model4[1].scale20, 0xC00, 0xC00, 0xC00);
        fx_1f2_model5.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8C), 1);
        setVector(&fx_1f2_model5.rot8, 0, 0, 0);
        setVector(&fx_1f2_model5.pos10, 0, 0, 0);
        setVector(&fx_1f2_model5.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model6[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        fx_1f2_model6[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        setVector(&fx_1f2_model6[0].rot8, 0, 0, 0);
        setVector(&fx_1f2_model6[1].rot8, 0, 0, 0x800);
        setVector(&fx_1f2_model6[0].pos10, 0, 0, 0);
        setVector(&fx_1f2_model6[1].pos10, 0, 0, 0);
        setVector(&fx_1f2_model6[0].scale20, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1f2_model6[1].scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model8[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        fx_1f2_model8[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        fx_1f2_model8[2].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        fx_1f2_model8[3].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        setVector(&fx_1f2_model8[0].rot8, 0, 0, 0);
        setVector(&fx_1f2_model8[1].rot8, 0, 0, 0x800);
        setVector(&fx_1f2_model8[2].rot8, 0, 0, 0);
        setVector(&fx_1f2_model8[3].rot8, 0, 0, 0x800);
        setVector(&fx_1f2_model8[0].pos10, 0, 0, 0);
        setVector(&fx_1f2_model8[1].pos10, 0, 0, 0);
        setVector(&fx_1f2_model8[2].pos10, 0, 0, 0);
        setVector(&fx_1f2_model8[3].pos10, 0, 0, 0);
        setVector(&fx_1f2_model8[0].scale20, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1f2_model8[1].scale20, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1f2_model8[2].scale20, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1f2_model8[3].scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model7.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8E), 1);
        setVector(&fx_1f2_model7.rot8, 0, 0, 0);
        setVector(&fx_1f2_model7.pos10, 0, 0, 0);
        setVector(&fx_1f2_model7.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model9.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xF), 1);
        setVector(&fx_1f2_model9.rot8, 0, 0, 0);
        setVector(&fx_1f2_model9.pos10, 0, 0, 0);
        setVector(&fx_1f2_model9.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_model10.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 2);
        setVector(&fx_1f2_model10.rot8, 0, 0, 0);
        setVector(&fx_1f2_model10.pos10, work->pos120.vx, work->pos120.vy, work->pos120.vz);
        setVector(&fx_1f2_model10.scale20, 0x1000, 0x1000, 0x1000);
        fx_1f2_orbitModels[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 0xFF);
        fx_1f2_orbitModels[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 0xFF);
        fx_1f2_orbitModels[2].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8B), 0xFF);
        fx_1f2_orbitModels[3].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 0xFF);
        for (i = 0; i < 4; i++) {
            fx_1f2_orbitModels[i].model4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
            fx_1f2_orbitModels[i].model8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 1);
            fx_1f2_orbitModels[i].radiusC = 0xC00;
            setVector(&fx_1f2_orbitModels[i].rot10, 0, i * 0x400 + 0x800, 0);
            setVector(&fx_1f2_orbitModels[i].pos18, 0, 0, 0);
            setVector(&fx_1f2_orbitModels[i].scale28, 0x1000, 0x1000, 0x1000);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->spin2C = 0x20;
            work->timer4C = 0x5F;
            emitter = work->emit84 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x2D:
            work->timer54 = 0x32;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->bonePos110[0]);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->bonePos110[1]);
            emitter = work->emit80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos120);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x4F:
            work->timerC = 0x10;
            break;
        case 0x5F:
            work->timer4C = 0;
            for (i = 0; i < 0x10; i++) {
                fx_1f2_trails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1f2_trails[i], 0);
            }
            work->timer20 = 0x40;
            work->active0 = 1;
            break;
        case 0x8F:
            work->timer8 = 0x50;
            work->timer44 = 0x40;
            break;
        case 0x9F:
            work->timer3C = 0x40;
            work->timer24 = 0x50;
            work->pos100.vy = -0x4C8;
            work->pos100.vx = 0;
            work->pos100.vz = 0;
            work->pos108.vx = 0;
            work->pos108.vy = 0x4C8;
            work->pos108.vz = 0;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos100);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos108);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0xCF:
            work->timer28 = 0x80;
            work->timer34 = 0x80;
            work->halfWidth38 = 0x200;
            break;
        case 0xFF:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&fx_1f2_emitPos0);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x11F:
            emitter = work->emit88 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&fx_1f2_emitPos1);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            emitter = work->emit8C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&fx_1f2_emitPos1);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            work->timer4 = 0x80;
            break;
        case 0x13F:
            work->timer14 = 0x10;
            break;
        case 0x14F:
            work->spin2C = 0x40;
            fx_1f2_emitPos0.vx = 0;
            fx_1f2_emitPos0.vy = -0x4FD;
            fx_1f2_emitPos0.vz = 0;
            fx_1f2_model9Pos.vx = 0;
            fx_1f2_model9Pos.vy = -0x4FD;
            fx_1f2_model9Pos.vz = 0;
            ex->btlfx_moveEmitterTo((s32)work->emit88, (s32)&fx_1f2_emitPos0);
            ex->btlfx_moveEmitterTo((s32)work->emit8C, (s32)&fx_1f2_emitPos0);
            work->timer18 = 0x10;
            work->timer50 = 0x68;
            work->timer24 = 0;
            work->active0 = 0;
            work->timer8 = 0;
            work->timer44 = 0;
            work->timer3C = 0;
            work->timer4 = 0;
            work->timer20 = 0x40;
            work->timer34 = 0x50;
            break;
        case 0x18F:
            work->timer14 = 0x10;
            for (i = 0; i < 0x10; i++) {
                fx_1f2_unitTrails[i].timer4C = 0;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, -1, 0x1000, 0x1000, 1,
                                    (u8 *)&fx_1f2_unitTrails[i], 0);
            }
            break;
        case 0x19F:
            work->timer18 = 0x10;
            ex->btlfx_stopEmitter(work->emit8C);
            ex->btlfx_stopEmitter(work->emit88);
            break;
        case 0x1A7:
            work->timer58 = 0x18;
            break;
        case 0x1BF:
            return 1;
        }
        ex->btlfx_aimAngles((s16 *)&fx_1f2_emitPos0, (s16 *)&env->camera->eye, (s16 *)&fx_1f2_faceAngles);
        if (work->active0 != 0) {
            fx_1f2_model1.rot8.vx = fx_1f2_faceAngles.vx + 0x800;
            fx_1f2_model1.rot8.vy = fx_1f2_faceAngles.vy;
            FX1F2_DRAW(fx_1f2_model0);
            FX1F2_DRAW(fx_1f2_model1);
            FX1F2_DRAW(fx_1f2_model2);
            FX1F2_DRAW(fx_1f2_model3);
            for (i = 0; i < 4; i++) {
                rg = &fx_1f2_orbitModels[i];
                rg->pos18.vx = (ex->math_sin(rg->rot10.vy) * rg->radiusC) >> 12;
                lo_r0 = ex->math_cos(rg->rot10.vy) * rg->radiusC;
                rg->pos18.vy = 0;
                rg->pos18.vz = lo_r0 >> 12;
                if (work->done48 == 0) {
                    ex->Hi_DrawEffModel(&fx_1f2_orbitModels[i].rot10, &fx_1f2_orbitModels[i].pos18, (s32)&fx_1f2_orbitModels[i].scale28, rg->model8);
                    if (work->timer44 != 0) {
                        if (--work->timer44 == 0) {
                            work->done48 = 1;
                        }
                        tint = (work->timer44 - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->model8, tint, tint, tint);
                        rg->radiusC -= work->timer44 >> 4;
                    }
                }
                rg->pos18.vx += ex->btlfx_randRange(-8, 8);
                rg->pos18.vy += ex->btlfx_randRange(-8, 8);
                if (work->done40 == 0) {
                    ex->Hi_DrawEffModel(&fx_1f2_orbitModels[i].rot10, &fx_1f2_orbitModels[i].pos18, (s32)&fx_1f2_orbitModels[i].scale28, rg->model4);
                    if (work->timer3C != 0) {
                        if (--work->timer3C == 0) {
                            work->done40 = 1;
                        }
                        tint = (work->timer3C - 0x40) * 2;
                        ex->Hi_ModifyEffModelRGB(rg->model4, tint, tint, tint);
                    }
                }
                ex->Hi_DrawEffModel(&fx_1f2_orbitModels[i].rot10, &fx_1f2_orbitModels[i].pos18, (s32)&fx_1f2_orbitModels[i].scale28, rg->model0);
            }
            for (i = 0; i < 2; i++) {
                md = &fx_1f2_model4[i];
                lo_md = ex->math_cos(md->rot8.vz) * md->radius4;
                md->pos10.vx = 0;
                md->pos10.vz = 0;
                md->pos10.vy = lo_md >> 12;
                ex->Hi_DrawEffModel(&fx_1f2_model4[i].rot8, &fx_1f2_model4[i].pos10, (s32)&fx_1f2_model4[i].scale20, md->model0);
                md->radius4 -= 4;
            }
            fx_1f2_model0.rot8.vy++;
            fx_1f2_model2.rot8.vy--;
            ex->btlfx_drawScrollMove(0x7530 - frame * 3, 0x340, 0x180, 0x10, 0x80, 0x350, 0x180);
            ex->btlfx_drawScrollMove(0x7530 - frame * 4, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->timer4 != 0) {
            work->timer4--;
            fx_1f2_model5.scale20.vx += 8;
            fx_1f2_model5.scale20.vy += 8;
            fx_1f2_model5.scale20.vz += 8;
            FX1F2_DRAW(fx_1f2_model5);
            if (work->timer4 > 0x60) {
                tint = (0x60 - work->timer4) * 2;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model5.model0, tint, tint, tint);
            }
        }
        if (work->timer24 != 0) {
            work->timer24--;
            fx_1f2_model7.rot8.vx = fx_1f2_faceAngles.vx + 0x800;
            fx_1f2_model7.rot8.vy = fx_1f2_faceAngles.vy;
            FX1F2_DRAW(fx_1f2_model7);
            if (work->timer24 > 0x30) {
                tint = (0x30 - work->timer24) * 8;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model7.model0, tint, tint, tint);
            }
            if (work->timer24 < 0x20) {
                tint = (work->timer24 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model7.model0, tint, tint, tint);
            }
        }
        if (work->timer8 != 0) {
            work->timer8--;
            fx_1f2_model6[0].rot8.vx = 0x900;
            fx_1f2_model6[1].rot8.vx = 0x700;
            fx_1f2_model6[1].rot8.vy = fx_1f2_faceAngles.vy;
            fx_1f2_model6[0].rot8.vy = fx_1f2_faceAngles.vy;
            for (i = 0; i < 2; i++) {
                ex->Hi_DrawEffModel(&fx_1f2_model6[i].rot8, &fx_1f2_model6[i].pos10, (s32)&fx_1f2_model6[i].scale20, fx_1f2_model6[i].model0);
                md = &fx_1f2_model6[i];
                if (work->timer8 > 0x30) {
                    tint = (0x30 - work->timer8) * 8;
                    ex->Hi_ModifyEffModelRGB(md->model0, tint, tint, tint);
                }
                if (work->timer8 < 0x20) {
                    tint = (work->timer8 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md->model0, tint, tint, tint);
                }
            }
        }
        if (work->timer28 != 0) {
            work->timer28--;
            for (i = 0; i < 4; i++) {
                md_r28 = &fx_1f2_model8[i];
                md_r28->rot8.vx = fx_1f2_faceAngles.vx + 0x800;
                md_r28->rot8.vy = fx_1f2_faceAngles.vy;
                ex->Hi_DrawEffModel(&fx_1f2_model8[i].rot8, &fx_1f2_model8[i].pos10, (s32)&fx_1f2_model8[i].scale20, md_r28->model0);
                if (work->timer28 > 0x70) {
                    tint = (0x70 - work->timer28) * 0x10;
                    ex->Hi_ModifyEffModelRGB(md_r28->model0, tint, tint, tint);
                }
                if (work->timer28 < 0x20) {
                    tint = (work->timer28 - 0x20) * 8;
                    ex->Hi_ModifyEffModelRGB(md_r28->model0, tint, tint, tint);
                }
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x320, 0x180, 0x20, 0x80, 0x360, 0x180);
        }
        for (i = 0, md_r28 = fx_1f2_model8; i < 4; i++) {
            md_r28[i].rot8.vz += work->spin2C;
        }
        if (work->timer50 != 0) {
            work->timer50--;
            fx_1f2_model9.rot8.vx = fx_1f2_faceAngles.vx + 0x800;
            fx_1f2_model9.rot8.vy = fx_1f2_faceAngles.vy;
            fx_1f2_model9.rot8.vz += 0x40;
            FX1F2_DRAW(fx_1f2_model9);
            if (work->timer50 > 0x70) {
                tint = (0x40 - work->timer50) * 0x10;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model9.model0, tint, tint, tint);
            }
            if (work->timer50 < 0x20) {
                tint = (work->timer50 - 0x20) * 8;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model9.model0, tint, tint, tint);
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->timer54 != 0) {
            work->timer54--;
            fx_1f2_model10.rot8.vx = fx_1f2_faceAngles.vx + 0x800;
            fx_1f2_model10.rot8.vy = fx_1f2_faceAngles.vy;
            fx_1f2_model10.rot8.vz += 0x40;
            FX1F2_DRAW(fx_1f2_model10);
            if (work->timer54 > 0x2A) {
                tint = (0x2A - work->timer54) << 5;
                ex->Hi_ModifyEffModelRGB(fx_1f2_model10.model0, tint, tint, tint);
            }
            ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x10, 0x80, 0x1D0, 0x100);
        }
        if (work->timerC != 0) {
            work->timerC--;
            tint = fxm_easeLerp(1, 0x20, work->timerC, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 2, tint, tint, tint);
        }
        if (work->timer10 != 0) {
            work->timer10--;
            tint = fxm_easeLerp(2, 0x10, work->timer10, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)env->ot, 2, tint, tint, tint);
        }
        if (work->timer14 != 0) {
            work->timer14--;
            tint = fxm_easeLerp(1, 0x10, work->timer14, 0, 0xFF);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 8), 1, tint, tint, tint);
        }
        if (work->timer18 != 0) {
            work->timer18--;
            tint = fxm_easeLerp(0, 0x10, work->timer18, 0xFF, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 8), 1, tint, tint, tint);
        }
        if (work->timer20 != 0) {
            s32 k;

            work->timer20--;
            k = work->timer20;
            scale.vz = scale.vy = scale.vx = k * 3 + 0x1000;
            col.r = col.g = col.b = k * 2;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
        if (work->timer34 != 0) {
            work->timer34--;
            for (i = 0; i < 0x10; i++) {
                tr_sp = &fx_1f2_trails[i];
                if (tr_sp->timer4C == 0 && work->timer34 >= 0xD) {
                    ex->math_rotMatrixYXZ(&fx_1f2_sparkRot, &m);
                    m.t[0] = fx_1f2_emitPos0.vx;
                    m.t[1] = fx_1f2_emitPos0.vy;
                    m.t[2] = fx_1f2_emitPos0.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    for (j = 0; j < 2; j++) {
                        if (i & 1) {
                            in[j].vx = ex->btlfx_randRange(0xC00 - j * 0x2400, 0x1800 - j * 0x2400);
                        } else {
                            in[j].vx = ex->btlfx_randRange(j * 0x2400 - 0x1800, j * 0x2400 - 0xC00);
                        }
                        in[j].vy = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        in[j].vz = (ex->btlfx_rand() & 0x1FFF) - 0x1000;
                        gte_ldv0(&in[j]);
                        gte_rtv0tr();
                        gte_stlvnl(&out[j]);
                        gte_stflg(&flag);
                    }
                    tr_sp->timer4C = (ex->btlfx_rand() & 3) + 8;
                    step.vx = (out[0].vx - out[1].vx) / 9;
                    step.vy = (out[0].vy - out[1].vy) / 9;
                    step.vz = (out[0].vz - out[1].vz) / 9;
                    for (j = 0; j < 9; j++) {
                        fx_1f2_trails[i].pos50[j].vx = out[0].vx;
                        fx_1f2_trails[i].pos50[j].vy = out[0].vy;
                        fx_1f2_trails[i].pos50[j].vz = out[0].vz;
                        fx_1f2_trails[i].velD0[j].vx = ex->btlfx_randRange(-0x80, 0x80);
                        fx_1f2_trails[i].velD0[j].vy = ex->btlfx_randRange(-0x80, 0x80);
                        fx_1f2_trails[i].velD0[j].vz = ex->btlfx_randRange(-0x80, 0x80);
                        out[0].vx -= step.vx;
                        out[0].vy -= step.vy;
                        out[0].vz -= step.vz;
                    }
                }
            }
            env->otZ = 0x80;
            for (i = 0; i < 0x10; i++) {
                tr_sp2 = &fx_1f2_trails[i];
                if (tr_sp2->timer4C != 0) {
                    if (--tr_sp2->timer4C < 8) {
                        s32 n = 9;

                        tr_sp2->lastSlot0 = tr_sp2->count2 = 8;
                        tr_sp2->head1 = 0;
                        sv_sp2 = tr_sp2->points4;
                        for (j = 0; j < n; j++) {
                            fx_1f2_trails[i].pos50[j].vx += fx_1f2_trails[i].velD0[j].vx;
                            fx_1f2_trails[i].pos50[j].vy += fx_1f2_trails[i].velD0[j].vy;
                            fx_1f2_trails[i].pos50[j].vz += fx_1f2_trails[i].velD0[j].vz;
                            sv_sp2->vx = fx_1f2_trails[i].pos50[j].vx;
                            sv_sp2->vy = fx_1f2_trails[i].pos50[j].vy;
                            sv_sp2->vz = fx_1f2_trails[i].pos50[j].vz;
                            sv_sp2->pad = work->halfWidth38;
                            sv_sp2++;
                        }
                        alpha = tr_sp2->timer4C << 9;
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha, 1,
                                            (u8 *)&fx_1f2_trails[i], 0);
                    }
                }
            }
            env->otZ = 0;
        }
        if (work->timer4C != 0) {
            work->timer4C--;
            for (i = 0; i < 0x10; i++) {
                s32 n;
                s32 dz;
                s32 dx;
                Fx1f2Trail *pt;

                if (i >= 8) {
                    if (fx_1f2_trails[i].timer4C != 0) {
                        if (--fx_1f2_trails[i].timer4C < 8) {
                            if (i & 1) {
                                ex->btlfx_getUnitBoneMatrix(0x10, 5, (BtlBone *)&m);
                                m.t[0] = work->bonePos110[0].vx;
                                m.t[1] = work->bonePos110[0].vy;
                                m.t[2] = work->bonePos110[0].vz;
                                dz = 0x200;
                            } else {
                                ex->btlfx_getUnitBoneMatrix(0x10, 9, (BtlBone *)&m);
                                m.t[0] = work->bonePos110[1].vx;
                                m.t[1] = work->bonePos110[1].vy;
                                m.t[2] = work->bonePos110[1].vz;
                                dz = 0x180;
                            }
                            gte_SetRotMatrix(&m);
                            gte_SetTransMatrix(&m);
                            n = 9;
                            pt = &fx_1f2_trails[i];
                            pt->lastSlot0 = pt->count2 = 8;
                            pt->head1 = 0;
                            sv = pt->points4;
                            for (j_u4ce = 0; j_u4ce < n; j_u4ce++) {
                                ((SVECTOR *)out)->vx = fx_1f2_trails[i].pos50[j_u4ce].vx + j_u4ce * 0x60 - 0x180;
                                ((SVECTOR *)out)->vz = fx_1f2_trails[i].pos50[j_u4ce].vz + dz;
                                ((SVECTOR *)out)->vy = fx_1f2_trails[i].pos50[j_u4ce].vy;
                                gte_ldv0((SVECTOR *)out);
                                gte_rtv0tr();
                                gte_stlvnl((VECTOR *)in);
                                gte_stflg(&flag2);
                                sv->vx = ((VECTOR *)in)->vx;
                                sv->vy = ((VECTOR *)in)->vy;
                                sv->vz = ((VECTOR *)in)->vz;
                                sv->pad = 0x40;
                                sv++;
                                fx_1f2_trails[i].pos50[j_u4ce].vx += fx_1f2_trails[i].velD0[j_u4ce].vx;
                                fx_1f2_trails[i].pos50[j_u4ce].vy += fx_1f2_trails[i].velD0[j_u4ce].vy;
                                fx_1f2_trails[i].pos50[j_u4ce].vz += fx_1f2_trails[i].velD0[j_u4ce].vz;
                            }
                            alpha_u4c = 0x800;
                            if (i & 1) {
                                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xC), 0, NULL, frame, 0x1000, alpha_u4c, 1, (u8 *)&fx_1f2_trails[i], 0);
                            } else {
                                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha_u4c, 1, (u8 *)&fx_1f2_trails[i], 0);
                            }
                        }
                    } else {
                        fx_1f2_trails[i].timer4C = (ex->btlfx_rand() & 3) + 8;
                        for (j_u4ce = 0; j_u4ce < 9; j_u4ce++) {
                            fx_1f2_trails[i].pos50[j_u4ce].vx = 0;
                            fx_1f2_trails[i].pos50[j_u4ce].vy = 0;
                            fx_1f2_trails[i].pos50[j_u4ce].vz = 0;
                            fx_1f2_trails[i].velD0[j_u4ce].vx = (ex->btlfx_rand() & 0xF) - 8;
                            fx_1f2_trails[i].velD0[j_u4ce].vy = (ex->btlfx_rand() & 0x1F) - 0x10;
                            fx_1f2_trails[i].velD0[j_u4ce].vz = (ex->btlfx_rand() & 0x1F) - 0x10;
                        }
                    }
                } else {
                    Fx1f2Trail *tp = &fx_1f2_trails[i];

                    n = 9;
                    sv = tp->points4;
                    a = fx_1f2_trails[i].timer4C;
                    fx_1f2_trails[i].timer4C = fx_1f2_trails[i].pos50[a].pad;
                    pt = &fx_1f2_trails[i];
                    pt->lastSlot0 = pt->count2 = 8;
                    pt->head1 = 0;
                    for (j_u4ce = 0; j_u4ce < n; j_u4ce++) {
                        sv->vx = fx_1f2_trails[i].pos50[a].vx;
                        sv->vy = fx_1f2_trails[i].pos50[a].vy;
                        sv->vz = fx_1f2_trails[i].pos50[a].vz;
                        sv->pad = 0x80;
                        sv++;
                        a = fx_1f2_trails[i].pos50[a].pad;
                    }
                    env->otZ = 0x80;
                    alpha_u4cn = 0xFFF;
                    if (i & 1) {
                        fd = ex->btlfx_getFileData(entry->file, 0xC);
                    } else {
                        fd = ex->btlfx_getFileData(entry->file, 6);
                    }
                    ex->btlfx_drawTrail((u8 *)fd, 0, NULL, frame, 0x1000, alpha_u4cn, 1, (u8 *)&fx_1f2_trails[i], 0);
                    env->otZ = 0;
                }
            }
        }
        if (work->timer58 != 0) {
            work->timer58--;
            env->otZ = 0x40;
            k8 = 8;
            for (i = 0; i < env->targets->targetCount; i++) {
                tr = &fx_1f2_unitTrails[i];
                if (tr->timer4C != 0) {
                    if (--tr->timer4C < 8) {
                        n_u58 = 9;
                        sv_u58 = tr->points4;
                        tr->lastSlot0 = tr->count2 = k8;
                        tr->head1 = 0;
                        for (j_u58 = 0; j_u58 < n_u58; j_u58++) {
                            tr->pos50[j_u58].vx += tr->velD0[j_u58].vx;
                            tr->pos50[j_u58].vy += tr->velD0[j_u58].vy;
                            tr->pos50[j_u58].vz += tr->velD0[j_u58].vz;
                            sv_u58->vx = tr->pos50[j_u58].vx;
                            sv_u58->vy = tr->pos50[j_u58].vy;
                            sv_u58->vz = tr->pos50[j_u58].vz;
                            sv_u58->pad = 0x80;
                            sv_u58++;
                        }
                        alpha_u58 = 0x1000;
                        if (work->timer58 < 8) {
                            alpha_u58 = work->timer58 << 9;
                        }
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 6), 0, NULL, frame, 0x1000, alpha_u58, 1,
                                            (u8 *)tr, 0);
                    }
                } else {
                    tr->timer4C = k8;
                    ((SVECTOR *)out)[1].vx = fx_1f2_unitPos[i].vx + ex->btlfx_randRange(-0x40, 0x40);
                    ((SVECTOR *)out)[1].vz = fx_1f2_unitPos[i].vz + ex->btlfx_randRange(-0x40, 0x40);
                    for (j_u58 = 0; j_u58 < 9; j_u58++) {
                        a_u58 = j_u58 + 1;
                        tr->pos50[j_u58].vx = ((SVECTOR *)out)[1].vx + ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->pos50[j_u58].vz = ((SVECTOR *)out)[1].vz + ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->pos50[j_u58].vy = ex->btlfx_randRange(-a_u58 * 4, 0) - (j_u58 << 6);
                        tr->velD0[j_u58].vx = ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->velD0[j_u58].vz = ex->btlfx_randRange(-a_u58 * 2, a_u58 * 2);
                        tr->velD0[j_u58].vy = -ex->btlfx_rand() & 7;
                    }
                }
            }
            env->otZ = 0;
        }
        fx_1f2_model3.rot8.vy += 3;
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f2/nonmatchings/fx_1f2", fx_1f2_headerPtr);

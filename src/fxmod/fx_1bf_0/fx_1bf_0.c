#include "fxmod/fx_1bf_0.h"

static void fxm_drawTrailLine(u8 *file, SVECTOR *from, SVECTOR *to, s32 frame, s32 fade, s32 abr, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1bf_0_env);
    SVECTOR *pts;

    fx_1bf_0_lineTrail.lastSlot0 = fx_1bf_0_lineTrail.count2 = 1;
    fx_1bf_0_lineTrail.head1 = 0;
    pts = fx_1bf_0_lineTrail.points4;
    pts[0].vx = from->vx;
    pts[0].vy = from->vy;
    pts[0].vz = from->vz;
    pts[0].pad = lineWidth;
    pts[1].vx = to->vx;
    pts[1].vy = to->vy;
    pts[1].vz = to->vz;
    pts[1].pad = lineWidth;
    ex->btlfx_drawTrail(file, 0, NULL, frame, 0x1000, fade, abr, (u8 *)&fx_1bf_0_lineTrail, 0);
}

void fx_1bf_0_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 reach, s32 lineWidth, s32 alpha, s32 fade, s32 flag) {
    u8 *data;
    s32 layer;
    s32 width;
    BtlfxExports *ex;
    SVECTOR last;
    SVECTOR lineBase;
    SVECTOR lineTop;
    s16 *pts;
    Fx1bf0Line *tr;
    SVECTOR *dst;
    s32 j;
    s32 i;
    s32 cnt;
    s32 pointFrame;

    ex = BTLFX_EXPORTS(fx_1bf_0_env);
    layer = 0;
    data = (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STRAND_POINTS);
    lineWidth = (lineWidth * 3) >> 1;
    do {
        for (j = 0; j < 18; j++) {
            cnt = *(u16 *)(data + ((j + layer * 18) << 2));
            pts = (s16 *)(data + cnt);
            cnt = pts[1];
            tr = fx_1bf_0_strandTrails[j];
            tr = (Fx1bf0Line *)((u8 *)tr + fx_1bf_0_strandParams[j * 4 + 2] * layer);
            width = lineWidth * (fx_1bf_0_strandParams[j * 4 + 3] + 1);
            tr->lastSlot0 = tr->count2 = cnt - 1;
            tr->head1 = 0;
            pts += 2;
            dst = tr->points4;
            for (i = 0; i < cnt; i++, dst++, pts += 3) {
                pointFrame = fx_1bf_0_strandParams[j * 4 + 1];
                if (i == 0) {
                    last.vx = from->vx + pts[0] * 2;
                    last.vy = from->vy + pts[1] * 2;
                    last.vz = from->vz + pts[2] * 2;
                    last.pad = 0;
                }
                pointFrame += i;
                if (reach >= pointFrame) {
                    dst->vx = from->vx + pts[0] * 2;
                    dst->vy = from->vy + pts[1] * 2;
                    dst->vz = from->vz + pts[2] * 2;
                    dst->pad = width;
                    BTLFX_COPY_SVEC(&last, dst);
                    if (pointFrame == reach && flag) {
                        BTLFX_COPY_SVEC(&lineBase, dst);
                        BTLFX_COPY_SVEC(&lineTop, dst);
                        lineTop.vy -= 0x1000;
                        fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &lineTop, &lineBase, 0, 0x800, 1, 0x32);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_1bf_0_strandParams[j * 4]), 0, (s32 *)from, 0, 0x1000, alpha,
                                1, (u8 *)tr, fade);
        }
        layer++;
    } while (layer < 3);
}

void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    s32 k = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1bf_0_env);
    Fx1bf0Pt *pt;
    s32 i;
    s32 orbitAng;

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    for (; k < 3; k++) {
        pt = (Fx1bf0Pt *)ex->btlfx_getFileData(entry->file, fx_1bf_0_pointFileIds[k]);
        pt = (Fx1bf0Pt *)((u8 *)pt + 4);
        for (i = 0; i < 20; i++, pt++) {
            if (i < n) {
                pos.vx = from->vx + pt->x * 2;
                pos.vy = from->vy + pt->y * 2;
                pos.vz = from->vz + pt->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_1bf_0_particleFileIds[i & 7]), &pos, &rot, 0x3000,
                                        0, 1, ot, 0, 0);
            }
        }
    }
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 27; i++) {
            if (i <= n * 27 / 20) {
                orbitAng = ((i + k * 27) << 12) / 81;
                pos.vx = from->vx + ((ex->math_cos(orbitAng) * 1600) >> 12);
                pos.vy = from->vy;
                pos.vz = from->vz + ((ex->math_sin(orbitAng) * 1600) >> 12);
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_1bf_0_particleFileIds[i & 7]), &pos, &rot, 0x1800,
                                        0, 1, ot, 0, 0);
            }
        }
    }
}

void fxm_drawOffsetParticles3(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *ofs = fx_1bf_0_particleOffsets;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1bf_0_env);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + ofs->vx * 2;
        pos.vy = from->vy + ofs->vy * 2;
        pos.vz = from->vz + ofs->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_1bf_0_particleFileIds[i + 8]), &pos, &rot, 0x2AF8, 0, 1, ot, 0, 0);
        ofs++;
        i++;
    } while (i < 3);
}

s32 fx_1bf_0_op80(s32 mode, Fx1bf0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ringTop;
    SVECTOR ringPos;
    SVECTOR rot;
    SVECTOR centerTop;
    SVECTOR centerPos;
    s32 frame;
    s32 t;
    s32 orbitAng;
    s32 orbitRadius;
    s32 offX;
    s32 prog;
    s32 offZ;
    s32 reach;
    s32 flag;
    s32 fade;
    s32 shown;
    s32 lineWidth;
    s32 alpha;
    s32 patternAlpha;
    s32 offsetAlpha;
    s32 bright;
    s16 baseY;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->state0 = 5;
        work->unk4 = 0;
        fx_1bf_0_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_1bf_0_effModels, 5);
        work->pos8.vx = fx_1bf_0_origin.vx;
        work->pos8.vy = fx_1bf_0_origin.vy;
        work->pos8.vz = fx_1bf_0_origin.vz;
        work->unk10.vx = work->pos8.vx;
        work->unk10.vy = work->pos8.vy;
        work->unk10.vz = work->pos8.vz;
        env->camera->points[3].vx = work->pos8.vx;
        env->camera->points[3].vy = work->pos8.vy;
        env->camera->points[3].vz = work->pos8.vz;
        fx_1bf_0_strandTrailTail[0] = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 5:
            env->otZ = 0x10;
            if (frame >= 0x26) {
                t = ((frame - 0x26) << 12) / 24;
                orbitAng = ex->math_cos(t >> 1) >> 1;
                orbitRadius = ex->math_cos(t >> 2) >> 1;
                offX = (ex->math_cos(orbitAng) * orbitRadius) >> 12;
                offZ = (ex->math_sin(orbitAng) * orbitRadius) >> 12;
                ringPos.vx = work->pos8.vx + offX;
                ringPos.vy = work->pos8.vy;
                ringPos.vz = work->pos8.vz + offZ;
                ringTop.vx = ringPos.vx;
                ringTop.vy = work->pos8.vy - 0x1000;
                ringTop.vz = ringPos.vz;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_RING), &ringPos, NULL, 0xAAA, 0, 1, t);
                fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &ringPos, &ringTop, frame, t, 1, 0x20);
            }
            env->otZ = 0;
            if (frame >= 0x3E) {
                work->state0 = 6;
                entry->frame = -1;
            }
            break;
        case 6:
            centerPos.vx = work->pos8.vx;
            centerPos.vy = work->pos8.vy;
            centerPos.vz = work->pos8.vz;
            centerTop.vx = centerPos.vx;
            baseY = work->pos8.vy;
            centerTop.vz = centerPos.vz;
            centerTop.vy = baseY - 0x1000;
            env->otZ = 0x10;
            t = (frame << 12) / 51;
            bright = 0x1000 - t;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_RING), &centerPos, NULL, 0xAAA, 0, 1, bright);
            fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &centerPos, &centerTop, frame, bright, 1, (t >> 7) + 0x20);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            if (frame < 0x10) {
                s32 ang = (frame << 12) >> 6;
                s32 ringSize = ex->math_sin(ang) * 5;
                s32 ringAlpha = ex->math_cos(ang);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &centerPos, &rot, ringSize, 0, 1, ringAlpha);
            }
            if (frame >= 0x1E) {
                offsetAlpha = (frame - 0x1E) << 9;
                if (offsetAlpha > 0x1000) {
                    offsetAlpha = 0x1000;
                }
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &centerPos, &rot, 0x5000, 0, 1, offsetAlpha);
            }
            flag = 0;
            if (frame < 0x26) {
                if (frame < 0x1E) {
                    reach = frame;
                    lineWidth = 0x20;
                    alpha = 0x1000;
                    fade = 0;
                    flag = 1;
                    shown = 0;
                    patternAlpha = fade;
                    offsetAlpha = fade;
                } else {
                    s32 elapsed = frame - 0x1E;
                    prog = elapsed << 9;
                    reach = 0x1D;
                    lineWidth = 0x27;
                    alpha = 0x1000;
                    fade = 0;
                    shown = elapsed * 3;
                    patternAlpha = alpha;
                    if (prog >= 0x800) {
                        offsetAlpha = (prog - 0x800) * 2;
                    } else {
                        offsetAlpha = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, ((0x1000 - prog) * 100) >> 12, 0, 0);
                }
            } else {
                prog = ((frame - 0x26) << 12) / 13;
                fade = 1;
                reach = 0x1D;
                shown = 0x15;
                alpha = 0x1000 - prog * 2 / 3;
                lineWidth = (prog >> 3) + 0x40;
                offsetAlpha = alpha;
                patternAlpha = offsetAlpha;
            }
            fx_1bf_0_drawStrands(entry, &fx_1bf_0_origin, reach, lineWidth, alpha, fade, flag);
            if (patternAlpha != 0) {
                fxm_drawPatternParticles(entry, &fx_1bf_0_origin, shown, patternAlpha);
                fxm_drawOffsetParticles3(entry, &fx_1bf_0_origin, offsetAlpha);
            }
            if (frame >= 0x33) {
                rot.vx = fx_1bf_0_origin.vx;
                rot.vy = fx_1bf_0_origin.vy;
                rot.vz = fx_1bf_0_origin.vz;
                rot.vy -= 0x300;
                env->camera->points[3].vx = rot.vx;
                env->camera->points[3].vy = rot.vy;
                env->camera->points[3].vz = rot.vz;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_0/nonmatchings/fx_1bf_0", fx_1bf_0_headerPtr);

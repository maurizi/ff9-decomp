#include "fxmod/fx_0cc.h"

s32 func_801E7704(s32 mode, Fx0ccPart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0ccWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 mpos[3];
    s32 scale[3];
    SVECTOR rot;
    SVECTOR pos;
    s32 t;

    if (mode == 1) {
        part->unk0 = 0;
        part->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&part->unk4);
        if (part->unkC != NULL) {
            part->unkC->unk22 = 0;
        }
    } else {
        switch (part->unk0) {
        case 0:
            BTLFX_COPY_SVEC(&pos, &part->unk10);
            t = (frame << 12) / 20;
            pos.vy -= 0x1000;
            pos.vy += t;
            pos.vz = pos.vz + ((0x1000 - t) >> 1);
            BTLFX_COPY_SVEC(&part->unk4, &pos);
            mpos[0] = part->unk4.vx;
            mpos[1] = part->unk4.vy;
            mpos[2] = part->unk4.vz;
            scale[0] = scale[1] = scale[2] = 0x1C7;
            rot.vx = frame * 0x18;
            rot.vy = frame * 0x28;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->unk10);
            env->unk28 = 0x80;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &part->unk4, NULL, 0x1000, 0, 1, -1, 0, 0);
            env->unk28 = 0;
            if (frame >= 0x14) {
                part->unk0 = 1;
                ((s16 *)part)[-1] = -1;
                if (part->unkC != NULL) {
                    part->unkC->unk38 = part->unkC->unk24->unk4.unk2;
                }
                part->unk4.vy = 0;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0, 0);
            }
            break;
        case 1:
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 5;
            rot.pad = 1;
            BTLFX_COPY_SVEC(&pos, &part->unk4);
            pos.vy -= frame * 4;
            env->unk28 = 0x10;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &pos, NULL, 0x3000, frame, 1, -1, 0, 0);
            if (frame < 0x10) {
                env->unk28 = 0x64;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &pos, NULL, -1, frame, 1, -1);
                env->unk28 = 0;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), &part->unk4, &rot, 0x1000, frame, 1,
                                  -1, 0, 0);
            }
            env->unk28 = 0;
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_0cc_op80(s32 mode, Fx0ccWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0ccPart *part;
    s32 frame;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = 0x14;
        ((EffInit *)arg4)->unk8 = 0x18;
        ((EffInit *)arg4)->unk14 = (s32)func_801E7704;
        ((EffInit *)arg4)->unk4 = 0xB;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unkC = 0;
        D_801E7C9C = env;
        ex->btlfx_initEffModels(D_801E7CAC, 1);
        work->unk10 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0);
        BTLFX_COPY_SVEC(&work->unk4, &env->targets->centerX);
        work->unk4.vy = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame & 1) {
                if (work->unkC < 10) {
                    work->unkC++;
                    part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (part != NULL) {
                        part->unk10.vx = work->unk4.vx + ex->btlfx_randCentered(0x800);
                        part->unk10.vz = work->unk4.vz + ex->btlfx_randCentered(0x800);
                        part->unk10.vy = 0;
                    }
                }
            }
            if (work->unkC >= 10) {
                if (frame >= 0x29) {
                    ex->btlfx_clearEffModels();
                }
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cc/nonmatchings/fx_0cc", fx_0cc_headerPtr);

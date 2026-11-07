#include "fxmod/fx_0e5.h"

s32 fx_0e5_op80(s32 mode, Fx0e5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 lineFrame;

    if (mode == 0) {
        *args = sizeof(Fx0e5Work);
    } else if (mode == 1) {
        fx_0e5_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot20);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        work->linePos38.vx = work->pos10.vx;
        work->linePos38.vy = work->pos10.vy;
        work->linePos38.vz = work->pos10.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot20, FX_SOUL_BLADE_FLASH_DISTANCE, (s16 *)&work->pos30);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SOUL_BLADE_LIGHT_FLASH), (s32)&work->pos30);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aimRot28);
        work->lineAlpha42 = 0x1000;
        work->lineFrame40 = 0;
    } else {
        frame = entry->frame;
        if (frame == FX_SOUL_BLADE_WISPS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SOUL_BLADE_SPIRIT_WISPS), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= FX_SOUL_BLADE_WISPS_FRAME && frame < FX_SOUL_BLADE_FLARES_END_FRAME) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_SOUL_BLADE_LENS_FLARES), &work->linePos38, work->lineFrame40, work->lineAlpha42);
            if (frame - FX_SOUL_BLADE_WISPS_FRAME >= FX_SOUL_BLADE_FLARES_FADE_FRAME) {
                work->lineAlpha42 -= 0x200;
            }
            work->lineFrame40++;
            lineFrame = work->lineFrame40;
            if (lineFrame >= FX_SOUL_BLADE_FLARES_ANIM_FRAMES) {
                lineFrame = FX_SOUL_BLADE_FLARES_ANIM_FRAMES - 1;
            }
            work->lineFrame40 = lineFrame;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aimRot28.vx;
                work->emit0->rot.vy = work->aimRot28.vy;
                work->emit0->rot.vz = work->aimRot28.vz;
            }
        }
        if (frame >= FX_SOUL_BLADE_WISPS_FRAME) {
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->aimRot28.vx;
                    work->emit4->rot.vy = work->aimRot28.vy;
                    work->emit4->rot.vz = work->aimRot28.vz;
                }
            }
        }
        return frame >= FX_SOUL_BLADE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e5/nonmatchings/fx_0e5", fx_0e5_headerPtr);

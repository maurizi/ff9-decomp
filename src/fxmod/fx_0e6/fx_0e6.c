#include "fxmod/fx_0e6.h"

s32 fx_0e6_op80(s32 mode, Fx0e6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 n;
    Fx0e6Line *line;
    SVECTOR *pt;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_0e6_env = env;
        ex->btlfx_initEffModels(fx_0e6_effModels, 1);
        work->rot24.vx = 0;
        work->rot24.vy = 0;
        work->rot24.vz = 0;
        work->scale4.vx = 0x1000;
        work->scale4.vy = 0x1000;
        work->scale4.vz = 0x1000;
        work->model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_STONE_ROCK_SHELL), 0xF7, 0x3D00, 0, 0);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos2C);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos34);
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_STONE_ROCK_STRIP), 0, 0, -1, 0x1000, 0x1000, mode,
                          (u8 *)&fx_0e6_strip, 0);
    } else {
        frame = entry->frame;
        ex->btlfx_lerpVec((s16 *)&work->pos2C, (s16 *)&work->pos34, (frame << 22) >> 12, (s16 *)&pos);
        work->pos14.vx = pos.vx;
        work->pos14.vy = pos.vy;
        work->pos14.vz = pos.vz;
        if (frame < 4) {
            ex->Hi_DrawEffModel(&work->rot24, &work->pos14, (s32)&work->scale4, work->model0);
        }
        if (frame < 8) {
            n = 25;
            line = &fx_0e6_strip;
            line->count2 = 0x18;
            line->lastSlot0 = 0x18;
            line->head1 = 0;
            pt = line->pts4;
            for (i = 0; i < n; i++) {
                ex->btlfx_lerpVec((s16 *)&work->pos2C, (s16 *)&pos, (0x1000000 / n * i) >> 12, (s16 *)pt);
                pt->pad = 0x20;
                pt++;
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_STONE_ROCK_STRIP), 0, 0, 0, 0x1000, (8 - frame) << 9, 1,
                              (u8 *)line, 0);
        }
        if (frame == 4) {
            Pool6CEntry *emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STONE_RED_RINGS), (s32)&work->pos34);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e6/nonmatchings/fx_0e6", fx_0e6_headerPtr);

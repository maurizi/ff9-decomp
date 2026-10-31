#include "fxmod/fx_0f7.h"

s32 fx_0f7_op80(s32 mode, Fx0f7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0f7Spark *p;
    s32 frame;
    s32 i;
    s32 a;
    s32 d;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->emitC = NULL;
        work->emit10 = NULL;
        work->emit14 = NULL;
        work->unk0 = 0;
        fx_0f7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->emit4);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->emit4);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            work->unk0 = 0x30;
            break;
        case 0x20:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->emit4);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x28:
            ex->btlfx_stopEmitter(work->emitC);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->emit4);
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0xC0);
            }
            break;
        }
        if (work->emitC != NULL && work->emitC->state == BTLFX_EMIT_DONE) {
            work->emitC = NULL;
        }
        if (work->emit14 != NULL && work->emit14->state == BTLFX_EMIT_DONE) {
            work->emit14 = NULL;
        }
        if (work->emitC != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 4), frame);
        }
        if (work->emit14 != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), frame);
        }
        if (work->unk0 != 0) {
            work->unk0--;
            for (i = 0; i < 4; i++) {
                p = &D_801E7AE0[i];
                if (p->unk0 == 0) {
                    a = 0x1000;
                    p->unk10 += p->unk18;
                    p->unk1C++;
                    if (p->unk1C < 8) {
                        a = (p->unkC * p->unk1C) >> 3;
                    }
                    if (work->unk0 < 8) {
                        a = (p->unkC * work->unk0) >> 3;
                    }
                    if (p->unk4 == 0) {
                        ex->btlfx_drawSpriteAtOt(p->unk8, (u8 *)ex->btlfx_getFileData(entry->file, 5), p->unk10, p->unk14, 0,
                                          1, a);
                    } else {
                        ex->btlfx_drawSpriteAtOt(p->unk8, (u8 *)ex->btlfx_getFileData(entry->file, 0xC), p->unk10, p->unk14,
                                          0, 1, a);
                    }
                } else {
                    p->unk0--;
                }
            }
        }
        return frame >= 0x48;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f7/nonmatchings/fx_0f7", fx_0f7_headerPtr);

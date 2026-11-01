#include "ff9.h"

void gfx_initTpageSprt(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetSprt((SPRT *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

void gfx_initTpageTile(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetTile((TILE *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyF3(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetPolyF3((POLY_F3 *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyF4(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetPolyF4((POLY_F4 *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyG3(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetPolyG3((POLY_G3 *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyG4(DR_TPAGE *drMode, s32 tpage) {
    SetDrawTPage(drMode, 0, 1, tpage);
    SetPolyG4((POLY_G4 *)(drMode + 1));
    if (MargePrim(drMode, drMode + 1)) {
        exit(-1);
    }
}

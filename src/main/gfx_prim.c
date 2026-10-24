#include "ff9.h"

void gfx_initTpageSprt(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetSprt((SPRT *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void gfx_initTpageTile(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetTile((TILE *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyF3(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF3((POLY_F3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyF4(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF4((POLY_F4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyG3(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG3((POLY_G3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void gfx_initTpagePolyG4(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG4((POLY_G4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

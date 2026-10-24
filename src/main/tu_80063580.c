#include "ff9.h"

void func_80063580(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetSprt((SPRT *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_800635E0(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetTile((TILE *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063640(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF3((POLY_F3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_800636A0(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyF4((POLY_F4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063700(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG3((POLY_G3 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

void func_80063760(DR_TPAGE *p, s32 tpage) {
    SetDrawTPage(p, 0, 1, tpage);
    SetPolyG4((POLY_G4 *)(p + 1));
    if (MargePrim(p, p + 1)) {
        exit(-1);
    }
}

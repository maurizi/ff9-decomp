#include "fxmod/fx_1fc.h"

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op80);

s32 fx_1fc_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EC14C = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

s32 fx_1fc_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EC14C = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_headerPtr);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op83);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op84);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E86A0);

void func_801E86B8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EC14C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E86FC(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8754(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E876C(Fx1fcPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1fcPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

void func_801E87D0(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E87E8);

void func_801E887C(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8894);

void func_801E88D8(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E88F0(Fx1fcTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk2 - 1;
    s32 done = 1;
    Fx1fcKey6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

void func_801E8954(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801EC14C)->btlfx_drawScreenTint((u32 *)(D_801EC14C->unk20 + ot), arg4, c, c, c);
}

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E89EC);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8AB8);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8B04);

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", func_801E8B60);

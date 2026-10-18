#include "fxmod/fx_1fd.h"

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7704);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7AF8);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7E18);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op80);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op81);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op82);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op83);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op84);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9710);

void func_801E9728(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E4C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E976C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E97C4(Fx1fdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fdPoint *)data;
}

s32 func_801E97DC(Fx1fdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1fdPoint *p;

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

void func_801E9840(Fx1fdPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fdPoint *)data;
}

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9858);

void func_801E98EC(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E9E4C)->btlfx_drawScreenTint((u32 *)(D_801E9E4C->unk20 + ot), arg4, c, c, c);
}

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9984);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9A50);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9A9C);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9AF8);

void func_801E9C58(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 m = x % w;
    Unk800F78A4 *env = D_801E9E4C;
    u32 *p = (u32 *)env->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->unk24 = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->unk20 + 0xFFF, p);
    }
}

INCLUDE_RODATA("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_headerPtr);

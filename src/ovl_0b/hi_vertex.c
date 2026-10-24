#include "ovl_0b.h"

void Hi_SplitMdlVertex(s32 arg0, s32 arg1, s32 arg2) {
    GeoSplitVertices(btlfx_getEffModel(arg0, "Hi_SplitMdlVertex()\nid:%d cannot use!")->unk0, arg1, arg2);
}

s32 Hi_GetSplitMdlVertex(s32 arg0, s32 arg1) {
    return func_800DF670(btlfx_getEffModel(arg0, "Hi_GetSplitMdlVertex()\nid:%d cannot use!")->unk0, arg1);
}

void GeoSplitVertices(BtlUnit9C *arg0, s32 arg1, s32 arg2) {
    s32 n;
    BtlMesh *m;
    s32 i;

    n = arg0->unk8->unk3;
    m = arg0->unk8->unk10;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            arg2 = func_800DF940(arg0, i, arg2);
            if (m->unk14[i] >= 0x322) {
                printf("warning:GeoSplitVertices() overflow vertex buffer!(%d)\n", m->unk14[i]);
                m->unk14[i] = 0x321;
            }
        }
    }
}

s32 func_800DF670(BtlUnit9C *arg0, s32 arg1) {
    BtlUnit9C8 *m = arg0->unk8;
    s32 n = m->unk3;
    s32 r = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            BtlMesh *g = &m->unk10[i];
            r += g->unk2 * 4;
            r += g->unk4 * 3;
            r += g->unk6 * 4;
            r += g->unk8 * 3;
            r += g->unkA * 4;
            r += g->unkC * 3;
            r += g->unkE * 4;
            r += g->unk10 * 3;
        }
    }
    return r * 8;
}

/* Void in the original; effect modules read $v0 after it (export 0x310): func_800DF7A0's u16 result.
   Ports must return it (docs/effect-modules.md). */
void Hi_GetMdlVertexPtr(s32 arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    func_800DF7A0(btlfx_getEffModel(arg0, D_800A825C)->unk0, arg1, arg2, arg3, arg4);
}

u16 func_800DF7A0(BtlUnit9C *arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    BtlMesh *m = &arg0->unk8->unk10[arg1];
    if (arg4 == 0) {
        *arg3 = arg2;
        return m->unk2;
    }
    arg2 += m->unk2 << 5;
    if (arg4 == 1) {
        *arg3 = arg2;
        return m->unk4;
    }
    arg2 += m->unk4 * 0x18;
    if (arg4 == 2) {
        *arg3 = arg2;
        return m->unk6;
    }
    arg2 += m->unk6 << 5;
    if (arg4 == 3) {
        *arg3 = arg2;
        return m->unk8;
    }
    arg2 += m->unk8 * 0x18;
    if (arg4 == 4) {
        *arg3 = arg2;
        return m->unkA;
    }
    arg2 += m->unkA << 5;
    if (arg4 == 5) {
        *arg3 = arg2;
        return m->unkC;
    }
    arg2 += m->unkC * 0x18;
    if (arg4 == 6) {
        *arg3 = arg2;
        return m->unkE;
    }
    arg2 += m->unkE << 5;
    if (arg4 == 7) {
        *arg3 = arg2;
        return m->unk10;
    }
    *arg3 = 0;
    return 0;
}

void func_800DF920(void) {
}

s32 func_800DF928(void) {
    return 0;
}

s32 func_800DF930(void) {
    return 0;
}

void func_800DF938(void) {
}

/* gives every vertex reference of nv-vertex primitives its own copy in arg2 */
#define SPLIT_PRIMS(cnt, stride, nv)                                \
    for (j = 0; j < (cnt); j++) {                                   \
        for (k = 0; k < (nv); k++) {                                \
            ((SVECTOR *)arg2)[n].vx = src[((u16 *)p)[k]].vx;        \
            ((SVECTOR *)arg2)[n].vy = src[((u16 *)p)[k]].vy;        \
            ((SVECTOR *)arg2)[n].vz = src[((u16 *)p)[k]].vz;        \
            ((u16 *)p)[k] = n;                                      \
            n++;                                                    \
        }                                                           \
        p += (stride);                                              \
    }

u32 func_800DF940(BtlUnit9C *arg0, u16 arg1, s32 arg2) {
    u16 n;
    BtlMesh *g;
    u8 *p;
    SVECTOR *src;
    u32 j;
    u32 k;

    n = 0;
    g = &arg0->unk8->unk10[arg1];
    p = (u8 *)g->unk1C;
    src = g->unk18;
    g->unk18 = (SVECTOR *)arg2;
    SPLIT_PRIMS(g->unk2, 0x18, 4);
    SPLIT_PRIMS(g->unk4, 0x14, 3);
    SPLIT_PRIMS(g->unk6, 0x20, 4);
    SPLIT_PRIMS(g->unk8, 0x18, 3);
    SPLIT_PRIMS(g->unkA, 0x18, 4);
    SPLIT_PRIMS(g->unkC, 0x14, 3);
    SPLIT_PRIMS(g->unkE, 0x10, 4);
    SPLIT_PRIMS(g->unk10, 0xC, 3);
    g->unk14[arg1] = n;
    return arg2 + n * 2;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_vertex", D_800A825C);

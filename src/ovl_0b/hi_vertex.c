#include "ovl_0b.h"

void Hi_SplitMdlVertex(s32 arg0, s32 arg1, s32 arg2) {
    GeoSplitVertices(btlfx_getEffModel(arg0, "Hi_SplitMdlVertex()\nid:%d cannot use!")->model, arg1, arg2);
}

s32 Hi_GetSplitMdlVertex(s32 arg0, s32 arg1) {
    return btlfx_getSplitVertexSize(btlfx_getEffModel(arg0, "Hi_GetSplitMdlVertex()\nid:%d cannot use!")->model, arg1);
}

void GeoSplitVertices(BtlUnit9C *arg0, s32 arg1, s32 arg2) {
    s32 n;
    BtlMesh *m;
    s32 i;

    n = arg0->header->meshCount;
    m = arg0->header->meshes;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            arg2 = btlfx_splitMeshVertices(arg0, i, arg2);
            if (m->vertexCounts[i] >= 0x322) {
                printf("warning:GeoSplitVertices() overflow vertex buffer!(%d)\n", m->vertexCounts[i]);
                m->vertexCounts[i] = 0x321;
            }
        }
    }
}

s32 btlfx_getSplitVertexSize(BtlUnit9C *arg0, s32 arg1) {
    BtlUnit9C8 *m = arg0->header;
    s32 n = m->meshCount;
    s32 r = 0;
    s32 i;
    for (i = 0; i < n; i++) {
        if (arg1 == -1 || arg1 == i) {
            BtlMesh *g = &m->meshes[i];
            r += g->nFT4 * 4;
            r += g->nFT3 * 3;
            r += g->nGT4 * 4;
            r += g->nGT3 * 3;
            r += g->nG4 * 4;
            r += g->nG3 * 3;
            r += g->nF4 * 4;
            r += g->nF3 * 3;
        }
    }
    return r * 8;
}

/* Void in the original; effect modules read $v0 after it (export 0x310): btlfx_getMeshPrims's u16 result.
   Ports must return it (docs/effect-modules.md). */
void Hi_GetMdlVertexPtr(s32 arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    btlfx_getMeshPrims(btlfx_getEffModel(arg0, s_hiGetMdlVertexPtrErr)->model, arg1, arg2, arg3, arg4);
}

u16 btlfx_getMeshPrims(BtlUnit9C *arg0, u16 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    BtlMesh *m = &arg0->header->meshes[arg1];
    if (arg4 == 0) {
        *arg3 = arg2;
        return m->nFT4;
    }
    arg2 += m->nFT4 << 5;
    if (arg4 == 1) {
        *arg3 = arg2;
        return m->nFT3;
    }
    arg2 += m->nFT3 * 0x18;
    if (arg4 == 2) {
        *arg3 = arg2;
        return m->nGT4;
    }
    arg2 += m->nGT4 << 5;
    if (arg4 == 3) {
        *arg3 = arg2;
        return m->nGT3;
    }
    arg2 += m->nGT3 * 0x18;
    if (arg4 == 4) {
        *arg3 = arg2;
        return m->nG4;
    }
    arg2 += m->nG4 << 5;
    if (arg4 == 5) {
        *arg3 = arg2;
        return m->nG3;
    }
    arg2 += m->nG3 * 0x18;
    if (arg4 == 6) {
        *arg3 = arg2;
        return m->nF4;
    }
    arg2 += m->nF4 << 5;
    if (arg4 == 7) {
        *arg3 = arg2;
        return m->nF3;
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

u32 btlfx_splitMeshVertices(BtlUnit9C *arg0, u16 arg1, s32 arg2) {
    u16 n;
    BtlMesh *g;
    u8 *p;
    SVECTOR *src;
    u32 j;
    u32 k;

    n = 0;
    g = &arg0->header->meshes[arg1];
    p = (u8 *)g->prims;
    src = g->vertices;
    g->vertices = (SVECTOR *)arg2;
    SPLIT_PRIMS(g->nFT4, 0x18, 4);
    SPLIT_PRIMS(g->nFT3, 0x14, 3);
    SPLIT_PRIMS(g->nGT4, 0x20, 4);
    SPLIT_PRIMS(g->nGT3, 0x18, 3);
    SPLIT_PRIMS(g->nG4, 0x18, 4);
    SPLIT_PRIMS(g->nG3, 0x14, 3);
    SPLIT_PRIMS(g->nF4, 0x10, 4);
    SPLIT_PRIMS(g->nF3, 0xC, 3);
    g->vertexCounts[arg1] = n;
    return arg2 + n * 2;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_vertex", s_hiGetMdlVertexPtrErr);

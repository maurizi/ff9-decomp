#include "ovl_0b.h"

void Hi_SplitMdlVertex(s32 modelId, s32 meshIndex, s32 vertexBuf) {
    GeoSplitVertices(btlfx_getEffModel(modelId, "Hi_SplitMdlVertex()\nid:%d cannot use!")->model, meshIndex, vertexBuf);
}

s32 Hi_GetSplitMdlVertex(s32 modelId, s32 meshIndex) {
    return btlfx_getSplitVertexSize(btlfx_getEffModel(modelId, "Hi_GetSplitMdlVertex()\nid:%d cannot use!")->model, meshIndex);
}

void GeoSplitVertices(BtlModel *model, s32 meshIndex, s32 vertexBuf) {
    s32 count;
    BtlMesh *meshes;
    s32 i;

    count = model->header->meshCount;
    meshes = model->header->meshes;
    for (i = 0; i < count; i++) {
        if (meshIndex == -1 || meshIndex == i) {
            vertexBuf = btlfx_splitMeshVertices(model, i, vertexBuf);
            if (meshes->vertexCounts[i] >= 0x322) {
                printf("warning:GeoSplitVertices() overflow vertex buffer!(%d)\n", meshes->vertexCounts[i]);
                meshes->vertexCounts[i] = 0x321;
            }
        }
    }
}

s32 btlfx_getSplitVertexSize(BtlModel *model, s32 meshIndex) {
    BtlModelHeader *hdr = model->header;
    s32 count = hdr->meshCount;
    s32 vertexCount = 0;
    s32 i;
    for (i = 0; i < count; i++) {
        if (meshIndex == -1 || meshIndex == i) {
            BtlMesh *mesh = &hdr->meshes[i];
            vertexCount += mesh->nFT4 * 4;
            vertexCount += mesh->nFT3 * 3;
            vertexCount += mesh->nGT4 * 4;
            vertexCount += mesh->nGT3 * 3;
            vertexCount += mesh->nG4 * 4;
            vertexCount += mesh->nG3 * 3;
            vertexCount += mesh->nF4 * 4;
            vertexCount += mesh->nF3 * 3;
        }
    }
    return vertexCount * 8;
}

/* Void in the original; effect modules read $v0 after it (export 0x310): btlfx_getMeshPrims's u16 result.
   Ports must return it (docs/effect-modules.md). */
void Hi_GetMdlVertexPtr(s32 modelId, u16 meshIndex, s32 prims, s32 *out, s32 primType) {
    btlfx_getMeshPrims(btlfx_getEffModel(modelId, s_hiGetMdlVertexPtrErr)->model, meshIndex, prims, out, primType);
}

u16 btlfx_getMeshPrims(BtlModel *model, u16 meshIndex, s32 prims, s32 *out, s32 primType) {
    BtlMesh *mesh = &model->header->meshes[meshIndex];
    if (primType == BTL_MESH_PRIM_FT4) {
        *out = prims;
        return mesh->nFT4;
    }
    prims += mesh->nFT4 << 5;
    if (primType == BTL_MESH_PRIM_FT3) {
        *out = prims;
        return mesh->nFT3;
    }
    prims += mesh->nFT3 * 0x18;
    if (primType == BTL_MESH_PRIM_GT4) {
        *out = prims;
        return mesh->nGT4;
    }
    prims += mesh->nGT4 << 5;
    if (primType == BTL_MESH_PRIM_GT3) {
        *out = prims;
        return mesh->nGT3;
    }
    prims += mesh->nGT3 * 0x18;
    if (primType == BTL_MESH_PRIM_G4) {
        *out = prims;
        return mesh->nG4;
    }
    prims += mesh->nG4 << 5;
    if (primType == BTL_MESH_PRIM_G3) {
        *out = prims;
        return mesh->nG3;
    }
    prims += mesh->nG3 * 0x18;
    if (primType == BTL_MESH_PRIM_F4) {
        *out = prims;
        return mesh->nF4;
    }
    prims += mesh->nF4 << 5;
    if (primType == BTL_MESH_PRIM_F3) {
        *out = prims;
        return mesh->nF3;
    }
    *out = 0;
    return 0;
}

void btlfx_vertexNop(void) {
}

s32 btlfx_vertexNopZero1(void) {
    return 0;
}

s32 btlfx_vertexNopZero2(void) {
    return 0;
}

void btlfx_imageNop(void) {
}

/* gives every vertex reference of nv-vertex primitives its own copy in vertexBuf */
#define SPLIT_PRIMS(cnt, stride, nv)                                   \
    for (j = 0; j < (cnt); j++) {                                      \
        for (k = 0; k < (nv); k++) {                                   \
            ((SVECTOR *)vertexBuf)[outIdx].vx = src[((u16 *)p)[k]].vx; \
            ((SVECTOR *)vertexBuf)[outIdx].vy = src[((u16 *)p)[k]].vy; \
            ((SVECTOR *)vertexBuf)[outIdx].vz = src[((u16 *)p)[k]].vz; \
            ((u16 *)p)[k] = outIdx;                                    \
            outIdx++;                                                  \
        }                                                              \
        p += (stride);                                                 \
    }

u32 btlfx_splitMeshVertices(BtlModel *model, u16 meshIndex, s32 vertexBuf) {
    u16 outIdx;
    BtlMesh *mesh;
    u8 *p;
    SVECTOR *src;
    u32 j;
    u32 k;

    outIdx = 0;
    mesh = &model->header->meshes[meshIndex];
    p = (u8 *)mesh->prims;
    src = mesh->vertices;
    mesh->vertices = (SVECTOR *)vertexBuf;
    SPLIT_PRIMS(mesh->nFT4, 0x18, 4);
    SPLIT_PRIMS(mesh->nFT3, 0x14, 3);
    SPLIT_PRIMS(mesh->nGT4, 0x20, 4);
    SPLIT_PRIMS(mesh->nGT3, 0x18, 3);
    SPLIT_PRIMS(mesh->nG4, 0x18, 4);
    SPLIT_PRIMS(mesh->nG3, 0x14, 3);
    SPLIT_PRIMS(mesh->nF4, 0x10, 4);
    SPLIT_PRIMS(mesh->nF3, 0xC, 3);
    mesh->vertexCounts[meshIndex] = outIdx;
    return vertexBuf + outIdx * 2;
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/hi_vertex", s_hiGetMdlVertexPtrErr);

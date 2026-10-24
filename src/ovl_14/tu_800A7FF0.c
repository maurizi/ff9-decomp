#include "ovl_14.h"

void scene_init(void) {
    s32 i;
    Ovl14Mode *m;

    i = 0;
    m = g_sceneModes;
    g_scene.unk0 = 0;
    g_scene.unk2 = 0;
    g_scene.unk3 = 0;
    for (; i < 6; i++, m++) {
        if (m->unk0 != NULL) {
            m->unk0();
        }
    }
    if (g_sceneModes[g_scene.unk0].unk4 != NULL) {
        g_sceneModes[g_scene.unk0].unk4();
    }
}

void scene_update(void) {
    if (g_sceneModes[g_scene.unk0].unk8 != NULL) {
        g_sceneModes[g_scene.unk0].unk8();
    }
}

void scene_exit(void) {
    if (g_sceneModes[g_scene.unk0].unkC != NULL) {
        g_sceneModes[g_scene.unk0].unkC();
    }
}

s32 scene_get(void) {
    return g_scene.unk0;
}

void scene_set(s32 arg0) {
    if (arg0 != g_scene.unk0) {
        if (g_sceneModes[g_scene.unk0].unkC != NULL) {
            g_sceneModes[g_scene.unk0].unkC();
        }
        g_scene.unk0 = arg0;
        if (g_sceneModes[g_scene.unk0].unk4 != NULL) {
            g_sceneModes[g_scene.unk0].unk4();
        }
    }
}

s32 scene_checkDisc(void) {
    u32 i;
    s32 prev;

    i = 0;
    prev = g_gameState->unkB;
    do {
        func_8001DCB8();
    } while (g_gameState->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (scene_isDiscGone(prev, g_gameState->unkB)) {
        return scene_discIndex(prev);
    }
    return 0xFF;
}

s32 scene_discIndex(s32 arg0) {
    s32 i;
    s32 m;

    for (i = 0; i < 4; i++) {
        m = 1 << i;
        if (arg0 & m) {
            return i;
        }
    }
    return 0;
}

u8 scene_isDiscGone(s32 arg0, s32 arg1) {
    s32 a = arg0 & 0xF;
    s32 b = arg1 & 0xF;
    return (b & a) != a;
}

s32 scene_isTitleIdle(void) {
    if (func_8002F810() != 0) {
        return 0;
    }
    return g_scene.unk0 == 1;
}

void scene_setFadeScreen(s32 arg0) {
    g_scene.unk3 = arg0;
    if (scene_get() == 1) {
        opening_titleRefresh();
    }
}

u8 scene_getFadeScreen(void) {
    return g_scene.unk3;
}

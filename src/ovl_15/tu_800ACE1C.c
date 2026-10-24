#include "ovl_15.h"

void scene_init(void) {
    s32 i;
    SceneMode *m;

    i = 0;
    m = g_sceneModes;
    g_scene.cur = 0;
    g_scene.unk2 = 0;
    g_scene.fadeScreen = 0;
    for (; i < SCENE_COUNT; i++, m++) {
        if (m->init != NULL) {
            m->init();
        }
    }
    if (g_sceneModes[g_scene.cur].enter != NULL) {
        g_sceneModes[g_scene.cur].enter();
    }
}

void scene_update(void) {
    void (*f)(void) = g_sceneModes[g_scene.cur].update;
    if (f != NULL) {
        f();
    }
}

void scene_exit(void) {
    void (*f)(void) = g_sceneModes[g_scene.cur].leave;
    if (f != NULL) {
        f();
    }
}

u8 scene_get(void) {
    return g_scene.cur;
}

void scene_set(s32 arg0) {
    if (arg0 != g_scene.cur) {
        if (g_sceneModes[g_scene.cur].leave != NULL) {
            g_sceneModes[g_scene.cur].leave();
        }
        g_scene.cur = arg0;
        if (g_sceneModes[g_scene.cur].enter != NULL) {
            g_sceneModes[g_scene.cur].enter();
        }
    }
}

s32 scene_checkDisc(void) {
    u32 i = 0;
    s32 pad = g_gameState->unkB;

    do {
        func_8001DCB8();
    } while (g_gameState->unkB == DISC_NONE && ++i < 10);
    func_80033D1C(1);
    if (scene_isDiscGone(pad, g_gameState->unkB)) {
        return scene_discIndex(pad);
    }
    return DISC_NONE;
}

s32 scene_discIndex(s32 arg0) {
    s32 i;
    s32 mask;

    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        if (arg0 & mask) {
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

s32 scene_isMenuIdle(void) {

    if (func_8002F810() == 0) {
        return (u32)(g_scene.cur - 1) < 3;
    }
    return 0;
}

void scene_setFadeScreen(s8 arg0) {
    g_scene.fadeScreen = arg0;
}

u8 scene_getFadeScreen(void) {
    return g_scene.fadeScreen;
}

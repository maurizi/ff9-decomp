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

s32 scene_get(void) {
    return g_scene.cur;
}

void scene_set(s32 scene) {
    if (scene != g_scene.cur) {
        if (g_sceneModes[g_scene.cur].leave != NULL) {
            g_sceneModes[g_scene.cur].leave();
        }
        g_scene.cur = scene;
        if (g_sceneModes[g_scene.cur].enter != NULL) {
            g_sceneModes[g_scene.cur].enter();
        }
    }
}

s32 scene_checkDisc(void) {
    u32 i = 0;
    s32 pad = g_gameState->disc;

    do {
        file_initImg();
    } while (g_gameState->disc == DISC_NONE && ++i < 10);
    party_setChanged(1);
    if (scene_isDiscGone(pad, g_gameState->disc)) {
        return scene_discIndex(pad);
    }
    return DISC_NONE;
}

s32 scene_discIndex(s32 discMask) {
    s32 i;
    s32 bit;

    for (i = 0; i < 4; i++) {
        bit = 1 << i;
        if (discMask & bit) {
            return i;
        }
    }
    return 0;
}

u8 scene_isDiscGone(s32 needMask, s32 discMask) {
    s32 a = needMask & 0xF;
    s32 b = discMask & 0xF;
    return (b & a) != a;
}

s32 scene_isMenuIdle(void) {

    if (fade_isBusy() == 0) {
        return (u32)(g_scene.cur - 1) < 3;
    }
    return 0;
}

void scene_setFadeScreen(s8 fade) {
    g_scene.fadeScreen = fade;
}

u8 scene_getFadeScreen(void) {
    return g_scene.fadeScreen;
}

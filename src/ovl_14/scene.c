#include "ovl_14.h"

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
    if (g_sceneModes[g_scene.cur].update != NULL) {
        g_sceneModes[g_scene.cur].update();
    }
}

void scene_exit(void) {
    if (g_sceneModes[g_scene.cur].leave != NULL) {
        g_sceneModes[g_scene.cur].leave();
    }
}

s32 scene_get(void) {
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
    u32 i;
    s32 prev;

    i = 0;
    prev = g_gameState->disc;
    do {
        file_initImg();
    } while (g_gameState->disc == DISC_NONE && ++i < 10);
    party_setChanged(1);
    if (scene_isDiscGone(prev, g_gameState->disc)) {
        return scene_discIndex(prev);
    }
    return DISC_NONE;
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
    if (fade_isBusy() != 0) {
        return 0;
    }
    return g_scene.cur == SCENE_TITLE;
}

void scene_setFadeScreen(s32 arg0) {
    g_scene.fadeScreen = arg0;
    if (scene_get() == SCENE_TITLE) {
        opening_titleRefresh();
    }
}

u8 scene_getFadeScreen(void) {
    return g_scene.fadeScreen;
}

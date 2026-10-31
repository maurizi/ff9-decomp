#ifndef SND_IDS_H
#define SND_IDS_H
/* Sound ids: AKAO command numbers, sound-effect ids, music ids, SPU register offsets. Included by ff9.h.
 * Evidence for each group: docs/sound-ids.md. */

/* AKAO driver command numbers: akao_command(cmd) runs g_akaoCommandHandlers[cmd] (main 0x8006F740, 0x100
 * entries, unused slots akao_cmdNop). Each name is its handler's; where the handler is still a placeholder
 * (akao_cmdA2, ...) the name says what its body does. The akao_api.c senders fill g_akaoArgs first. */
#define AKAO_CMD_PLAY_MUSIC            0x10 /* akao_cmdPlayMusic */
#define AKAO_CMD_STOP_MUSIC            0x11 /* akao_cmdStopMusic */
#define AKAO_CMD_PLAY_MUSIC_COUNT      0x12 /* akao_cmdPlayMusicCount */
#define AKAO_CMD_PLAY_MUSIC_FROM       0x14 /* akao_cmdPlayMusicFrom */
#define AKAO_CMD_PUSH_MUSIC            0x19 /* akao_cmdPushMusic */
#define AKAO_CMD_CROSSFADE_MUSIC       0x1A /* akao_cmdCrossfadeMusic */
#define AKAO_CMD_PLAY_SE               0x20 /* akao_cmdPlaySe: SE number from the resident tables */
#define AKAO_CMD_STOP_SE               0x21 /* akao_cmdStopSe */
#define AKAO_CMD_PLAY_SE_BANK          0x24 /* akao_cmdPlaySeBank: arg0 points at AKAO data */
#define AKAO_CMD_PLAY_SE_DEFAULT       0x30 /* akao_cmdPlaySeDefault */
#define AKAO_CMD_PLAY_SE_DATA          0x34 /* akao_cmdPlaySeData */
#define AKAO_CMD_SAVE_MUSIC            0x40 /* akao_cmdSaveMusic */
#define AKAO_CMD_SET_CD_VOLUME         0x70 /* akao_cmdSetCdVolume */
#define AKAO_CMD_FADE_CD_VOLUME        0x71 /* akao_cmdFadeCdVolume */
#define AKAO_CMD_FADE_CD_VOLUME_FROM   0x72 /* akao_cmdFadeCdVolumeFrom */
#define AKAO_CMD_SET_OUTPUT_MODE1      0x80 /* akao_cmdSetOutputMode1 (akao_setOutputMode(!= 1)) */
#define AKAO_CMD_SET_OUTPUT_MODE2      0x81 /* akao_cmdSetOutputMode2 (akao_setOutputMode(1)) */
#define AKAO_CMD_SET_MUTE_MASK         0x90 /* akao_cmdSetMuteMask */
#define AKAO_CMD_SET_CONDITION         0x92 /* akao_cmdSetCondition */
#define AKAO_CMD_RESUME_ALL            0x98 /* akao_command: RESUME_MUSIC + RESUME_SE, then RESUME_STREAM */
#define AKAO_CMD_PAUSE_ALL             0x99 /* akao_command: PAUSE_MUSIC + PAUSE_SE, then PAUSE_STREAM */
#define AKAO_CMD_RESUME_MUSIC          0x9A /* akao_cmdResumeMusic */
#define AKAO_CMD_PAUSE_MUSIC           0x9B /* akao_cmdPauseMusic */
#define AKAO_CMD_RESUME_SE             0x9C /* akao_cmdResumeSe */
#define AKAO_CMD_PAUSE_SE              0x9D /* akao_cmdPauseSe */
#define AKAO_CMD_RESUME_STREAM         0x9E /* akao_cmdResumeStream */
#define AKAO_CMD_PAUSE_STREAM          0x9F /* akao_cmdPauseStream */
#define AKAO_CMD_SET_SE_VOLUME         0xA0 /* akao_cmdSetSeVolume */
#define AKAO_CMD_FADE_SE_VOLUME        0xA1 /* akao_cmdFadeSeVolume */
#define AKAO_CMD_SET_SE_PAN            0xA2 /* akao_cmdA2: sePan of the matching SE channels */
#define AKAO_CMD_FADE_SE_PAN           0xA3 /* akao_cmdA3: sePanStep/sePanSlideLen */
#define AKAO_CMD_SET_SE_PITCH          0xA4 /* akao_cmdA4: sePitch */
#define AKAO_CMD_FADE_SE_PITCH         0xA5 /* akao_cmdA5: sePitchStep/sePitchSlideLen */
#define AKAO_CMD_SET_ALL_SE_VOLUME     0xA8 /* akao_cmdSetAllSeVolume */
#define AKAO_CMD_FADE_ALL_SE_VOLUME    0xA9 /* akao_cmdFadeAllSeVolume */
#define AKAO_CMD_SET_ALL_SE_PAN        0xAA /* akao_cmdAA: sePan of every SE channel */
#define AKAO_CMD_FADE_ALL_SE_PAN       0xAB /* akao_cmdAB */
#define AKAO_CMD_SET_ALL_SE_PITCH      0xAC /* akao_cmdAC: sePitch of every SE channel */
#define AKAO_CMD_FADE_ALL_SE_PITCH     0xAD /* akao_cmdAD */
#define AKAO_CMD_SET_MUSIC_VOLUME      0xC0 /* akao_cmdSetMusicVolume */
#define AKAO_CMD_FADE_MUSIC_VOLUME     0xC1 /* akao_cmdFadeMusicVolume */
#define AKAO_CMD_FADE_MUSIC_VOLUME_FROM 0xC2 /* akao_cmdFadeMusicVolumeFrom */
#define AKAO_CMD_SET_MUSIC_PAN         0xC4 /* akao_cmdC4: AkaoSeq.pan */
#define AKAO_CMD_FADE_MUSIC_PAN        0xC5 /* akao_cmdC5: AkaoSeq.panStep/panSlideLen */
#define AKAO_CMD_SET_TEMPO_SCALE       0xD0 /* akao_cmdSetTempoScale */
#define AKAO_CMD_FADE_TEMPO_SCALE      0xD1 /* akao_cmdFadeTempoScale */
#define AKAO_CMD_FADE_TEMPO_SCALE_FROM 0xD2 /* akao_cmdFadeTempoScaleFrom */
#define AKAO_CMD_SET_PITCH_SCALE       0xD4 /* akao_cmdSetPitchScale */
#define AKAO_CMD_FADE_PITCH_SCALE      0xD5 /* akao_cmdFadePitchScale */
#define AKAO_CMD_FADE_PITCH_SCALE_FROM 0xD6 /* akao_cmdFadePitchScaleFrom */
#define AKAO_CMD_SET_TEMPO_PITCH_SCALE 0xD8 /* akao_command: SET_TEMPO_SCALE, then SET_PITCH_SCALE */
#define AKAO_CMD_FADE_TEMPO_PITCH_SCALE 0xD9 /* FADE_TEMPO_SCALE, then FADE_PITCH_SCALE */
#define AKAO_CMD_FADE_TEMPO_PITCH_SCALE_FROM 0xDA /* FADE_TEMPO_SCALE_FROM, then FADE_PITCH_SCALE_FROM */
#define AKAO_CMD_STREAM_START          0xE0 /* akao_cmdE0: akao_streamStop, akao_streamStart */
#define AKAO_CMD_STREAM_STOP           0xE2 /* akao_cmdE2: akao_streamStop */
#define AKAO_CMD_SET_STREAM_VOLUME     0xE4 /* akao_cmdE4: g_akaoStream.volume */
#define AKAO_CMD_FADE_STREAM_VOLUME    0xE5 /* akao_cmdE5: g_akaoStream.volumeStep/volumeFadeLen */
#define AKAO_CMD_SET_STREAM_PAN        0xE6 /* akao_cmdE6: g_akaoStream.pan */
#define AKAO_CMD_STREAM_SETUP_RING     0xE8 /* akao_cmdE8: ring-buffer stream state (akao_streamStartRing) */
#define AKAO_CMD_STREAM_PLAY_ONE_SHOT  0xEC /* akao_cmdEC: akao_streamPlayOneShot */
#define AKAO_CMD_STREAM_PLAY_RESIDENT  0xED /* akao_cmdED: akao_streamPlayResident */
#define AKAO_CMD_STOP_MUSIC_NOW        0xF0 /* akao_cmdStopMusicNow */
#define AKAO_CMD_STOP_ALL_SE           0xF1 /* akao_cmdStopAllSe */

#endif

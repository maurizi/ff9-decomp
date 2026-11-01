#ifndef SND_IDS_H
#define SND_IDS_H
/* Sound ids: AKAO command numbers, sound-effect ids, music ids, SPU register offsets. Included by ff9.h.
 * Evidence for each group: docs/sound-ids.md. */

/* AKAO driver command numbers: akao_command(cmd) runs g_akaoCommandHandlers[cmd] (main 0x8006F740, 0x100
 * entries, unused slots akao_cmdNop). Each name is its handler's; where the handler is still a placeholder
 * (akao_cmdSetSePan, ...) the name says what its body does. The akao_api.c senders fill g_akaoArgs first. */
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
#define AKAO_CMD_SET_SE_PAN            0xA2 /* akao_cmdSetSePan: sePan of the matching SE channels */
#define AKAO_CMD_FADE_SE_PAN           0xA3 /* akao_cmdFadeSePan: sePanStep/sePanSlideLen */
#define AKAO_CMD_SET_SE_PITCH          0xA4 /* akao_cmdSetSePitch: sePitch */
#define AKAO_CMD_FADE_SE_PITCH         0xA5 /* akao_cmdFadeSePitch: sePitchStep/sePitchSlideLen */
#define AKAO_CMD_SET_ALL_SE_VOLUME     0xA8 /* akao_cmdSetAllSeVolume */
#define AKAO_CMD_FADE_ALL_SE_VOLUME    0xA9 /* akao_cmdFadeAllSeVolume */
#define AKAO_CMD_SET_ALL_SE_PAN        0xAA /* akao_cmdSetAllSePan: sePan of every SE channel */
#define AKAO_CMD_FADE_ALL_SE_PAN       0xAB /* akao_cmdFadeAllSePan */
#define AKAO_CMD_SET_ALL_SE_PITCH      0xAC /* akao_cmdSetAllSePitch: sePitch of every SE channel */
#define AKAO_CMD_FADE_ALL_SE_PITCH     0xAD /* akao_cmdFadeAllSePitch */
#define AKAO_CMD_SET_MUSIC_VOLUME      0xC0 /* akao_cmdSetMusicVolume */
#define AKAO_CMD_FADE_MUSIC_VOLUME     0xC1 /* akao_cmdFadeMusicVolume */
#define AKAO_CMD_FADE_MUSIC_VOLUME_FROM 0xC2 /* akao_cmdFadeMusicVolumeFrom */
#define AKAO_CMD_SET_MUSIC_PAN         0xC4 /* akao_cmdSetMusicPan: AkaoSeq.pan */
#define AKAO_CMD_FADE_MUSIC_PAN        0xC5 /* akao_cmdFadeMusicPan: AkaoSeq.panStep/panSlideLen */
#define AKAO_CMD_SET_TEMPO_SCALE       0xD0 /* akao_cmdSetTempoScale */
#define AKAO_CMD_FADE_TEMPO_SCALE      0xD1 /* akao_cmdFadeTempoScale */
#define AKAO_CMD_FADE_TEMPO_SCALE_FROM 0xD2 /* akao_cmdFadeTempoScaleFrom */
#define AKAO_CMD_SET_PITCH_SCALE       0xD4 /* akao_cmdSetPitchScale */
#define AKAO_CMD_FADE_PITCH_SCALE      0xD5 /* akao_cmdFadePitchScale */
#define AKAO_CMD_FADE_PITCH_SCALE_FROM 0xD6 /* akao_cmdFadePitchScaleFrom */
#define AKAO_CMD_SET_TEMPO_PITCH_SCALE 0xD8 /* akao_command: SET_TEMPO_SCALE, then SET_PITCH_SCALE */
#define AKAO_CMD_FADE_TEMPO_PITCH_SCALE 0xD9 /* FADE_TEMPO_SCALE, then FADE_PITCH_SCALE */
#define AKAO_CMD_FADE_TEMPO_PITCH_SCALE_FROM 0xDA /* FADE_TEMPO_SCALE_FROM, then FADE_PITCH_SCALE_FROM */
#define AKAO_CMD_STREAM_START          0xE0 /* akao_cmdStreamStart: akao_streamStop, akao_streamStart */
#define AKAO_CMD_STREAM_STOP           0xE2 /* akao_cmdStreamStop: akao_streamStop */
#define AKAO_CMD_SET_STREAM_VOLUME     0xE4 /* akao_cmdSetStreamVolume: g_akaoStream.volume */
#define AKAO_CMD_FADE_STREAM_VOLUME    0xE5 /* akao_cmdFadeStreamVolume: g_akaoStream.volumeStep/volumeFadeLen */
#define AKAO_CMD_SET_STREAM_PAN        0xE6 /* akao_cmdSetStreamPan: g_akaoStream.pan */
#define AKAO_CMD_STREAM_SETUP_RING     0xE8 /* akao_cmdStreamSetupRing: ring-buffer stream state (akao_streamStartRing) */
#define AKAO_CMD_STREAM_PLAY_ONE_SHOT  0xEC /* akao_cmdStreamPlayOneShot: akao_streamPlayOneShot */
#define AKAO_CMD_STREAM_PLAY_RESIDENT  0xED /* akao_cmdStreamPlayResident: akao_streamPlayResident */
#define AKAO_CMD_STOP_MUSIC_NOW        0xF0 /* akao_cmdStopMusicNow */
#define AKAO_CMD_STOP_ALL_SE           0xF1 /* akao_cmdStopAllSe */

/* g_akaoPauseFlags */
#define AKAO_PAUSED_MUSIC 0x1 /* akao_cmdPauseMusic / akao_cmdResumeMusic; new songs start paused */
#define AKAO_PAUSED_SE    0x2 /* akao_cmdPauseSe / akao_cmdResumeSe; new SEs start paused */
#define AKAO_PAUSE_FORCE_TICK 0x4 /* both tempo loops step the sequencer every frame while it is set. Never set: the
                                   * only writers are the pause/resume commands (1, 2) and akao_init (0), no overlay
                                   * references the flags, runtime write watch saw none. Medium: a debug step? */
/* AkaoChannel.seMask (VoiceParams.mask) bit given by akao_cmdPlaySeDefault: the SE keeps running through
 * akao_cmdPauseSe and ignores the all-SE volume/pan/pitch commands and its own sePan/seVolume/sePitch
 * (akao_calcSeChannelVoice plays it centred, unscaled) */
#define AKAO_SE_DEFAULT 0x2000000

/* AkaoStreamState.flags (copied from the stream header's flags; akao_cmdStreamSetupRing sets RING_PENDING alone) */
#define AKAO_STREAM_STEREO       0x1 /* voice plays left only, voice + 1 right only (akao_cmdSetStreamVolume/Pan); one-shot upload via akao_streamStartStereo */
#define AKAO_STREAM_LOOP         0x2 /* loopStart is valid: loopPtr/loopBytes set (akao_streamPlayOneShot) */
#define AKAO_STREAM_FOUR_VOICES  0x400 /* ring streams use voice - 2 and voice - 1 too (twice the SPU ring) */
#define AKAO_STREAM_RING_PENDING 0x1000000 /* akao_streamPushBlock starts the ring once two blocks are written */

/* AKAO sequence opcodes (akao_execChannel, akao_peekNextNote). Bytes from AKAO_OP_FIRST_CMD up are commands run
 * through g_akaoOps[op - 0xA0] (lengths g_akaoOpLengths); AKAO_OP_EXT takes a second byte that indexes
 * g_akaoOpsFE. Opcodes are named from their handlers (named, or what the akao_opXX placeholder does). */
#define AKAO_OP_FIRST_CMD 0xA0
#define AKAO_OP_END       0xA0 /* akao_opEnd; akao_peekNextNote's "no further note" result */
#define AKAO_OP_PROGRAM   0xA1 /* akao_opProgram */
#define AKAO_OP_OCTAVE    0xA5 /* akao_opOctave */
#define AKAO_OP_OCTAVE_UP 0xA6 /* akao_opOctaveUp */
#define AKAO_OP_OCTAVE_DOWN 0xA7 /* akao_opOctaveDown */
/* note bytes below AKAO_OP_FIRST_CMD: note / AKAO_NOTE_LENGTHS is the pitch (or tie / rest), note % AKAO_NOTE_LENGTHS
 * indexes g_akaoNoteLengths (akao_execChannel) */
#define AKAO_NOTE_LENGTHS 11
#define AKAO_NOTE_TIE  0x84 /* 0x84-0x8E: tie, the note goes on (no key-on) */
#define AKAO_NOTE_REST 0x8F /* 0x8F-0x99: rest (key-off) */
#define AKAO_NOTE_END  0x9A /* 0x9A-0x9F aren't notes (akao_peekNextNote: AKAO_OP_END) */
#define AKAO_OP_LOOP_START   0xC8 /* akao_opC8: push a loop (depth 4) at the next byte */
#define AKAO_OP_LOOP_END     0xC9 /* + count (0: 256): akao_opC9, back to the loop start until the count is reached */
#define AKAO_OP_LOOP_FOREVER 0xCA /* akao_opCA: back to the loop start (ends the channel once AKAO_CH_STOP_REQUESTED) */
#define AKAO_OP_EFFECTS_OFF  0xCB /* akao_opCB: vibrato, tremolo, pan LFO, noise, pitch mod and reverb off, legato off */
#define AKAO_OP_NOP_CD       0xCD /* akao_opCD: empty; akao_peekNextNote treats it as ending a legato */
#define AKAO_OP_NOP_D1       0xD1 /* akao_opD1: empty, the same */
#define AKAO_OP_PORTAMENTO_OFF 0xDB /* akao_opDB: portamentoLen = 0 */
#define AKAO_OP_NOTE_LEN 0xF0 /* 0xF0-0xFB: note (pitch op - 0xF0) + u8 length in ticks; not in g_akaoOps */
#define AKAO_OP_TIE_LEN  0xFC /* + u8 length: tie */
#define AKAO_OP_REST_LEN 0xFD /* + u8 length: rest */
#define AKAO_OP_EXT       0xFE /* prefix of the g_akaoOpsFE opcodes */
#define AKAO_OPFE_TEMPO       0x00 /* akao_opFETempo */
#define AKAO_OPFE_TEMPO_SLIDE 0x01 /* akao_opFETempoSlide */
#define AKAO_OPFE_JUMP        0x06 /* akao_opFEJump */
#define AKAO_OPFE_JUMP_IF_GE  0x07 /* akao_opFEJumpIfGe (AKAO_CMD_SET_CONDITION's value) */
#define AKAO_OPFE_LOOP_PASS_JUMP 0x08 /* + pass (0: 256), s16: akao_opFE08, jump on that pass of the loop (1-based) */
#define AKAO_OPFE_LOOP_PASS_EXIT 0x09 /* + pass, s16: akao_opFE09, the same and leave the loop */
#define AKAO_OPFE_CALL        0x0E /* akao_opFECall */
#define AKAO_OPFE_RETURN      0x0F /* akao_opFEReturn */

/* Sound command words for the mode's sound handler (GameData.soundHook: fld_soundCommand, wm_soundCommand,
 * btl_soundCmd, and the mode-local opening_soundCmd / tm_soundCmd / ending_soundCmd, which decode them the same way).
 * Bits 12-13 type (0 music, 1 SE, 2 stream, 3 bank), 8-11 kind, 0-5 sub-command, 7 resident SE slot, 6 slot index.
 * The SE/state/volume words carry the world map's debug-print names (ovl_0c WM_SND_*). */
#define SND_MUSIC_PLAY 0x0000
#define SND_MUSIC_STOP 0x0100
#define SND_MUSIC_LOAD 0x0700 /* load only */
#define SND_MUSIC_SUSPEND 0x0800 /* save song/volume, then stop */
#define SND_MUSIC_RESUME 0x0900 /* replay the saved song */
#define SND_GET_STATE 0x0C00 /* "Waiting [ State : %d ]": non-zero while loading */
#define SND_SE_PLAY 0xD000 /* "SE Play" (snd_playSe) */
#define SND_SE_STOP 0x5100 /* "SE Stop" */
#define SND_SE_VOL 0x9201 /* "SE Vol" */
#define SND_SE_PITCH 0x9203
#define SND_SE_VOL_INTPL 0xD301 /* "SE Vol Intpl" */
#define SND_SONG_VOL_INTPL 0x8301 /* kind 3: akao_fadeMusicVolume on the mode's song handle */
#define SND_SONG_VOL_FROM_INTPL 0xC401 /* kind 4: akao_fadeMusicVolumeFrom on the song handle */
#define SND_MUSIC_VOL_INTPL 0x8601 /* "Music Vol Intpl", "Music Fade Force" */
#define SND_SE_SUSPEND 0x1800
#define SND_SE_RESUME 0x1900
#define SND_STREAM_VOL 0x6201 /* type 2 (streamed sound, db 0x18xxxx) kind 2 sub 1: stream volume (akao_sendCmdE4) */
#define SND_BANK_LOAD 0x7700
#define SND_RESIDENT 0x80
/* Fields of a sound command word, as every handler decodes them (fld_soundCommand, wm_soundCommand, tm_soundCmd,
 * opening_soundCmd, ending_soundCmd). The event script's sound ops pass the word through unchanged. */
/* type: (w >> 12) & 3 */
#define SND_TYPE_MUSIC  0 /* id | DB_MUSIC */
#define SND_TYPE_SE     1 /* id | DB_SE */
#define SND_TYPE_STREAM 2 /* id | 0x180000, akao_sendCmdE0..E6 (field only) */
#define SND_TYPE_BANK   3 /* id | DB_SOUND_BANK (field, world map) */
/* command: (w >> 8) & 0xF. A set/fade command picks the property with SND_PARAM_*. */
#define SND_CMD_PLAY          0x0 /* akao_playMusic / akao_playSe / akao_sendCmdE0 */
#define SND_CMD_STOP          0x1 /* akao_stopMusic / akao_stopSe / akao_sendCmdE2 */
#define SND_CMD_SET           0x2 /* set a property at once (akao_setMusicVolume, akao_setSeVolume, ...) */
#define SND_CMD_FADE          0x3 /* fade a property to param2 over param1 frames (akao_fadeMusicVolume, ...) */
#define SND_CMD_FADE_FROM     0x4 /* fade from param2 to param3 over param1 frames (akao_fadeMusicVolumeFrom, ...) */
#define SND_CMD_SET_ALL       0x5 /* SE: every SE (akao_setAllSeVolume ...); no id lookup */
#define SND_CMD_FADE_ALL      0x6 /* SE: every SE (akao_fadeAllSeVolume ...); music: handle 0, the main song whatever its id */
#define SND_CMD_LOAD          0x7 /* music: load without starting; bank: load the bank */
#define SND_CMD_SUSPEND       0x8 /* save the song (or resident SE slot) for this field / world, then stop */
#define SND_CMD_RESUME        0x9 /* replay what SUSPEND saved, if it was saved on this field / world */
#define SND_CMD_SET_CONDITION 0xA /* music, with SND_PARAM_CONDITION: akao_setCondition(id) */
#define SND_CMD_FADE_ALL_FROM 0xB /* music: akao_fadeMusicVolumeFrom on handle 0 */
#define SND_CMD_GET_STATE     0xC /* steps a pending load; non-zero until done (6: music still fading out) */
/* parameter of SND_CMD_SET / FADE / FADE_FROM / SET_ALL / FADE_ALL / FADE_ALL_FROM: w & 0x3F */
#define SND_PARAM_VOLUME      1
#define SND_PARAM_TEMPO       2 /* music: akao_setTempoScale / akao_fadeTempoScale(From) (field only) */
#define SND_PARAM_PITCH       3 /* music: akao_setPitchScale ...; SE: akao_sendCmdA4/A5/AC/AD (AKAO_CMD_*_SE_PITCH) */
#define SND_PARAM_TEMPO_PITCH 4 /* music: akao_setTempoPitchScale ... (field only) */
#define SND_PARAM_PAN         5 /* SE: akao_sendCmdA2/A3/AA/AB (AKAO_CMD_*_SE_PAN); stream: akao_sendCmdE6 */
#define SND_PARAM_CONDITION   6 /* SND_CMD_SET_CONDITION */
#define SND_PARAM_PLAY_COUNT  7 /* music SND_CMD_SET: akao_playMusicCount(seq, param1) (field only) */
#define SND_PARAM_ALL_SLOTS   9 /* SE SND_CMD_STOP with SND_RESIDENT: stop both resident SE slots (field only) */
#define SND_PARAM_MOVIE_VOLUME 10 /* stream SND_CMD_SET: the stream volume used while a field movie plays */
#define SND_SLOT1 0x40 /* with SND_RESIDENT on PLAY / SUSPEND / RESUME: resident SE slot 1 instead of 0 (field) */
/* pan and volume arguments of SND_SE_PLAY (akao_playSe keeps pan & 0xFF and volume & 0x7F) */
#define SND_PAN_CENTER 0x80
#define SND_VOL_MAX 0x7F

/* Sound-effect ids (SND_SE_PLAY / snd_playSe). Shared by several binaries; ids used by one screen only keep
 * their name in that overlay's header. Runtime evidence: docs/runtime-findings.md "Sound ids". */
#define SE_CANCEL 0x65 /* runtime: Circle backs out (menus, battle), help window closes */
#define SE_BUZZER 0x66 /* runtime: refused action (unusable item, item on a full-HP member) */
#define SE_CURSOR 0x67 /* runtime: cursor move (menuList_up/down) and a pick with Cross */
#define SE_USE_ITEM 0x6A /* code: an item (ovl_37) or ability (ovl_33) was used */
#define SE_SET 0x6B /* runtime: equipment changed (ovl_33); code: chocograph set or cleared (ovl_32) */
#define SE_HELP_OPEN 0x2AA /* runtime: Select opens the help window (help_toggle, btlui_toggleHelp) */
#define SE_WINDOW_OPEN 0x414 /* runtime: battle command menu opens; code: save file list fades in (ovl_39) */
#define SE_MESSAGE 0x416 /* code: a message window opens (ovl_39 save, ovl_65 "cannot carry more") */
#define SE_PAGE 0x417 /* runtime: menuList page, L1/R1 member switch, battle command side switch */
#define SE_COMPLETE 0x4ED /* code: save/load finished (ovl_39), the right disc was found (ovl_3e) */
#define SE_GAME_START 0xC18 /* code: New Game on the title (ovl_14), the ending's secret code (ovl_15) */
#define SE_CHOCOBO_CALL 0x44A /* code: looped while the item menu's "CHOCOBO" / "Kweeeh!" window is up on the world map
                               * (ovl_37 menuItem_onKwehOpened); world-map SE number 36 (w_musicGetID) */
#define SE_NONE 0xFFFF /* GameData.sfxSlotId: no sound effect in the resident slot */
/* encounter: the three sounds played together when a random battle starts (field fld_movie, world map) */
#define SE_ENCOUNTER_1 0x27C
#define SE_ENCOUNTER_2 0x27B
#define SE_ENCOUNTER_3 0x27A
/* battle */
#define SE_ENEMY_DEATH_1 0x79 /* btl_updateKo, as a defeated enemy starts to fade (then _2) */
#define SE_ENEMY_DEATH_2 0x78
#define SE_FLEE_1 0xB5A /* btl_victory: the party escapes (three sounds together) */
#define SE_FLEE_2 0xB5B
#define SE_FLEE_3 0xB5C

/* Music ids (SND_MUSIC_PLAY / SND_MUSIC_LOAD), only where the code says which song */
#define MUSIC_NONE 0xFFFF /* GameData.musicId / savedMusicId, FieldScene / WorldMapObj battleMusic: no song */
#define MUSIC_VICTORY 5 /* btl_playVictoryMusic after BTL_END_VICTORY */
#define MUSIC_DEFEAT 6 /* btl_playVictoryMusic in BTL_PHASE_EXIT_DEFEAT (game over) */
#define MUSIC_TETRA_MASTER 0x42 /* ovl_0d tm_init plays it as the card game starts */

/* SPU registers (psx-spx names) */
#define SPU_VOICE_REG(v, r) (*(u16 *)(SPU_VOICE_BASE + (r) + (v) * 16))
#define SPU_VOICE_REG8(v, r) (*(u8 *)(SPU_VOICE_BASE + (r) + (v) * 16))
#define SPU_REG16(a) (*(u16 *)(a))
#define SPU_VOICE_BASE 0x1F801C00 /* 24 voices x 16 bytes (SpuVoiceReg) */
/* voice register offsets (r of SPU_VOICE_REG; SpuVoiceReg fields) */
#define SPU_VOICE_VOL_L 0x0
#define SPU_VOICE_VOL_R 0x2
#define SPU_VOICE_PITCH 0x4 /* sample rate, 0x1000 = 44.1 kHz */
#define SPU_VOICE_START_ADDR 0x6 /* SPU RAM address / 8 */
#define SPU_VOICE_ADSR1 0x8
#define SPU_VOICE_ADSR2 0xA
#define SPU_VOICE_ADSR_VOL 0xC
#define SPU_VOICE_LOOP_ADDR 0xE /* repeat address / 8 */
/* ADSR register fields (psx-spx "SPU Volume and ADSR Generator"; PsyQ SpuVoiceAttr ar/dr/sl, sr/rr and the
 * *_MODE fields). ADSR2 bit 13 is unused by the SPU; akao keeps it in some masks (0x201F, 0x3FDF), left raw. */
#define SPU_ADSR1_SUSTAIN_LEVEL 0x000F
#define SPU_ADSR1_DECAY_RATE    0x00F0
#define SPU_ADSR1_ATTACK_RATE   0x7F00 /* shift and step */
#define SPU_ADSR1_ATTACK_EXP    0x8000 /* exponential attack */
#define SPU_ADSR2_RELEASE_RATE  0x001F
#define SPU_ADSR2_RELEASE_EXP   0x0020 /* exponential release */
#define SPU_ADSR2_SUSTAIN_RATE  0x1FC0 /* shift and step */
#define SPU_ADSR2_SUSTAIN_DEC   0x4000 /* sustain decreases */
#define SPU_ADSR2_SUSTAIN_EXP   0x8000 /* exponential sustain */
/* AKAO envelope mode numbers (instrument region sustainMode, ops B7/BB/BF, akao_setVoiceAttack/Sustain/Release's
 * mode argument): bit 1 = decreasing, bit 2 = exponential (akao_setVoiceSustain: (mode >> 1) << 14,
 * akao_setVoiceAttack/Release: (mode >> 2) into the exp bit). */
#define AKAO_ENV_LINEAR_INC 1
#define AKAO_ENV_LINEAR_DEC 3
#define AKAO_ENV_EXP_INC    5
#define AKAO_ENV_EXP_DEC    7
#define SPU_MAIN_VOL_L 0x1F801D80
#define SPU_MAIN_VOL_R 0x1F801D82
#define SPU_KEY_ON_LO 0x1F801D88
#define SPU_KEY_ON_HI 0x1F801D8A
#define SPU_KEY_OFF_LO 0x1F801D8C
#define SPU_KEY_OFF_HI 0x1F801D8E
#define SPU_PITCH_MOD_LO 0x1F801D90
#define SPU_PITCH_MOD_HI 0x1F801D92
#define SPU_NOISE_ON_LO 0x1F801D94
#define SPU_NOISE_ON_HI 0x1F801D96
#define SPU_REVERB_ON_LO 0x1F801D98
#define SPU_REVERB_ON_HI 0x1F801D9A
#define SPU_CTRL 0x1F801DAA
#define SPU_CD_VOL_L 0x1F801DB0
#define SPU_CD_VOL_R 0x1F801DB2
#define SPU_MAIN_VOL_MAX 0x3FFF /* main volume in fixed mode: 0x3FFF is the top of the range */
#define SPU_CD_VOL_MAX 0x7FFF

#endif

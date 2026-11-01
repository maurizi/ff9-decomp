# Literals pass: runtime questions

Literals the constants pass left as numbers because only the running game can name them. Each entry says where the
value is used and what to look at. Answers go to docs/runtime-findings.md as usual.

## Battle motion slots (ovl_0b, BtlUnit.motions)
Named so far: `BTL_MOTION_*` in include/ovl_0b.h. Still numbers:
- **8**: btl_status.c (revive from KO): `motions[BTL_MOTION_KO]` or `motions[8]` switch to GET_UP. Is 8 a second KO
  pose (dying / lying)? Poke a party model's motion to slot 8 (as in the slot 2/3 test) and screenshot.
- **0x12**: btl_update.c, the victory exit: played instead of the idle when `Character.info` bit 10 is set.
- **0x13**: btl_update.c, the exit phase with a victory pose: played after the current motion ends (victory loop?).
- **0x20 (32)**: btl_cmd.c btl_execCmdPhase phase 0: a READY member whose command can't be used (no MP, no target)
  plays it. READY back to idle?
- **0x1A, 0x1B, 0x1C, 0x1F**: btlfx_opCast: 0x1A starts the cast (motionMode != 3), 0x1F instead for mode 3, 0x1B once
  the start motion ends, 0x1C repeats while signal 2 is set (frame >= 27). Watch a Black Magic cast (Vivi) and a
  White Magic cast, screenshot each change.
- **0x1D, 0x1E, 0x20**: btlfx_stepBack (target an enemy, frame 0), btlfx_moveUnitHome (moving), btlfx_opReturnHome
  (arrived and not stopped). Step forward / run back / land?

## Menu sprite ids (main g_uiSprites)
- **0x53, 0x54, 0x80-0x82, 0x1F-0x21**: msg.c ATE prompt (the "Active Time Event" hint at 16,200): two styles that
  alternate every 44 frames, and the blink style. What do the sprites show?
- `UI_SPRITE_HELP_TAG` (0xB7): drawn on the help window's top edge in the field menu and battle. A "Help" tab?
  Screenshot to confirm the name.
- `UI_SPRITE_NEW` / `UI_SPRITE_NEW_BLINK` (0xB5/0xB6): unseen key items (ovl_37) and TEXT_FLAG_ICON. Confirm the look.

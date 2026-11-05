# False pointer words in data

A port that relocates data by treating every `.word` whose value falls in PS1 RAM
(0x80000000-0x801FFFFF) as a pointer corrupts byte data that happens to contain such a
word (the 3DS port broke a trance camera in ovl_0b's `g_btlfxCommonData` this way:
0x800F84AC holds the bytes `0a 00 01 80` of a camera command).

Blocks below are typed in the symbols files (`type:u8 size:N`) so splat emits them as
`.byte`, and a `.word` relocation pass can no longer mistake them. Types only change
the asm listing, so all 20 binaries and the 380 effect modules stay byte-identical.

`python3 tools/false_pointers.py [--bin B] [--hidden] [--words]` (after `make resplit`)
lists every in-range `.word` per block: `sym` (splat resolved it to a symbol),
`raw` (left as hex), and with `--hidden` the aligned in-range words emitted as
`.short`/`.byte`. Jump-table entries (`.word .L…`) are skipped. After this change every
remaining in-range `.word` is a real pointer; the converted blocks show up only as
`hidden`.

## Converted: false pointers inside non-pointer data

| binary | symbol | range | data | false words | typed |
|---|---|---|---|---|---|
| ovl_0b | `g_btlfxCommonData` | 0x800F78B4-0x800F8CB4 | battle-effect common data: offset table + camera scripts, emitters, textures, sounds | 22 | `u8`, 0x1400 |
| ovl_0b | `D_800F61D4` | 0x800F61D4-0x800F6384 | status-icon sprite animation (a `g_btlStatusIcons[].sprite` target) | 2 | `u8`, 0x1B0 |
| ovl_0b | `D_800F6384` | 0x800F6384-0x800F650C | status-icon sprite animation | 2 | `u8`, 0x188 |
| ovl_0b | `D_800F65CC` | 0x800F65CC-0x800F670C | status-icon sprite animation | 7 | `u8`, 0x140 |
| ovl_0b | `D_800F670C` | 0x800F670C-0x800F684C | status-icon sprite animation | 7 | `u8`, 0x140 |
| ovl_0b | `D_800F684C` | 0x800F684C-0x800F6A28 | status-icon sprite animation | 1 | `u8`, 0x1DC |
| ovl_0b | `g_btlStatusCountdowns` | 0x800F6CA4-0x800F6CB4 | `{s32 status mask, s16 timer, u16}` x2; mask 0x80000000 | 1 | `u8`, 0x10 |
| main | `g_usableItemTable` | 0x8006B1F4-0x8006B374 | usable-item effect records; status mask 0x80000001 (was emitted as `.word D_80000000 + 0x1`) | 1 | `u8`, 0x180 |
| ovl_32 | `s_menuChocographFrameImage` | 0x801F7948-0x801F7AC0 | CLUT + pixels | 1 | `u8`, 0x178 |
| ovl_32 | `s_menuChocographPictureImages` | 0x801EF6FC-0x801F713C | 24 chocograph pictures (CLUT + pixels) | 8 | `u8`, 0x7A40 |
| ovl_36 | `s_menuCardImage2` | 0x801EEFB4-0x801F0274 | CLUT + pixels | 4 | `u8`, 0x12C0 |
| ovl_39 | `s_menuSaveImage` | 0x801EE4D4-0x801F2854 | image data for func_80028720 | 3 | `u8`, 0x4380 |
| ovl_3a | `s_menuNamePortraits` | 0x801EE0B4-0x801F3874 | portraits (CLUT + pixels) | 4 | `u8`, 0x57C0 |
| ovl_3b | `s_menuPartyImage` | 0x801EDFE8-0x801F37A8 | character portraits (CLUT + pixels), 0x57C0 like s_menuNamePortraits | 4 | `u8`, 0x57C0 |
| ovl_3c | `s_shopImage` | 0x801EDF3C-0x801F36FC | character portraits (CLUT + pixels), 0x57C0 like s_menuNamePortraits | 4 | `u8`, 0x57C0 |
| ovl_65 | `s_rewardsImage1` | 0x801EE19C-0x801EF260 | CLUT + pixels (level-up label) | 1 | `u8`, 0x10C4 |

The other seven status-icon animations (`D_800F5968`, `D_800F5B1C`, `D_800F5CD0`,
`D_800F5DB0`, `D_800F5F5C`, `D_800F6010`, `D_800F650C`) hold no in-range word but were
typed `u8` too, so the whole 0x800F5968-0x800F6A28 group is emitted the same way.

## Converted the other way: real pointers hidden in `.short` data

spimdisasm emitted these as `.short` pairs (the code reads halfwords of the structs), so
a relocation pass reading `.word`s would miss real pointers. Typed `u32`, they are
`.word`s resolved to symbols now.

| binary | symbol | range | pointers | typed |
|---|---|---|---|---|
| ovl_38 | `s_menuStatusPageCloseTweens` | 0x801F114C-0x801F11CC | 9 (`D_801F1144` x8, `menuStatus_onPagesClosed`) | `u32`, 0x80 |
| ovl_64 | `g_mbgMovieLists` | 0x8019DC5C-0x8019DC84 | 5 (`D_8019D400`, `D_8019D518`, `D_8019D75C`, `D_8019D93C`, `D_8019DACC`) | `u32`, 0x28 |

(`include/ovl_64.h` declares `g_mbgMovieLists[6]`, but the block holds 5 entries;
the 6th would read `g_mbgMaskBit`.)

## Not converted

Blocks with in-range `.short`/`.byte` values already (false, but no `.word`, so
harmless): main `D_800666E8`, `D_80067E88`, `g_itemTable` (39), `g_equipStatBonus`;
ovl_0b `g_btlPopupBounce`.

Real pointers left as raw hex (splat had no label at the target; a port must relocate
them like any other pointer, and the cross-binary ones point into whichever overlay is
loaded):

- main: `jtbl_8006E718`, `jtbl_8006E918` (jump tables into main code), `D_80012198`
  (akao_peekNextNote jump table in rodata), `g_label*`/`g_winLabelStrings`,
  `D_80068274`, `D_800682F4`, `D_800103F0`, `D_8006788C`, `D_80068530` (pointers into
  main's .rodata strings and tables), `g_menuScreensField`, `g_menuScreensWorld`,
  `g_menuTypes` (overlay functions and data), `g_modeOverlayAddrs` (overlay load
  address 0x801EDF00).
- ovl_0b `g_btlfxModuleExports`: `SquareRoot0` 0x80030874, `ClearImage` 0x80013228
  (PsyQ functions in main, named only in `symbols.psyq.us.txt`).
- ovl_0a/ovl_0c `s_menuMainEquipLabels`, ovl_0c `g_worldImageNamesDisc1/4`, ovl_38
  `s_menuStatusHelpArgs`, ovl_64 `D_8019D400`, `D_8019D518`, `D_8019D75C`,
  `D_8019D93C`, `D_8019DACC` (movie path pointers into the middle of the
  `s_mbgTestMoviePaths` string block): string pointers.

Effect modules (fx_XXX): every in-range word is a real pointer (`*_header`,
`*_headerPtr`, the named particle/spawn tables, jump tables). Their data chunks are
not part of the code binaries.

Data inside `.text` (`textbin` subsegments, `docs/text-data.md`) is emitted as
`.incbin`, not `.word`, so it is out of reach of a `.word` pass either way.

## Ambiguous

- main `D_8006671C` (0x8006671C-0x80066730, PsyQ libgpu data used by `ClearOTag`/
  `ClearOTagR`): the word at 0x80066720 is 0x80000000, emitted as `.word D_80000000`.
  Library data with an unknown layout; 0x80000000 is more likely a flag/terminator
  than a pointer to the bottom of RAM, but left as is.

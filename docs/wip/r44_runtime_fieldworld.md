# R44: runtime questions from the layout / tuning pass over ovl_0a (field) and ovl_0c (world map)

Round R44 named screen layout and tuning literals: the main menu's layout (`MENU_MAIN_*`, menu_main.c, identical in
ovl_0a and ovl_0c), the field / world screen centre (`FLD_SCREEN_*`, `WM_SCREEN_*`), the field work RAM layout
(`FLD_WORK_*`), camera follow and player control tuning (`WM_CAM_*`, `WM_WALK_*`, `WM_STEER_*`, `WM_FLY_*`,
`WM_AUTOPILOT_*`), effect scales and SE fades (`WM_FX_SCALE_*`, `WM_SE_FADE_*`), shadows and tilt settling
(`WM_SHADOW_*`, `WM_ROLL_*`, `WM_VEHICLE_*`), and the polygon-cache-off boxes (`WM_NOCACHE_*`, named for the navi
place at their centre: world text bank 0, read from disc 1 FF9.IMG at 0x6DC2xx, entry place + 1).

The rows below are what the code alone doesn't settle. Addresses: ovl_0a `menuMain_drawMember` 0x800C883C,
`fld_togglePause` 0x800AF740; ovl_0c `wm_movePlayer` 0x800B0378, `g_worldUsePolyCache` 0x800D30C6,
`wm_drawActorShadow` 0x800AF8C0, `g_worldVehicleActor` 0x800CE3D0, `g_worldPlayer` 0x800CE3DC (x at +20, z at +28).

| # | symbol | binary / file | current name and what the code shows | what to observe | mode / state |
|---|---|---|---|---|---|
| 1 | `MENU_MAIN_PORTRAIT_X_BACK` 8 / `_FRONT` 0x16 | ovl_0a / ovl_0c `menuMain_drawMember` (menu_main.c) | the portrait sits 0xE further right when `Character.info.b.battleRow` is clear; which of front / back row is that? | `menu_main` state: Order a character with themselves (toggles battleRow), screenshot before / after; or poke the battleRow bit of party[0] and screenshot. Then name the two by the row the game's Order help describes ("front row or back row") | `menu_main` |
| 2 | `MENU_MAIN_STATUS_BOX_*` (panel x + 0x82, 0x72 x 0x12), `MENU_MAIN_SHADE_*` (0x2E x 0x30 translucent tile at x + 8) | menu_main.c `menuMain_drawParty`, `menuMain_drawPortraitShade` | a line-style box along each party panel's top right, around the status icons; a translucent tile behind the portrait | screenshot `menu_main` with a party member who has a status (or poke `Character.status`), zoom on one panel: confirm the box frames the status icons / magic stones row and the tile is the portrait backdrop | `menu_main` |
| 3 | `UI_SPRITE_PAUSE` 0xEB (main-exe candidate in both overlay headers) | ovl_0a `fld_togglePause`, ovl_0c `wm_togglePause` | the sprite centred on the dimmed screen while paused | pause in the field (and on the world map), screenshot: what the sprite shows (e.g. the word "PAUSE") | any field save state; `wm_enter.py` |
| 4 | `FLD_PAUSE_LIGHT_FADE` 200 | ovl_0a `fld_togglePause` | with a fade colour >= 200 in every channel the pause sprite switches to subtractive blending | pause during a white fade-in (e.g. right after `fade_inAdditiveDefault`, or poke the fade colour to 0xFFFFFF mid-fade) and screenshot: is the pause sprite drawn dark on white? | a field entry with a white fade |
| 5 | `WM_NOCACHE_*` (10 boxes: Conde Petie, Ice Cavern x2, Chocobo's Lagoon, Conde Petie Mountain Path, Gizamaluke's Grotto, Desert Palace, Ipsen's Castle, Daguerreo, and `WM_NOCACHE_CELL_16_14` at (264183, -245187)) | ovl_0c `wm_movePlayer` (w_movement.c) | inside them `g_worldUsePolyCache` = 0, so `wm_getGroundHeight` ignores `WmPolyCache` | (a) teleport the walker to (264183, -245187) and screenshot: what is there (rename `WM_NOCACHE_CELL_16_14` after it); (b) at one or two boxes (Conde Petie, Daguerreo) patch the clear out (nop the `sb zero` stores in `wm_movePlayer`) and walk around the landmark: does the walker fall through / snap onto the wrong level (bridge, overhang, stacked walkmesh)? That says why the cache is off (comment) | `cpass_wm_hadris` / `wm_enter.py`, disc 3 for Daguerreo and Ipsen's Castle |
| 6 | `WM_SHADOW_PLAIN_*` (fixed 0x302 x 0x294 shadow, strength 0x7D0 / 0xAF0 in fog, OT bias 0x46) | ovl_0c `wm_drawActorShadow` | the shadow `WM_ACTOR_NO_MODEL` (type 14) gets instead of a model's | `actors.lua` / trace `wm_drawActorShadow` (0x800AF8C0) printing actorType: which world object has type 14, and screenshot its shadow (is it the airship's shadow on the ground, a cursor?) | `cpass_wm_airship9` |
| 7 | `WM_VEHICLE_REST_ROLL` 14 | ovl_0c `wm_updateActors` | a parked / non-banking vehicle's rotZ settles to 14, not 0 | poke `g_worldVehicleActor->rotZ` (+0x24) to 0 and 14 with the vehicle parked, screenshot both: is 14 its resting tilt (e.g. the airship's slight bank) | `cpass_wm_airship9` (parked airship) |

Not asked: the camera, control and effect tuning names say what the code does with each value (dead zones, step
divisors, speeds); they don't depend on game content.

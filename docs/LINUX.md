# Linux (Steam Proton): experimental

Spidy is built for Windows. On Linux it runs inside the game's Proton session, started by
`tools/spidy-linux.sh` instead of Spidy Launcher.exe. Tested once: Spidy 0.3.2, game 4.630.0.0,
Proton Experimental, SteamVR on Linux with a Quest 3 over Steam Link, CachyOS, RX 6800.
Contributed by Sebi (sjrl2000). In the release zip since 0.3.6; that build has not run under Proton
yet (the tested session used 0.3.2 with the lock change below patched in by hand).
WiVRn: tested once with 0.3.6, WiVRn 26.9 (Flatpak) with a Quest 3, Proton Experimental. Steam Link
stayed the more reliable of the two.

## Setup

1. Extract the Spidy zip anywhere, for example `~/Spidy`.
2. In Steam: right-click the game > Properties > General > Launch Options:

       STEAM_COMPAT_LAUNCHER_SERVICE=proton %command%

   It lets programs run inside the game's Proton session; the game itself is unchanged.
3. WiVRn only: let the game's container load WiVRn too. With WiVRn's Flatpak build (as WiVRn
   recommends, tested), the launch options become

       PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES=1 PRESSURE_VESSEL_FILESYSTEMS_RW=/var/lib/flatpak/app/io.github.wivrn.wivrn STEAM_COMPAT_LAUNCHER_SERVICE=proton %command%

   A WiVRn installed from your distribution's packages does not need `PRESSURE_VESSEL_FILESYSTEMS_RW`.
   Without these the container does not see WiVRn.

## Play

1. Start SteamVR and connect the headset (SteamVR home showing), or start WiVRn and connect the
   headset in the WiVRn app. Do this before the game: Proton sets up its OpenXR bridge only when the
   game starts.
2. Start the game from Steam and wait at the main menu.
3. Run `bash tools/spidy-linux.sh` from the Spidy folder (a zip made on Windows keeps no run
   permission for the script).
4. Click the game window once, load your save with mouse or keyboard, and VR takes over in gameplay.

## When it does not work

- `No headset found (wineopenxr64: OpenXR instance unavailable: -51)`: Proton found no VR runtime
  when the game started. Quit the game, make sure SteamVR or WiVRn has the headset, start the game
  again, then the script. With WiVRn, check that `~/.config/openxr/1/active_runtime.json` names
  WiVRn and that `~/.config/openvr/openvrpaths.vrpath` lists WiVRn's xrizer (WiVRn writes both
  while it runs; after a WiVRn update a stale xrizer path is fixed by restarting WiVRn).
- To see what Proton found, add `PROTON_LOG=1` to the launch options and start the game: in
  `~/steam-1817070.log`, a working setup shows xrizer and the headset (for WiVRn, `WiVRn HMD`) next to
  `wineopenxr test instance`. Remove `PROTON_LOG=1` afterwards: it slows the game.
- The script warns when WiVRn is the active runtime but the game was started without
  `PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES=1`.
- WiVRn: stepping outside the Quest's boundary stops the game until you are back inside. The headset
  shows passthrough and the session loses focus, and Spidy reads no input without focus. Make the
  boundary larger, use a stationary boundary, or (developer mode, over USB)
  `adb shell setprop debug.oculus.guardian_pause 0` until the headset restarts; that also drops the
  boundary's warning, so clear the room.

## Why not Spidy Launcher.exe

- Run on its own (for example with protontricks-launch), the launcher is outside the game's
  session: Proton's OpenXR bridge (wineopenxr) finds no headset, and the game cannot be reached.
- START VR starts the game through `steam.exe -applaunch`, which does not work from inside Proton.
  The script attaches to the game Steam started instead.

## Known limits

- Attaching to a running game, Spidy cannot enlarge its render memory (the launcher does that in
  the first seconds): heavy scenes may drop eye frames.
- Menus do not read the VR controllers (SteamVR's virtual gamepads); use mouse or keyboard there,
  or try Properties > Controller > Disable Steam Input.
- Quitting to the main menu may close the game (Steam ticket guard 9803 under Proton).
- Spidy sets the game's settings for VR only in a game it starts itself. Here, set them in the game
  before you play: frame generation and vertical sync off, Ray-Traced Reflections off, Aspect Ratio
  at Auto.

## What was changed for Proton

- `render_probe.cpp`: Proton's DXGI (DXVK/vkd3d-proton) has no composition swapchains, so finding
  the game's queue failed with 2104. It now falls back to a swapchain on a hidden window.
- `native_eye_gpu.cpp`: vkd3d-proton uses one function for every command list, so the ResourceBarrier,
  Reset and EndQuery hooks also see Spidy's own calls and re-entered while `recording` was held. The
  game closed with resource_deadlock_would_occur. `recording` is now a recursive mutex.

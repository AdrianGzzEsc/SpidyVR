#!/usr/bin/env bash
# Start Spidy VR inside a running Spider-Man Remastered Proton session.
# Needs: Steam launch option  STEAM_COMPAT_LAUNCHER_SERVICE=proton %command%
#   (WiVRn: PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES=1 STEAM_COMPAT_LAUNCHER_SERVICE=proton %command%)
# Order: start SteamVR or WiVRn, connect the headset, start the game from Steam, wait at the main menu, run this.
set -euo pipefail

# Default: the Spidy folder this script is in (tools/ sits next to python/ and build/).
SPIDY_DIR="${SPIDY_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"
BUS=com.steampowered.App1817070

# steam-runtime-launch-client from any Steam library's Linux runtime.
CLIENT="${SPIDY_LAUNCH_CLIENT:-}"
if [[ -z "$CLIENT" ]]; then
    libraries=("$HOME/.local/share/Steam/steamapps")
    vdf="$HOME/.local/share/Steam/steamapps/libraryfolders.vdf"
    [[ -f "$vdf" ]] && while IFS= read -r path; do libraries+=("$path/steamapps"); done \
        < <(sed -n 's/^\s*"path"\s*"\(.*\)"/\1/p' "$vdf")
    for lib in "${libraries[@]}"; do
        for runtime in SteamLinuxRuntime_4 SteamLinuxRuntime_sniper SteamLinuxRuntime_soldier; do
            candidate="$lib/common/$runtime/pressure-vessel/bin/steam-runtime-launch-client"
            if [[ -x "$candidate" ]]; then CLIENT="$candidate"; break 2; fi
        done
    done
fi
[[ -x "$CLIENT" ]] || { echo "steam-runtime-launch-client not found (set SPIDY_LAUNCH_CLIENT)." >&2; exit 1; }
[[ -f "$SPIDY_DIR/tools/run_game_vr.py" ]] || { echo "Spidy not found in $SPIDY_DIR (set SPIDY_DIR)." >&2; exit 1; }

# Proton's OpenXR bridge uses the runtime the host's active_runtime.json names.
ACTIVE="${XDG_CONFIG_HOME:-$HOME/.config}/openxr/1/active_runtime.json"
if grep -qi wivrn "$ACTIVE" 2>/dev/null; then
    pgrep -x wivrn-server >/dev/null || echo "Warning: WiVRn does not seem to be running. Start it and connect the headset first." >&2
    # pressure-vessel hides the host's runtime from the game unless asked to import it.
    game=$(pgrep -f 'Spider-Man\.exe' | head -n1 || true)
    if [[ -n "$game" ]] && ! tr '\0' '\n' < "/proc/$game/environ" 2>/dev/null | grep -q '^PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES=1$'; then
        echo "Warning: the game was not started with PRESSURE_VESSEL_IMPORT_OPENXR_1_RUNTIMES=1 in its launch options; it may not find WiVRn." >&2
    fi
else
    pgrep -x vrserver >/dev/null || echo "Warning: SteamVR does not seem to be running. Start it and connect the headset first." >&2
fi

if ! "$CLIENT" --list 2>/dev/null | grep -qx -- "--bus-name=$BUS"; then
    cat >&2 <<MSG
The game's Proton session was not found. Check that:
  1. Spider-Man's Steam launch options are:  STEAM_COMPAT_LAUNCHER_SERVICE=proton %command%
  2. the game was started from Steam and is at the main menu.
MSG
    exit 1
fi

echo "Starting Spidy VR. Click the game window once, then load your save (mouse or keyboard)."
cd "$SPIDY_DIR"
exec "$CLIENT" --bus-name="$BUS" --directory="$SPIDY_DIR" --unset-env=XR_RUNTIME_JSON \
    -- wine python/python.exe -u tools/run_game_vr.py --xr-runtime auto "$@" \
    2> >(grep --line-buffered -v -E 'Fontconfig|fixme:' >&2)

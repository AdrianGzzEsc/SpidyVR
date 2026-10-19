# Spidy Wall Climb Preview 7 — custom build

This modified build of Ilya Mezerowsky's Spidy VR mod is based on upstream
0.3.8, commit aed5c0c5f4584d7bc231b90fd1200b3ea85728c9. Keep the whole
extracted preview folder together.

## Start

1. Close the game and any other Spidy session.
2. Open **Spidy Wall Climb Preview 7.exe**, connect your headset, and start VR.
3. Enable **Settings → SPIDY VR → EXPERIMENTAL → HAND WALL CLIMBING**.
   Enable it again after restarting VR; the switch lasts for the session.
4. **WALL CAMERA DISTANCE** sets the close climbing view to 10, 15, 20, 25,
   30, 40, 50, 60, 75 or 100 cm from the facade. Default: 30 cm. Resume play
   to apply a change.

## Grip climbing

Touch climbing and its setting have been removed. Putting a hand against
or inside the wall without pressing a grip does not attach it.

On a wall, with the left stick at rest, the view stays close and lowered.
Reach to within about 18 cm of the wall, release the grip if already held,
then squeeze the physical grip. This catches your motion as in the earlier
previews. Palms inside a solid facade still register.

Pull a held hand down to move up, sideways to traverse, or up to descend.
Release the grip to let that palm go, reach again, and squeeze again.
Movement follows the opposite of your hand motion in the current view;
looking elsewhere alone does not steer a pull. Two hands share movement,
with hand pulls limited to 3 m/s.

Attached hands stay open and give one short vibration on each new contact.
The existing vibration setting controls its strength. Both physical grips
work regardless of the WEB BUTTON mapping. Existing webs retain their
controls. A free hand away from the wall can shoot a web. Press **A** to
jump away. Tracking loss releases that hand and requires a fresh grip press.

## Camera roll and wall running

Hold both physical grips and roll the controllers left or right to turn the
climbing view along the wall. Each two-grip hold captures a fresh neutral
wrist pose. Past roughly 10 degrees of tilt, the view turns, up to 90 degrees
per second at 45 degrees. Return toward neutral to stop, or release either
grip to lock the angle. Sideways and upside-down hand pulls still work.

Push the left stick to wall run. The starting view and joystick direction
choose the camera's heading once, when the run begins. The earlier
wall-as-floor camera transition is restored: its original up-to-wall turn,
speed and placement calculation are used again. The extra camera steering
introduced in Preview 6 has been removed. Changing stick direction during
a held run changes movement without automatically turning the camera again.
Physical head movement and the usual turning controls remain available.

Starting a run resets your selected roll. Release the stick to return to a
fresh, upright close climbing view on the same wall. It does not restore
the previous roll angle. Release and squeeze a grip again to resume hand
climbing. Small stick drift remains inside the dead zone.

Jumping, leaving the wall, disabling hand climbing or recentering resets
the roll. Tracking/focus interruptions suspend rotation and require a fresh
neutral capture before it resumes.

## Corners

The smooth camera rotation onto a building's next side is retained.
After an outside corner finds solid facade beside the body, it now carries
you approximately **85 cm farther along the new wall** before returning
control fully to your hand pulls. This adds about 40 cm over Preview 6. The last segment moves more slowly,
so the body continues forward after the camera has rotated onto the next
wall rather than stopping at the corner.

The extra move is bounded by the existing corner timeout and goes through
the normal movement/collision system. A jump cancels the wall hold.
Missing facades, large gaps and roof edges keep their existing behavior.
Irregular building geometry and mission areas still need gameplay testing.

## Body rotation: gesture follows, gaze does not

The hips and legs once again rotate with the two-grip controller-roll
gesture. The body, hands and climbing movement share that chosen rotation,
including sideways and upside down.

The separate gaze-driven wall-climbing body frame remains removed.
Looking down does not turn the body into the custom downward climbing pose
or move the lower body behind the new gaze direction. The earlier body and
leg implementation is retained, with the gesture tilt applied to the whole
body about the eyes as it was before Preview 6.

No new original-game climbing animation cycle is integrated by this update.

## Validation

Windows Release build: passed. Native core: **363/363** tests. Launcher:
**23/23** checks. Python protocols: **131/131** tests across six suites.
The packaged runtime imports, source whitespace, and module/source copies
are also checked during packaging.

Tests cover grip-only attachment and release, no button-free contact or
vibration, deep palms, tracked-hand corner traversal, the extra corner
advance and stop, retained smooth corner rotation, entry-heading alignment from upright/sideways/upside-down views, the original
wall-turn timing, no continuous joystick camera steering, diagonal travel,
physical head tracking, roll reset, body and leg rotation from the actual
controller gesture, and the absence of gaze-driven body rotation.

**Preview 7 has not yet been played in the game or tested in a headset.**
Earlier previews were played by the user. Synthetic corner tests do not
establish that every game building has suitable collision geometry.

## Separate installation and source

The original installed Spidy and previous previews are unchanged. This
launcher uses preferences under %APPDATA%/SpidyWallClimbPreview and starts
without automatic update checks. Official updates replace custom modules;
use the normal installation for those updates.

Original project: https://github.com/IlyaMez/SpidyVR

Copyright (C) 2026 Ilya Mezerowsky. Distributed under GPL-3.0 with the
original license and third-party notices. The matching source archive
contains these modifications and their tests. Build using
tools/bootstrap.ps1 -Observer, then tools/build.ps1 -Observer,
as described in CONTRIBUTING.md.

The swing command remains version 7 (248 bytes). Settings return to version
6 (40 bytes), with the hand-climbing switch and no touch-climbing field.
Version 3–5 settings remain accepted. The body command returns to version
4 (192 bytes); earlier body commands are supported. The separate sticky-palm
contact report stays version 1 (48 bytes). Existing swing telemetry layouts
are unchanged.

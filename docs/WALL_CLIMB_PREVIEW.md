# Spidy Wall Climb Finished 2 — ground-camera fix

This modified build of Ilya Mezerowsky's Spidy VR mod is based on upstream
0.4.0, commit c747c184dd97acaddf0db47f1a32f30cebe0d974, merged with the
working Preview 7 changes saved at e0fcb4c. Keep the whole
extracted preview folder together.

## Finished build

Moving the left stick on ordinary ground or a walkable slope no longer
starts the wall-run camera alignment. The transition requires a wall or
ceiling hold. Normal ground movement keeps the camera facing where the
headset and the usual turning controls point, including after a wall landing.

This is the finished build requested after Preview 12: the experimental vent
hand crawling, its input/feedback path and its special camera changes have
been removed. Vents use the behavior from before that experiment.

The original grip rules from before the free-hand-web change are restored.
While both grips are held on a wall, they are reserved for the camera-roll
gesture and do not fire webs. Release the grips, then press again to shoot.
The deliberate-roll-only reservation added by the free-hand-web change is
removed as well.

Outdoor grip climbing, open palms and vibration, camera/body roll, extended
corner carry, wall running and its backward-input fix are retained.

## Retained distance and movement controls

The default WALL CAMERA DISTANCE is now **40 cm**. You can still choose any
of the existing distance options in the settings menu.

Pulling the left stick backward moves you backward without turning the
camera around. Briefly crossing the stick centre (under 100 ms) keeps the
running view steady and produces no movement while inside the dead zone.
A sustained release still returns to the close climbing view. Starting a
new run backward also keeps the forward-facing view.

## Head steering

Wall running follows left/right head turns again. While holding the stick
forward, look left or right to steer along the wall. Up/down head tilt does
not steer or reverse movement, including while you are turning sideways.
This works upright, sideways and upside down after the controller-roll gesture.

Preview 8 could reverse movement when downward head tilt crossed a direction
threshold. Preview 9 prevented that by also locking out left/right head
steering. Preview 10 restores that steering while keeping the pitch fix.

Near a straight-up/down head pose, the last reliable horizontal heading is
kept until it can be measured again. Passing that vertical pitch does not
count as a 180-degree head turn. Ordinary continuous left/right turns can
still steer through a full turn.

Joystick forward/back/strafe commands, deliberate turning controls, corner
transport, the earlier wall-running camera transition, gesture-driven body
roll, and all merged Spidy 0.4.0 features are retained.

## What is merged

This build combines your updated official Spidy 0.4.0 release with Preview 7.
It keeps the update's arcade/physical web styles, phone and music player,
Spider-Sense/counters, rendering and body fixes, the reorganized SPIDY VR
sections, and head-collision protection.

Preview 7's grip climbing, open palms and vibration, adjustable distance,
two-grip view/body roll, one-time wall-run entry heading and extended
corner carry are retained. Touch climbing and the custom gaze-driven body
frame remain removed.

## Start

1. Close the game and any other Spidy session.
2. Open **Spidy Wall Climb Finished 2.exe**, connect your headset, and start VR.
3. Enable **Settings → SPIDY VR → EXPERIMENTAL → HAND WALL CLIMBING**.
   Enable it again after restarting VR; the switch lasts for the session.
4. **WALL CAMERA DISTANCE** sets the close climbing view to 10, 15, 20, 25,
   30, 40, 50, 60, 75 or 100 cm from the facade. Default: 40 cm. Resume play
   to apply a change.

Spidy 0.4.0's head-collision protection remains active. It can keep the
camera at least 20 cm from solid geometry even if 10 or 15 cm is selected,
and can push it farther out at obstructions. Eyes, hands and body receive
the same collision adjustment.

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
controls. Both held grips reserve web actions for the roll gesture, as in
the original grip controls. Release the grips before shooting again. Press **A** to
jump away. Tracking loss releases that hand and requires a fresh grip press.

## Camera roll and wall running

Hold both physical grips and roll the controllers left or right to turn the
climbing view along the wall. Each two-grip hold captures a fresh neutral
wrist pose. Past roughly 10 degrees of tilt, the view turns, up to 90 degrees
per second at 45 degrees. Return toward neutral to stop, or release either
grip to lock the angle. Sideways and upside-down hand pulls still work.

Push the left stick to wall run. The starting view and joystick direction
choose the camera's heading once, when a forward or sideways run begins.
A backward start keeps the view facing forward. The running axes
follow left/right head turns while ignoring up/down tilt. The earlier
wall-as-floor camera transition is restored: its original up-to-wall turn,
speed and placement calculation are used again. The extra camera steering
introduced in Preview 6 has been removed. Changing stick direction during
a held run changes movement without automatically turning the camera again.
Physical head movement and the usual turning controls remain available.

Starting a run resets your selected roll. Release the stick to return to a
fresh, upright close climbing view on the same wall after 100 ms at rest.
This short delay avoids a camera transition when reversing the stick. It does not restore
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
or move the lower body behind the new gaze direction. The updated 0.4.0 body and leg implementation is retained, with the gesture
tilt applied to the whole body about the eyes as in Preview 7.

No new original-game climbing animation cycle is integrated by this update.

## Validation

Windows Release build: passed. Native core: **397/397** tests. Launcher:
**24/24** checks. Python protocols: **137/137** tests across six suites.
The packaged runtime imports, source whitespace, and module/source copies
are also checked during packaging.

Tests cover grip-only attachment and release, no button-free contact or
vibration, deep palms, tracked-hand corner traversal, the extra corner
advance and stop, retained smooth corner rotation, entry-heading alignment from upright/sideways/upside-down views, the original
wall-turn timing, no continuous joystick camera steering, diagonal travel,
physical head tracking, roll reset, body and leg rotation from the actual
controller gesture, and the absence of gaze-driven body rotation. Additional
merge tests cover independent phone/arcade/climb settings, climbing in both
web styles and while changing styles, and head protection during rolled climbs.
Upstream phone, music, sense/counter and rendering tests are retained.
New regression coverage reproduces Preview 8's backward movement and verifies
left/right yaw steering with pitch independence upright, sideways and upside
down, reverse/strafe stick commands, corner transport, deliberate turns,
continuous full head turns and pitches through vertical. Preview 11 also
checks forward-to-backward reversals with zero, one and six centre frames
in upright, sideways and upside-down views, backward and diagonal-backward
starts after a full stop, and the return to climbing after a sustained release.

Ground-camera regressions check every frame through left/right, forward,
backward, diagonal and neutral stick changes on ground, walkable slopes and
floor crawl contacts. A wall-run-to-ground test checks that walking and
strafing after landing preserve the view while following the moving player.

**This finished build has not yet been played in the game or tested in a headset.**
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

The swing command remains custom version 7 (248 bytes), including the
official update's native-action requests. Swing settings are version 8
(48 bytes): the official phone suppression and arcade style fields keep
their 0.4.0 offsets, with handClimb appended at byte 44. Official version
3–7 settings remain accepted with their original meanings. The body command
remains version 4 (192 bytes), and the separate sticky-palm contact report
remains version 1 (48 bytes). Keep this preview's modules together; old custom
preview settings packets are not interchangeable with the updated modules.

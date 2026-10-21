# Spidy Vent Crawl Preview

This modified build of Ilya Mezerowsky's Spidy VR mod is based on upstream
0.4.0, commit c747c184dd97acaddf0db47f1a32f30cebe0d974, merged with the
working Preview 7 changes saved at e0fcb4c. Keep the whole
extracted preview folder together.

## Vent crawling

This local preview builds on the corner improvements at 0ebdab2. Enable
HAND WALL CLIMBING, enter a vent using the game's usual interaction, then
reach toward its floor or a wall and squeeze a physical grip. Pull the hand
back toward you to crawl forward along the floor; release, reach and grip
again to keep going. On a shaft wall, pull down to climb up. Touch alone
does not attach. A grip already held before contact can catch when the palm
reaches the surface. Palms inside the surface still register.

Vents retain a low climbing camera on floors, walls and ceilings. The left
stick provides ordinary native crawling and never enters wall running there.
On the floor, the default camera height is 40 cm. WALL CAMERA DISTANCE also
sets vent clearance, limited to 25–70 cm; actual collision/headroom can
require a different position. Keep the standing calibration: physically
crouching is optional. Eyes, hands and body share the camera adjustment.

Hands stay open and give the same short contact vibration. The two-grip
wrist-roll gesture is retained, including rolling on a vent floor; release
either grip to lock the angle. Its reserved hands do not pull while rolling.
Holding the left stick overrides hand movement without switching the view.
The game retains crawl collision, floor-to-shaft transitions and scripted
interactions. Pulling a wall from the floor approaches the bend before the
game turns onto the shaft. A game-authored interaction or exit still needs
the usual button; hand movement does not bypass mission gates or grates.

Enclosed native crawl activates the mode; a scripted floor with no usable
collision rays must remain in native crawl for 0.4 seconds. Brief outdoor
wall-top crawl does not enter vent mode. Normal ground movement and outdoor
wall running keep their existing behavior. Releasing a grip stops its pull;
tracking/focus loss and leaving supported crawl clear the hand holds.

This replaces the earlier removed vent experiment with floor-support
fallback, held-grip contact retries, native-stick dead-zone compensation,
and a dedicated low climbing view. A session log entry, "Vent hand crawling
active", records when detection engages. This build has not been tested in
a headset yet; try a flat duct, a floor-to-shaft turn, then its normal exit.

## Retained corner improvements

Climbing camera safety rays now start from the native collision capsule's
centre. The old point half a metre from the facade could itself sit inside
a projecting pillar, causing its front to be ignored. Close climbing also
checks 75 cm farther ahead and to the sides/up, using the same ray count,
so upcoming trim is found before the view swings into it.

Close climbing resolves opposing wall planes for more passes at acute
inside corners. This prevents clearing one wall from leaving the eyes
inside the other. Ground and wall-running clearance keep their existing
short ray distances and ordinary constraint passes.

The shared wall detector already follows clean non-right-angle corners.
Regression worlds now cover 15 to 150 degree facade turns in both
directions, bevels and faceted rounded corners, plus projecting trim during
a complete corner turn with one frame of delayed world-ray feedback.

Try the same buildings where the view collided with a corner. Headset play
is still needed: synthetic shapes do not establish that every game facade
or ornament has suitable collision geometry.

## Retained finished-build controls

Moving the left stick on ordinary ground or a walkable slope no longer
starts the wall-run camera alignment. The transition requires a wall or
ceiling hold. Normal ground movement keeps the camera facing where the
headset and the usual turning controls point, including after a wall landing.

This preview adds the vent behavior above to Finished 2 and the corner
preview. It is a separate local build; the previous finished and corner
packages remain available.

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
2. Open **Spidy Vent Crawl Preview.exe**, connect your headset, and start VR.
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

Windows Release build: passed. Native core: **413/413** tests. Launcher:
**24/24** checks. Python protocols: **138/138** tests across six suites.
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

Vent checks cover low floor/wall/ceiling placement, all left-stick directions
without a camera transition, held-grip contact retries, rayless/deep support
contacts, averaged pulls and support-plane constraints, floor-to-shaft pulls,
roll/release, tracking loss, native pad mapping of slow pulls, stale feedback,
entry/exit, and restoration of outdoor wall running.

**This vent preview has not yet been played in the game or tested in a headset.**
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
is version 2 (72 bytes), adding expiring native crawl intent. The diagnostic
reader accepts both contact-report versions. Keep this preview's modules together; old custom
preview settings packets are not interchangeable with the updated modules.

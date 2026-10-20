# Hand wall climbing

An optional grip-driven wall-climbing mode, off by default. Enable
**Settings > SPIDY VR > EXPERIMENTAL > HAND WALL CLIMBING**, then resume play.
The setting lasts for the VR session.

## Controls

- On a wall, leave the left stick centred. The climbing view stays close to
  the facade and lowers from standing height.
- Reach within about 18 cm of solid wall, release an already-held grip,
  then squeeze the physical grip. The hand catches the wall and arrests
  movement. Palms already inside the facade also register.
- Pull the hand down to move up, sideways to traverse, or up to descend.
  Release, reach and squeeze again for another stroke. Two hands share
  movement; hand pulling is limited to 3 m/s.
- Attached hands stay open and produce one short vibration on each new
  contact. The existing vibration-strength setting applies.
- Both physical grips held together reserve the hands for camera roll.
  Roll the controllers to rotate the climbing view and body along the
  wall, including sideways and upside down. The initial wrist poses are
  neutral; rotation starts past about 10 degrees of roll. Releasing either
  grip locks the view angle. Release the grips before shooting webs again.
- Push the left stick to use the existing wall-as-floor running view.
  The starting view chooses the running direction. Left/right head turns
  steer; looking up or down cannot reverse movement. Backward stick input
  moves backward without turning the camera around.
- A brief stick-centre crossing while reversing keeps the running view;
  a sustained release returns to a fresh upright climbing view. Ground
  movement and walkable slopes cannot start the wall-run camera alignment.
- Press A to jump away. Tracking loss releases a hand and requires a
  fresh grip press before that hand attaches again.

The grip controls work with either WEB BUTTON mapping. There is no
button-free wall attachment and no gaze-directed hand-pull movement.

## Clearance and corners

**WALL CAMERA DISTANCE** offers 10, 15, 20, 25, 30, 40, 50, 60, 75 and
100 cm. Its default is 40 cm. Existing head-collision protection remains
active and can keep the camera farther out, including its 20 cm minimum
clearance at solid geometry.

The climbing camera turns smoothly onto the next building face. Outside
corners carry the body farther along the new facade after the turn, bounded
by the corner timeout and normal collision handling. Missing walls and
irregular collision geometry still need gameplay testing.

This feature does not add hand locomotion inside vents. Vent interactions
and native crawling retain their existing paths.

## Integration and validation

The feature is gated by `handClimb`, false by default, and retains the
official launcher, update checks, preferences location and release version.
There are no packaged executables or preview-specific launcher changes in
this contribution.

Swing input is version 7 (248 bytes), settings version 8 (48 bytes), and
sticky-palm feedback version 1 (48 bytes). The new settings field is
appended after the upstream phone and web-style fields. Earlier official
packet versions remain accepted. Build and deploy matching modules together.

Windows Release build and automated checks: 397 native core tests, 24
launcher checks and 137 Python protocol tests across six suites.

The contributor tested earlier iterations on Quest 3 through Virtual
Desktop. The cleaned contribution branch has automated validation; it has
not been separately played in a headset. The work was developed with Codex
assistance. Real buildings, mission areas and interaction boundaries need
maintainer review and further play testing.

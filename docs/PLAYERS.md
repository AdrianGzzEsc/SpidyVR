SPIDY - VR for Marvel's Spider-Man Remastered
by Ilya Mezerowsky

Support Spidy: https://ko-fi.com/ilyamezerowsky
Spidy is free. It is built with a lot of AI help, and those tokens aren't free.
If you enjoy it, a tip on Ko-fi keeps the updates coming.


WHAT YOU NEED
-------------
- Marvel's Spider-Man Remastered on Steam. Spidy works with the Steam version
  only; the Epic Games Store version is a different build.
- A PC VR headset and its OpenXR runtime. Spidy has been played on a Quest 3
  through Virtual Desktop (https://www.vrdesktop.net/) and through SteamVR
  (Steam Link). Other SteamVR headsets and Meta Quest Link should work too.
  The launcher finds the runtime your headset is connected to.
- About 19 GB of memory Windows can give programs (RAM plus page file).


INSTALL
-------
1. Extract this whole zip to a folder you can write to, for example
   Documents\Spidy. Do not run it from inside the zip.
2. Start "Spidy Launcher.exe". Windows may say it protected your PC, because
   Spidy is not signed by a company: choose "More info", then "Run anyway".
3. The launcher finds the game and your VR runtime and checks this PC. Fix
   anything marked red. It can install Microsoft's Visual C++ runtime for you,
   and "Add shortcuts" puts Spidy VR on your desktop and in the Start menu.

Spidy changes nothing in the game's folder. Some antivirus programs dislike
tools that attach to a running game; if files go missing after extracting,
allow the Spidy folder and extract it again.


UPDATES
-------
When it starts, the launcher asks GitHub whether a newer Spidy is out. If one
is, a blue bar above the Play tab offers it. "Update now" downloads it, checks
it, puts its files in this folder and starts the new launcher, all in a few
seconds. Your settings and the reports folder stay. Stop VR and close the
game first. "Later" asks again at the next start. About > UPDATES checks
again on request, and its switch turns the check at start off.
The folder keeps the name it was extracted with (Spidy-0.2.7, say) through
updates; About shows the version inside.


PLAY
----
1. Close the game if it is running.
2. Put on the headset and connect it to the PC (Quest 3: open Virtual Desktop
   and connect, or Steam Link; SteamVR headsets: start SteamVR).
3. Press START VR. Steam starts the game. Its intro and menus show on a screen
   in the headset; pick your save with the VR controllers. VR takes over as
   soon as you play.
4. The first time, a panel asks you to calibrate your body: stand tall, look
   ahead, stretch both arms straight out to the sides (a T-pose) and hold
   both triggers until the bar fills, about a second and a half. Spider-Man
   then has your height and arm length, so his hands sit on your
   controllers. B skips it. The launcher remembers it; "Redo" under its
   options, or CALIBRATE BODY in SPIDY VR (below), does it again.

Keep the game window in front on the desktop: the game pauses while another
window is in front of it, the launcher included. If the headset shows a still
picture, click the game window once.

Press STOP VR, or close the game, to end the session. Each session leaves a
report in the "reports" folder; send it along when you report a problem.
VR starts once per game launch: to play in VR again after STOP VR, or after
VR ended by itself, close the game first, then press START VR.

For VR the launcher starts the game with a small desktop window, without
frame generation or vertical sync, and with the game's Windows.Gaming.Input
off (under SteamVR it would take the controllers away from Spidy). It also
turns the game's Depth of Field off and its Sharpness to the lowest: in the
headset the game blurs away from where its own camera focuses, not from
where you look, and its sharpening rings on edges. "Game's depth of field"
and "Game's sharpening" in the launcher's options keep your own. Your
settings come back when the game closes.

While you swing in VR, the game window on the desktop shows your left eye,
30 pictures a second: every picture sent to the monitor costs the graphics
card time the headset could have. "Monitor frame rate" in the launcher's
options chooses Every frame, 60, 30, 15 or 5 a second. On menus, cutscenes
and the flat screen the window shows the game as usual.

The picture in each eye is anti-aliased and upscaled by the game itself:
the scene is rendered smaller and built up to the eye's full size from the
last few frames, as DLSS does (any graphics card). "Upscaling" in the
launcher's options chooses how much smaller: Quality (67%, the default),
Balanced or Performance for more frames, High or Off for the sharpest
picture. The game's own DLSS, FSR or XeSS setting does not reach the
headset, whatever it is set to.

Wet streets, water and glass mirror their surroundings in the headset when
Screen Space Reflections is on in the game's own graphics settings (Settings >
Display and Graphics > Graphics), as on the monitor. Turned off there, they
are off in the headset too; turned on there while you play, they appear in
the headset the next time you start VR. With Ray-Traced Reflections on in
the game, the headset shows neither kind.


CONTROLS
--------
The grip webs and the trigger reels, as below. To shoot webs with the
trigger instead, set WEB BUTTON to TRIGGER (Settings > SPIDY VR, or "Web
button" in the launcher's options): the trigger then shoots and holds a
web, and the grip reels in and shoots web balls. Fists, the T-pose
calibration and the game's menus keep the buttons as they are.

Swinging
  Squeeze a grip ........ shoot that hand's web; hold to swing
  Release the grip ...... let go
  Trigger ............... reel in while the web is attached
  Pull a hand sharply ... zip toward the web's anchor
  Pull a hand in ........ the web is a rope in your hand: hanging from it,
                          you go where you pull it, as fast as you pull
  A corner in the way ... the web bends round it and holds you from there;
                          swing back and it lies straight again. Round six
                          edges at most
  Walking with a web .... it pays out as you go, and holds you at its new
                          length when you step off an edge
  PHYSICAL WEBS ......... switches those three off (Settings > SPIDY VR, or
                          "Physical webs" in the launcher's options): a wall
                          in the way then cuts the web off, as before

Grabbing
  Grip aimed at a prop or thug ... catch it with your web
  Trigger / sharp pull ........... reel it in / yank it to your hand
  Release the grip ............... throw it (swing harder to throw faster)
  A thug you pull flies off his feet, flailing, and lands hard when thrown.
  Slammed into a wall or the ground he gets hurt, and thrown into another
  thug, or hit by a prop you throw, that thug goes down too.
  "Thug" here means any enemy, flying ones included.

Web shooter
  Trigger of a hand ...... shoot a web ball where that hand points: the
  without a web            game's own, from your wrist. Aimed close to a
                           thug, it goes to him; otherwise it splats on the
                           first thing in its way. One ball per pull; the
                           other hand can shoot while one hand swings.
                           Three quick hits web a thug up.

Aim markers (where each hand's web would go if you pressed its web button
now; the left hand's are blue, the right hand's orange)
  Ring with a dot ...... the web holds there
  Faint dashed ring .... nothing in reach: it holds in open air (with "Webs
                         hold in open air" off it would miss: no marker)
  Red cross ............ it would miss (a car, or a wall in your way)
  Ring of three arcs ... it catches that prop or thug (the ring turns, and
                         closes in as it locks on)
  AIM MARKERS .......... hides or shows the markers: Settings > SPIDY VR, or
                         "Aim markers" in the launcher's options (the next
                         session starts as you left it). X no longer
                         switches them: it opens the game menu

Your body and fists
  Look down ............ you are Spider-Man: his body under you, his arms
                         and hands where your controllers are
  Punch a thug ......... a fist moving fast into him hits: see Fighting,
                         below
  Squeeze a grip ....... closes that hand into a fist (it shoots a web too;
                         with WEB BUTTON: TRIGGER, a web ball)
  T-pose, both triggers  calibrates your body (when the panel asks): his
                         height and arms become yours. B skips it

Fighting (web strikes: punching across the room, as in Batman: Arkham Shadow)
  Punch a thug ......... a fist that meets him fast lands a blow, as hard as
                         it moved. Every blow kicks in your hand, sounds,
                         bursts where it landed (a comic's star: yellow for
                         a jab, red for a harder blow) and stops time for a
                         moment
  Punch at a thug ...... web strike: you fly to him and the blow lands as
  out of reach           you arrive. He can be 10 m away, anywhere you can
                         see him: across the street, below you, in the air.
                         From the ground, a jump, a fall or a swing (your
                         webs let go). Throw the fist at him; it need not
                         be a hard punch
  Punch along your ..... the same: you go to him, and the web lets go. A
  web at the thug        sharp pull still yanks him to you instead
  it holds
  A hard punch ......... sends him flying, the harder the farther
  Uppercut ............. lifts him into the air. Strike up after him: you
                         hang beside him for a moment after every blow you
                         land in the air, long enough for the next
  Punch down at a ...... slams him into the ground
  thug in the air
  Punch down onto a .... knocks him down, stunned
  thug on his feet
  Blows in a row ....... count up (X2, X3 ... lettered over each blow): each
                         hits a little harder than the one before, the fifth
                         is a finisher that sends him flying far, once you
                         throw it as called (below). Three and a half
                         seconds without a blow and the count starts over
  A ring over a thug ... your next punch goes to him and to nobody else.
                         A thin ring is on the enemy you look at, up to
                         10 m away, and moves with your look. Hit him and
                         the ring locks on: it gets four ticks and stays on
                         him wherever you look, until he is down. A web
                         that holds a thug locks him too. While one is
                         marked no punch flies you to another: a fist that
                         falls short of him is just a miss. No ring, no web
                         strike: look at him first
  Changing your mind ... look squarely at another enemy for a third of a
                         second, with the locked one well off to the side:
                         the ring goes to him. Four seconds without a blow
                         at the locked one and the ring follows your look
                         again
  KO! .................. a thug your blow knocked out: lettered where he
                         stood, with a thump in both hands and time easing
                         for an instant. He is done, the ring is free: look
                         at the next one
  An arrow into the .... a called blow, as Arkham Shadow's. From your second
  ring                   blow on, the arrow shows how your next fist must
                         travel, as you see him. It comes in from the side
                         your fist comes from and points where it goes:
                         from your right to your left (a right hook), from
                         your left (a left hook), from below (an uppercut)
                         or from above (a blow down). Do it within arm's
                         reach and the fist that lands that way is the
                         called blow. Do it at the marked thug from across
                         the room and you fly to him and it lands as a
                         called blow all the same: swing it, do not jab. It
                         hits half again as hard and throws him that way: a
                         hook sends him flying, up lifts him, down floors
                         him or slams him out of the air. Then the arrow
                         shows a new way. A plain punch or a straight web
                         strike is an ordinary blow and the arrow stays as
                         it is. The arrow is white over a blue ring, and
                         the ring runs down as your combo's time does. A
                         called blow's burst is printed out of register, a
                         blue edge and a pink one
  The yellow arrow ..... into a jagged red ring: it calls the finisher.
                         After four blows the next one is the finisher only
                         if a fist lands, or opens a web strike, going the
                         way the yellow arrow shows; until then your blows
                         are plain ones and the count stays at four
  How the marks look ... as a comic prints them, like Spidy's videos: flat
                         colour with a black outline, popping in rather than
                         fading. They are drawn the same in both eyes, so a
                         coloured edge is print and not your lenses
  Yank a thug from ..... he flies to you, wherever you go meanwhile and
  the air                whether or not you still hold the grip, and floats
                         an arm's length in front of you. As he arrives
                         time slows and you hang in the air (not while a
                         web carries you): hit him. Each blow in the air
                         holds you up a little longer. After a second and a
                         half without a blow he falls
  A hand whose web holds a prop, or carries you through the air, neither
  punches nor strikes. A hand whose web only hangs from a wall while you
  stand does both: clench your fists as you like.

Moving
  Left stick ........... walk and run (with FLIPS on, in the air it flips
                         you, below)
  Right stick .......... flick left or right to turn (30 degrees; SPIDY VR
                         in Settings sets how far, or turns it off). With
                         SMOOTH TURN on, hold it to turn steadily instead:
                         the further you tilt it, the faster
  A .................... jump (from the ground, a wall or a perch; in the
                         air it flips you, below: no web zip from the wrist).
                         On the ground the jump is made as you let A go
  Hold A ............... a charged leap: hold A on the ground (your hands
                         buzz as it builds, for most of a second) and let
                         go. You leap 4 to 11 m high by how long you held,
                         and forward too with the left stick tilted. It is
                         Spidy's own: the game's Charge Jump is a skill a
                         save may not have
  B .................... dodge: the game's B. You dodge the way the left
                         stick points; with the stick at rest the game
                         chooses the way. Not while a web carries you
  Y .................... interact: the game's Y (backpacks, doors, prompts;
                         web strike in a fight). Look at the thing and
                         press Y, within about a metre of it: you turn to
                         it and the game takes it
  Hold Y ............... you step up to what you look at until the game
                         takes it (2 seconds at most; let go to stop). At a
                         door or a console: look at it and hold Y. Not in a
                         fight
  Right stick click .... the game's LB + RB: its web pull. Where the game
                         shows LB + RB (a door or a grate to tear off),
                         click the right stick, or hold it for as long as
                         the game wants the two held. Not while a web
                         carries you
  Right stick down ..... heal: the game's D-pad down. Push the stick
                         straight down and hold it a moment; the right
                         controller ticks when the game gets it. In the air
                         too
  Right stick up ....... the game's LT + RT: its zip to a point. Where the
                         game shows LT + RT (a perch, a vent to enter),
                         look at it, push the stick straight up and hold
                         it. Not while a web carries you
  Hand to your temple, . scan your surroundings: the game's R3. Raise either
  trigger                hand beside your head and pull its trigger there;
                         it holds for as long as you hold the trigger. No
                         web ball from that pull
  Menu button .......... pause
  X .................... game menu (map, suits, skills)
  A, B and Y are the game's own A, B and Y: where the game shows one of
  them on screen, press that button. Your grips shoot webs, so they are
  not the game's LB and RB in VR: the right stick's click is both. The
  controllers have no D-pad, and the triggers are webs too: the right
  stick pushed down is the game's D-pad down, pushed up its LT + RT. A
  push counts from the stick at rest, straight down or up: turning, a
  diagonal and a quick flick do neither.
  VR or a flat screen .. pause, Settings > SPIDY VR > VIEW: FLAT SCREEN shows
                         the game on a screen in the headset, VR goes back.
                         (Clicking both sticks no longer switches.)

Walls
  Walk at one .......... push the left stick straight at a wall beside you
                         and you hop onto it and walk on up it. Falling
                         past a wall, push at it and it takes you
  Fly or jump into one . you stay on it, an arm's length off. At speed you
                         run along it with the speed you had, and nothing
                         pulls you down it; without the left stick or a web
                         you slow down, stop and stay
  Left stick ........... walks the wall where you look along it; pushed at
                         the wall, up it. It steers a run, too
  A .................... jumps off the wall, out and up
  A web ................ pulls you off the wall, or along it
  The wall's top ....... go up over it and you hop onto the roof; at a walk
                         you step round a corner onto the next wall
  STAND ON WALLS ....... walking a wall or stopped on it, your view turns so
                         that the wall is your floor, and turns back when you
                         leave it. A run along a wall never turns it. Off
                         (Settings > SPIDY VR, or "Stand on walls" in the
                         launcher), you stay upright beside the wall

Slow motion (as in Blade & Sorcery)
  Click the left stick . the world slows to a third of its speed, easing in;
                         click again and it eases back. Your head and hands
                         keep real time: you move fast in a slow world, and
                         your swing slows with it
  Focus meter .......... the cyan ring around an hourglass over the back of
                         your left wrist. It empties while slow motion lasts
                         (12 seconds from full) and ends it when empty; it
                         refills a moment after slow motion ends (12 seconds
                         from empty). With too little left a click flashes
                         it red and does nothing
  A menu, the game screen or the flat screen ends slow motion
  By itself ............ time also slows for a moment, with no press and
                         no focus spent: at a finisher, an uppercut that
                         lifts a thug and a slam; at a called blow; when a
                         thug you yanked from the air arrives in front of
                         you; and at a perfect dodge (a dodge the game
                         itself slows time for). A flurry of blows slows
                         time once, not for every blow

Flips (experimental, off until you switch FLIPS on in Settings > SPIDY VR or
in the launcher's options)
  Left stick in the air  turns you head over heels about your head, as
                         fast as you tilt it (ahead a front flip, back a
                         backflip, to a side a cartwheel); it does not move
                         you meanwhile. A stick held from a running jump
                         still steers you until you let it go once
  Let go of the stick .. you turn back level
  Tap A in the air ..... one flip, about a second and a half, toward where
                         the left stick points (at rest a front flip)
  Hold A in the air .... the left stick turns you, and at rest you stay
                         turned; let go of A and the stick to come back level.
                         Landing or a wall levels you too
  FLIP SPEED ........... how fast all of it turns you: 180 degrees a second
                         at full tilt unless you change it (90 to 480;
                         Settings > SPIDY VR or "Flip speed" in the launcher)

Menus and cutscenes show on a screen in the headset. There the controllers act
as an Xbox controller: left stick moves, A selects, B goes back, grips switch
tabs, the menu button is Start.

Cutscenes have depth on that screen (3D, as in a 3D cinema): the game draws
them once for each of your eyes. So do finishers and the other scenes the game
plays by itself. The far background lies far behind the screen, and what is
close to the game's camera comes out of it toward you. Subtitles and prompts
stay on the screen itself. Menus, hint cards, the map and loading are flat, as
before. GAME SCREEN 3D in Settings > SPIDY VR (or "Game screen 3D" in the
launcher's options) sets how much: LOW and MEDIUM keep the scene further
behind the screen, HIGH shows everything as far from you as it is from the
game's camera, OFF is the flat picture again.

The game's HUD (health, gadgets, minimap, prompts, subtitles and the game's
markers) floats 2 m ahead of you, about 60 degrees wide. It stays still while
your head moves a little, and glides back in front of you when you look
further away. A marker sits over its target while the target is inside that
frame; otherwise the game's edge arrow points the way. HUD in Settings > SPIDY
VR (or "HUD" in the launcher's options) makes it SMALL, MEDIUM or LARGE, or
switches it OFF.

VR settings
  They are a tab of the game's own Settings: pause with the menu button,
  choose Settings, then SPIDY VR, the last tab (Up from GAME gets there in
  one press). It works like the game's other tabs: the left stick moves and
  changes the selected setting, X resets it, Y resets the whole tab, B goes
  back.
  Choose the WEB BUTTON (GRIP or TRIGGER: the one that shoots and holds a
  web; the other reels in and shoots web balls), switch the aim markers and
  webs holding in open air (a web that meets nothing within 100 m holds
  there; off, it misses) on or off, or step the
  swing speed limit, your weight, PHYSICAL WEBS, snap turn, smooth turn,
  controller vibration, the VIEW (VR or FLAT SCREEN; each session starts in
  VR), the game screen's size, its 3D depth for cutscenes and the HUD's size
  (OFF hides it in VR).
  Under EXPERIMENTAL, FLIPS (off until
  you switch it on) lets the left stick and A in the air flip you, and FLIP
  SPEED sets how fast. Changes apply at once. The
  launcher starts your next session with them; its options show them too.
  CALIBRATE BODY: set it to ON RESUME and resume, and the T-pose panel asks
  for your measurements again.
  Catching props and thugs, the web shooter, your own body and punching are
  always on.
  The title screen's Options have no SPIDY VR tab: load your save first, or
  set them in the launcher.


IF SOMETHING IS WRONG
---------------------
- "This game version is not supported": the launcher's game line names your
  version and the fix. Spidy supports version 4.630.0.0, Steam's since July
  8, 2026. An older version: Steam may be set to one of the game's rollback
  betas (right-click the game in Steam > Properties > Betas > None) or have
  an update waiting. Version 4.630.0.0 that still fails: a mod, a patch or a
  damaged download changed Spider-Man.exe; Properties > Installed Files >
  Verify integrity of game files puts Steam's file back. A newer version:
  Spidy reads the game's code at fixed places, so a game update needs a
  Spidy update.
- The headset is not found: connect it first (SteamVR headsets: start
  SteamVR), then press Check in the launcher. On Automatic it shows which VR
  runtime has your headset; if it picks the wrong one, choose yours in the
  list.
- Steam Link or another SteamVR headset, and SteamVR is not in the
  launcher's VR runtime list (version 0.2.9 and older, when Virtual Desktop
  or the Meta Quest Link app is installed too): in SteamVR open Settings >
  OpenXR and press "Set SteamVR as OpenXR runtime", then press Re-check in
  the launcher. Newer versions find SteamVR by themselves.
- "VR already ran in this game" (older versions: "Game XR start: 1000"):
  VR starts once per game launch. Close the game (Spider-Man.exe gone from
  Task Manager), then press START VR.
- "VR needs administrator rights": Steam or the game runs as administrator
  on your PC, and Windows keeps a program without those rights out of such
  a game. Choose "Restart as administrator" when the launcher asks (Windows
  asks you to allow it), close the game if it is running, then press START
  VR. Or have Steam start without administrator rights.
- "Windows refuses Spidy access to the game ... a security program": your
  antivirus is guarding the game. Allow the Spidy folder in it, close the
  game, then press START VR.
- "render memory module unavailable ([WinError 5] Access is denied)", then
  "No loaded player within three minutes" (version 0.2.9 and older): Windows
  kept Spidy out of the game, for one of the two reasons above. Close the
  game, right-click Spidy Launcher, choose "Run as administrator" and press
  START VR; if nothing changes, allow the Spidy folder in your antivirus.
  Newer versions say which it is.
- VR does not start and the game stays flat: send the whole "reports"
  folder. The newest game-vr-...-console.log holds everything the launcher
  showed, and the report beside it says where it stopped.
- The game's menus ignore the VR controllers: keep the game window in front
  on the desktop, and start the game from the launcher with the game closed
  (the launcher turns off what lets SteamVR's virtual gamepads take over).
- Buttons do nothing in VR (no menu, no doors, no web strike) while webs and
  walking work, and the game's prompts show keyboard keys: Steam Input is
  on for the game. In Steam, right-click the game > Properties > Controller
  > Disable Steam Input, close the game, then press START VR.
- The game closes at its first logo, every time, and you have OpenXR
  Toolkit installed: version 0.3.0 and older do not run with it. Newer
  versions switch it off for Spider-Man by themselves (the launcher's log
  says so) and leave it on for your other games. On an older version, turn
  it off in its companion app, or choose SteamVR under "VR runtime".
- The picture breaks up: start the game from the launcher with the game
  closed. Attached to a game that is already running, Spidy cannot give it
  the larger render memory VR needs.
- The picture freezes or jumps for a second or two at a time, however fast
  the game runs: with a wireless headset (Virtual Desktop, Steam Link), connect
  the PC to the router by network cable and turn the PC's Wi-Fi off, then
  connect the headset again. A PC on Wi-Fi sends every picture through the
  air twice. Spidy warns in its window when it starts with the PC on Wi-Fi,
  and says after a session if the stream stalled.
- Low frame rate: set "Upscaling" in the launcher's options to Balanced or
  Performance first (the picture keeps its size and gets a little softer;
  on an RTX 4080 laptop, Quality gave 61 frames a second and Performance 77
  where Off gave 39). Then lower "Render resolution" (below 100% renders
  fewer pixels than your headset asks for), or your streaming quality in
  Virtual Desktop. On a laptop, plug it in and choose its highest
  performance mode. A lower "Monitor frame rate" helps a little more, most
  if the game window is large.
- Blurry edges: set "Upscaling" in the launcher's options to High or Off,
  then raise "Render resolution" above 100%; try 125% first. Both cost
  frame rate and memory: 150% gave about 30% fewer frames on an RTX 5090.
  Press Check beside your headset to see the pixels per eye. Virtual
  Desktop's quality setting raises what 100% is.
- The aim markers distract you: switch AIM MARKERS off in Settings > SPIDY VR
  (or "Aim markers" in the launcher's options).
- Webs shot at the sky hold somewhere you can't see: switch "Webs hold in
  open air" off in Settings > SPIDY VR or the launcher's options; a web then
  needs something within 100 m to hold on to.
- Web balls fly when you only meant to reel in: the trigger (the grip with
  WEB BUTTON: TRIGGER) reels only while that hand's web is attached;
  otherwise it shoots.
- You would rather shoot webs with the trigger, as in other VR games: set
  WEB BUTTON to TRIGGER in Settings > SPIDY VR (or "Web button" in the
  launcher's options). The grip then reels in and shoots web balls.
- Snap turning makes you uneasy, or the vibration is too strong: Settings >
  SPIDY VR has both.
- Swinging feels floaty, or falls too hard: WEIGHT in Settings > SPIDY VR
  (or "Weight" in the launcher's options) sets how heavy you are while
  swinging and after letting go, until you land: 100% is real gravity and
  the default (80% until October 12; a launcher that still had 80% starts on
  100%), up to 300%. Jumps and falls without a web are the game's own.
- A web lets go at a corner, or does something odd as it bends round one:
  it bends round six edges at most, and lets go where it finds no edge to
  bend round. Switch PHYSICAL WEBS off in Settings > SPIDY VR for webs as
  before, and tell us where it happened.
- Lowering your arm on a web moves you: with PHYSICAL WEBS the web's length
  is measured to your hand, so a taut web follows it. That is also how you
  pull yourself along it. Off: PHYSICAL WEBS in Settings > SPIDY VR.
- A jump comes a moment late: on the ground Spidy waits to see whether you
  hold A for a charged leap, and jumps as you let go of a tap.
- The game closed when you chose Quit in the pause menu (every time, up to
  the fourth build of October 12): on its way to the main menu the game asks
  Steam for a ticket with one argument missing, and Steam read what happened
  to lie there. Spidy now guards that call. If it still happens, send the
  session's report: its "steam_guard" says what the game passed.
- A vent, a grate or a hatch at your feet or over your head shows no prompt
  and takes no button: the game offers a thing only when its own camera
  looks at it, and Spidy turned that camera left and right with your head
  but not up and down. It follows your head both ways now: look at the
  thing, then press what the game shows (Y; the right stick's click for
  LB + RB; the right stick up for LT + RT).
- The picture is softer, or a cutscene has lost its blurred background:
  Spidy turns the game's sharpening and depth of field off for VR. "Game's
  sharpening" and "Game's depth of field" in the launcher's options keep
  yours.
- Clicking both sticks no longer shows the flat screen: it is VIEW in
  Settings > SPIDY VR now.
- You would rather turn smoothly than in steps: SMOOTH TURN in Settings >
  SPIDY VR (or "Smooth turn" in the launcher's options). If it makes you
  uneasy, try a slower speed, or OFF for snap turning again.
- The menu screen is too small or too large: "Game screen size" in Settings
  > SPIDY VR.
- A cutscene's depth strains your eyes, or things come too far out of the
  screen: GAME SCREEN 3D in Settings > SPIDY VR, LOW or OFF. Too little
  depth: HIGH.
- The HUD is too small, too large or in the way: HUD in Settings > SPIDY VR
  (or "HUD" in the launcher's options): SMALL, MEDIUM, LARGE, or OFF.
- The game asks for LB + RB (a door you cannot get through, a grate) and
  your webs do nothing to it: click the right stick. That is the game's
  LB + RB in VR; your grips shoot your own webs there.
- The game asks you to heal, or to press LT + RT (a vent, a point to zip
  to), and nothing on the controllers does it: push the right stick
  straight down to heal, straight up for LT + RT, and hold it a moment.
  Versions up to 0.3.0 had no control for either.
- You fly at a thug you did not mean to strike: a web strike opens with a
  fist thrown out fast (2.6 m/s or more) toward an enemy within 10 m, about
  35 degrees to either side. Reaching for a web slowly, or pulling a hand
  back, opens none.
- A punch at a thug far off does nothing: he must be an enemy on his feet or
  in the air (not one already knocked out), within 10 m and in sight, and the
  fist thrown at him, out from your body. A hook, or a hand whose web holds a
  prop or swings you, opens no strike.
- The blows are too loud or the bursts too bold: tell us; this build has
  no switch for them yet.
- B no longer interacts, or Y no longer opens the game menu: the buttons
  moved to make room for the dodge. B dodges, Y interacts (as the game's
  prompts say) and X opens the game menu.
- Y does nothing at a door, a console or a backpack: look at the thing as
  you press it, from within about a metre (closer than feels natural at a
  wall in VR). The game takes its Y only for what its own camera sees and
  its hero faces; Spidy turns both to where you look. From further away
  hold Y: you step up to the thing and it opens. B there dodges, away from
  it.
- A punch flew you to an empty spot, or hit nothing you could see: thugs of
  a fight's later waves wait unseen where they will appear, and until
  October 12 fists, webs and punches across the room took them for enemies.
  A thug who is knocked out is nobody's target either.
- The game says "Tap RB repeatedly to web up the turret" (Fisk's office):
  shoot web balls at the turret, three in a row. Then, for "Hold LB + RB to
  yank and throw turrets": from within about 15 m, look at the webbed turret
  and hold the right stick's click until it is thrown. Being shot during the
  first second and a half stops the yank, as in the plain game: web both
  turrets first, or wait until they reload. The webbing lasts four seconds.
- The view turning onto a wall makes you uneasy: switch STAND ON WALLS off in
  Settings > SPIDY VR (or "Stand on walls" in the launcher's options). You
  still stay on walls, upright beside them.
- A flip you did not mean, or flips make you uneasy: switch FLIPS off in
  Settings > SPIDY VR (or "Flips (experimental)" in the launcher's options).
  It is off unless you switched it on. Flips too fast or too slow: FLIP
  SPEED, under it (or "Flip speed" in the launcher).


SHARING
-------
Please share the original zip with this credit intact, so players get working
files and know where updates come from.

Spidy is Copyright (C) 2026 Ilya Mezerowsky, and free software under the GNU
General Public License, version 3 (LICENSE.txt). Its source code is at
https://github.com/IlyaMez/SpidyVR

Spidy is a free fan project, not affiliated with or endorsed by Insomniac
Games, Sony Interactive Entertainment, Marvel or Valve. Third-party licences
are in THIRD_PARTY_NOTICES.md and docs\licenses.

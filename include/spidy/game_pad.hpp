#pragma once
#include "tracking.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

// A virtual Xbox controller for the game's menus, cutscene prompts and hint
// cards, played with the VR controllers, and for native walking and jumping
// in VR gameplay. The game reads controllers through
// XInput (it loads XInput1_4.dll itself); Spidy serves controller 0 there,
// merged with a real controller in that slot.
namespace spidy::game_pad {
// XINPUT_GAMEPAD's layout and button bits.
struct State {
    uint16_t buttons{};
    uint8_t leftTrigger{}, rightTrigger{};
    int16_t thumbLX{}, thumbLY{}, thumbRX{}, thumbRY{};
    bool operator==(const State&) const = default;
};
static_assert(sizeof(State) == 12);
enum Button : uint16_t {
    dpadUp = 0x1,
    dpadDown = 0x2,
    dpadLeft = 0x4,
    dpadRight = 0x8,
    start = 0x10,
    back = 0x20,
    leftThumb = 0x40,
    rightThumb = 0x80,
    leftShoulder = 0x100,
    rightShoulder = 0x200,
    a = 0x1000,
    b = 0x2000,
    x = 0x4000,
    y = 0x8000,
};
enum class Mapping {
    none,
    // VR gameplay: the controllers swing, walk and jump. The menu button
    // (Start: pause) and X (Back: map, suits, skills) reach the game as
    // buttons; walking, jumping, B (the game's B: dodge), Y (the game's
    // Y: interact), the right stick's click (the game's LB + RB: its
    // web pull), that stick pushed down (the game's D-pad down: heal) and
    // up (its LT + RT: the zip to a point) reach it as the swing leaves
    // them (Walk). The virtual controller's own right stick turns the
    // game's camera to where the player looks (CameraLook).
    gameplay,
    // The game screen: every control as on an Xbox controller. A, B, X and Y
    // sit where Xbox has them; grips are the bumpers.
    menus,
};
// Native walking and jumping in VR gameplay: the left stick in the game
// camera's horizontal axes (-1..1) and the jump, both as the swing leaves
// them to the game. Once this controller has been pressed (the VR menus are
// played with it), the game plays the player with it and ignores the
// keyboard, the input bridge's W/A/S/D/Space included: on October 6 those
// moved the player 0 m while this controller's A jumped him 2.9 m.
// interact: the game's Y, held. The game interacts with it (a backpack, a
// door, an on-screen prompt) and web-strikes with it in a fight; held on, it
// walks the player up to the thing (InteractStep). dodge: the
// game's B, held: its dodge, the way the stick points (a perfect dodge and
// all that follows it are the game's). The VR controllers' Y and B give
// them, where the game's own prompts show them; until October 11 B was the
// interact, Y the game menu and X the aim markers' switch. pull: the game's
// LB and RB together, held: its web pull, where it shows the two (a door or
// a grate to tear off). The grips are no bumpers in VR play (they shoot the
// webs), so the right thumbstick's click gives them (PullPress): until
// October 11 nothing did, and a door of the first mission that asks for
// LB + RB could not be opened in VR. heal: the game's D-pad down, held: its
// heal. zip: the game's LT and RT together, held: its zip to a point, where
// it shows the two (a perch, a vent to enter). The VR controllers have no
// D-pad, and their triggers and grips are the webs in VR play, so the right
// thumbstick pushed down and up gives them (StickPush): until October 12
// nothing did, and the first mission stopped at the heal it asks for and at
// its vent. look: the virtual controller's own right stick, its X, which
// turns the game's camera (CameraLook).
struct Walk {
    float right{}, forward{};
    bool jump{}, interact{}, dodge{}, pull{}, heal{}, zip{};
    float look{};
};
// The right thumbstick pushed straight down or up, as Walk::heal and
// Walk::zip. In VR play that stick turns the player with its left and right;
// its down and up did nothing. A push counts from the stick at rest, once it
// has been `pushed` far and no more than `aside` off the line for
// holdSeconds, and holds until the stick is back within `released`: a turn
// rolled round to the top, a diagonal and a flick through the line are none.
// Outside VR play nothing holds, and a stick held through that must be let
// go first (on the game screen it is the game's own right stick).
class StickPush {
  public:
    static constexpr float pushed = .75f, aside = .45f, released = .5f, holdSeconds = .15f;
    enum class Way { none, down, up };
    // seconds: since the previous update. play: the controllers play VR.
    // x, y: the right thumbstick, -1..1 (0 for a hand that is not tracked).
    Way update(float seconds, bool play, float x, float y) {
        const float dt = std::isfinite(seconds) ? std::clamp(seconds, 0.f, .1f) : 0.f;
        // A bad reading is a stick at rest.
        const bool rest = !std::isfinite(x) || !std::isfinite(y) || std::hypot(x, y) < released;
        if (!play || rest) {
            way_ = Way::none;
            held_ = 0;
            spent_ = !rest;
            return Way::none;
        }
        if (spent_)
            return Way::none;
        if (way_ == Way::none) {
            if (std::abs(x) > aside) {
                spent_ = true;
                return Way::none;
            }
            if (std::abs(y) < pushed)
                return Way::none;
            way_ = y < 0 ? Way::down : Way::up;
        } else if ((way_ == Way::down) != (y < 0) || std::abs(y) < released) {
            way_ = Way::none;
            held_ = 0;
            spent_ = true;
            return Way::none;
        }
        held_ += dt;
        return held_ >= holdSeconds ? way_ : Way::none;
    }

  private:
    Way way_{};
    bool spent_{};
    float held_{};
};
// The game's own camera, kept looking where the player looks. VR shows the
// world from the headset; the game's follow camera is still there, turns
// with the right stick alone (which in VR play turns the player's view with
// its left and right, and heals and zips with its down and up: StickPush)
// and otherwise stays where the last run left it. The game offers
// its Y only for a thing that camera has in view. In the game, October 12,
// 0.4 m from the red door at the end of the first mission's hall: "Open
// Door" showed with the camera within 30 degrees of the door and not from 50
// (to one side) or 70 (to the other) on, and there Y did nothing; in the
// headset the evening before, 25 presses at that door opened nothing.
//
// So the virtual controller's right stick turns that camera until it looks
// along the head's level direction. The game's camera answers the stick
// steeply (measured there, degrees a second: 180 at full tilt after a fifth
// of a second, 22 at 0.76, 9 at 0.6, 3 at 0.5), so the stick is the cube
// root of the turn wanted, above `floor`: from 170 degrees off the camera
// was within 4 in 1.4 s and stayed there. It starts beyond startDegrees and
// rests within stopDegrees: a head that looks about turns nothing.
class CameraLook {
  public:
    static constexpr float startDegrees = 12, stopDegrees = 4, fullRate = 180, seconds = .25f, floor = .45f;
    // look: where the head looks; camera: the game camera's forward (both in
    // the world). steer: VR play, with that camera the player's own. Returns
    // the right stick's X, -1..1.
    float update(Vec3 look, Vec3 camera, bool steer) {
        look.y = camera.y = 0;
        // Straight up or down says no way to look.
        if (!steer || !finite(look) || !finite(camera) || length(look) < .3f || length(camera) < .3f) {
            turning_ = false;
            error_ = 0;
            return 0;
        }
        look = normalized(look);
        camera = normalized(camera);
        // To the camera's right, as the game takes the stick: cross(forward, up).
        error_ = std::atan2(dot(look, cross(camera, {0, 1, 0})), dot(look, camera)) * 57.29578f;
        const float off = std::abs(error_);
        const bool turn = off > (turning_ ? stopDegrees : startDegrees);
        turns_ += turn && !turning_;
        turning_ = turn;
        if (!turn)
            return 0;
        const float rate = std::min(1.f, off / (fullRate * seconds));
        return std::copysign(floor + (1 - floor) * std::cbrt(rate), error_);
    }
    // From the camera's forward to the head's level direction, degrees (to
    // the camera's right: more than 0); 0 while it is not steered.
    float error() const {
        return error_;
    }
    // The times the stick began to turn the camera.
    uint32_t turns() const {
        return turns_;
    }

  private:
    float error_{};
    bool turning_{};
    uint32_t turns_{};
};
// The right thumbstick's click as Walk::pull. Like the left one's
// (StickPress, slow_motion.hpp) it is not half of clicking both, which
// switches between VR and the flat screen: it holds once the stick has been
// down chordSeconds without the other, for as long as it stays down, and a
// click let go sooner holds for tapSeconds after it. Outside VR play
// nothing holds, and a stick held through that must be let go first.
class PullPress {
  public:
    static constexpr float chordSeconds = .12f, tapSeconds = .2f;
    // seconds: since the previous update. play: the game has the player in
    // VR. mine, other: the right and the left thumbstick's click.
    bool update(float seconds, bool play, bool mine, bool other) {
        const float dt = std::isfinite(seconds) ? std::clamp(seconds, 0.f, .1f) : 0.f;
        if (!play) {
            down_ = mine;
            spent_ = true;
            tap_ = 0;
            return false;
        }
        if (!mine) {
            tap_ = down_ && !spent_ && held_ < chordSeconds ? tapSeconds : std::max(0.f, tap_ - dt);
            down_ = spent_ = false;
            return tap_ > 0;
        }
        if (!down_) {
            down_ = true;
            spent_ = false;
            held_ = tap_ = 0;
        } else {
            held_ += dt;
        }
        spent_ = spent_ || other;
        return !spent_ && held_ >= chordSeconds;
    }

  private:
    bool down_{}, spent_{};
    float held_{}, tap_{};
};
// Y held, to reach what it is for. The game takes its Y only close to the
// thing: a door of the first mission showed "Open Door" 1.1 m from its middle
// and not 1.2 m, nearer than a VR player stands to a wall (October 11: four
// presses from 1.9 to 2.5 m did nothing, in the headset and in the plain
// game alike). Y held longer than holdSeconds walks the player on where they
// look, at walkStick of the stick, for walkSeconds at most; the game takes a
// Y held on the way in (in the game, October 11), and the scene that follows
// ends the walk. A walk that meets a wall (slower than stallSpeed for
// stallSeconds, once it has had startSeconds to get going) ends too. A walk
// that ended starts again only with another press.
//
// The game also takes its Y only from a hero who faces the thing, and in VR
// he faces wherever he last walked: a player who slid along that door to its
// middle, or dropped off the wall above it, stood with his back or his side
// to it. In the game, October 12, at the red door of the first mission's
// hall, the camera on it: no "Open Door" for a hero turned away, and two Y
// taps did nothing. So every press begins with the stick where the player
// looks for turnSeconds (at walkStick the hero turned right round within
// 0.55 s and took a step of 0.13 m; a tilt of 0.3 turned nobody), and the
// game's Y stays down for pressSeconds at least, the turn's time: such a tap
// opened the door.
class InteractStep {
  public:
    static constexpr float holdSeconds = .3f, walkSeconds = 2, walkStick = .5f, startSeconds = .4f,
                           stallSeconds = .25f, stallSpeed = .3f, turnSeconds = .15f, pressSeconds = .6f;
    // seconds: since the previous update. held: the player's Y. allowed: the
    // game has the player on the ground, out of a fight (there Y is the web
    // strike, as long as the player presses it). speed: the player's over
    // the ground, m/s. Returns how hard the stick points where the player
    // looks: 0, or walkStick.
    float update(float seconds, bool held, bool allowed, float speed) {
        const bool timed = std::isfinite(seconds);
        const float dt = timed ? std::clamp(seconds, 0.f, .1f) : 0.f;
        if (held && !down_) {
            since_ = 0;
            turns_ = allowed;
        } else {
            since_ = std::min(since_ + dt, pressSeconds);
        }
        down_ = held;
        // A frame without a time turns nobody: no turn that never ends.
        turns_ = turns_ && allowed && timed && since_ < pressSeconds;
        const float turn = turns_ && since_ < turnSeconds ? walkStick : 0.f;
        if (!held) {
            held_ = walked_ = stalled_ = 0;
            walking_ = done_ = false;
            return turn;
        }
        held_ += dt;
        done_ = done_ || !allowed;
        if (!done_ && walking_) {
            walked_ += dt;
            stalled_ = walked_ > startSeconds && !(speed >= stallSpeed) ? stalled_ + dt : 0.f;
            done_ = walked_ >= walkSeconds || stalled_ >= stallSeconds;
        }
        if (done_ || held_ < holdSeconds) {
            walking_ = false;
            return turn;
        }
        steps_ += !walking_;
        walking_ = true;
        return walkStick;
    }
    // The game's Y is down: the player's, or the rest of pressSeconds after
    // a press that turned the hero.
    bool pressed() const {
        return down_ || turns_;
    }
    // Walks begun so far.
    uint32_t steps() const {
        return steps_;
    }

  private:
    float held_{}, walked_{}, stalled_{}, since_ = pressSeconds;
    bool walking_{}, done_{}, down_{}, turns_{};
    uint32_t steps_{};
};
// Where B, X and Y went down. On the game screen they are the game's own B,
// X and Y; in VR play B dodges, Y interacts and X opens the game menu. Held
// from one into the other, a button stays what it was until it is let go:
// Back held into play is no dodge, and an interact held into a menu the game
// opened is no Y there (which resets a Settings tab).
class PressOrigins {
  public:
    // buttons: XrFrame::buttons. play: the controllers play VR
    // (Mapping::gameplay); else a press counts for the game screen.
    void update(uint32_t buttons, bool play) {
        play_ &= buttons;
        screen_ &= buttons;
        (play ? play_ : screen_) |= buttons & ~(play_ | screen_);
    }
    // The buttons held since a press in VR play, and on the game screen.
    uint32_t play() const {
        return play_;
    }
    uint32_t screen() const {
        return screen_;
    }
    // `frame` with B, X and Y held only where they went down for `mapping`.
    XrFrame filtered(XrFrame frame, Mapping mapping) const {
        constexpr uint32_t face = buttonB | buttonX | buttonY;
        const uint32_t kept = mapping == Mapping::gameplay ? play_ : mapping == Mapping::menus ? screen_ : 0;
        frame.buttons = (frame.buttons & ~face) | (kept & face);
        return frame;
    }

  private:
    uint32_t play_{}, screen_{};
};
inline State fromControllers(const XrFrame& frame, Mapping mapping, const Walk& walk = {}) {
    State s;
    if (mapping == Mapping::none)
        return s;
    // Quest sticks rest a few percent off centre; the game adds its own dead zone.
    const auto stick = [](float v) {
        v = std::isfinite(v) ? std::clamp(v, -1.f, 1.f) : 0;
        return static_cast<int16_t>(std::abs(v) < .1f ? 0 : std::lround(v * 32767));
    };
    if (frame.buttons & buttonMenu)
        s.buttons |= start;
    if (mapping == Mapping::gameplay) {
        if (frame.buttons & buttonX)
            s.buttons |= back;
        if (walk.jump)
            s.buttons |= a;
        if (walk.dodge)
            s.buttons |= b;
        if (walk.interact)
            s.buttons |= y;
        if (walk.pull)
            s.buttons |= leftShoulder | rightShoulder;
        if (walk.heal)
            s.buttons |= dpadDown;
        if (walk.zip)
            s.leftTrigger = s.rightTrigger = 255;
        s.thumbLX = stick(walk.right);
        s.thumbLY = stick(walk.forward);
        s.thumbRX = stick(walk.look);
        return s;
    }
    const auto trigger = [](float v) {
        return static_cast<uint8_t>(std::isfinite(v) ? std::lround(std::clamp(v, 0.f, 1.f) * 255) : 0);
    };
    const auto& left = frame.hands[0];
    const auto& right = frame.hands[1];
    s.thumbLX = stick(left.stickX);
    s.thumbLY = stick(left.stickY);
    s.thumbRX = stick(right.stickX);
    s.thumbRY = stick(right.stickY);
    s.leftTrigger = trigger(left.trigger);
    s.rightTrigger = trigger(right.trigger);
    const std::pair<bool, Button> pressed[] = {
        {(frame.buttons & buttonA) != 0, a}, {(frame.buttons & buttonB) != 0, b},
        {(frame.buttons & buttonX) != 0, x}, {(frame.buttons & buttonY) != 0, y},
        {left.squeeze > .6f, leftShoulder},  {right.squeeze > .6f, rightShoulder},
        {left.stickClick, leftThumb},        {right.stickClick, rightThumb}};
    for (const auto& [on, button] : pressed)
        if (on)
            s.buttons |= button;
    return s;
}

// Hooks XInputGetState and XInputGetCapabilities in the game's XInput DLL.
// 0 when hooked; 8001 while the game has not loaded XInput yet (try again).
uint32_t install();
// The game reads `state` from controller 0 until `leaseMs` runs out, then a
// controller at rest. From the first submit on, controller 0 stays
// connected, so the game never reports it unplugged between menus.
void submit(const State& state, uint32_t leaseMs);
void uninstall();
struct Telemetry {
    // The game's reads of controller 0 while Spidy served it, and how many of
    // them had a button, trigger or stick in use.
    uint64_t reads{}, active{};
    uint32_t installed{}, buttons{};
};
Telemetry telemetry();
} // namespace spidy::game_pad

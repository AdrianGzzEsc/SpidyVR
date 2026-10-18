#pragma once
#include "gadget_wheel.hpp"
#include "math.hpp"
#include "vertex.hpp"
#include <string>
#include <vector>

// Spidy's phone (the player, October 17: "an overlay of something that looks
// like a flat phone screen ... I can start stop go back a track and auto play
// music"). MY PHONE on the pause menu's first page opens it in front of the
// player once play resumes; the stick picks a button, A presses it, B puts the
// phone away. The music goes on with the phone away. Songs are the files in
// the Music folder beside the launcher (phone_music.hpp).
namespace spidy {
enum class PhoneButton : int { previous, play, next, autoplay };
constexpr int phoneButtons = 4;
// The phone's screens: its home with an app for each of the others (the
// player, October 17: "a home screen with apps on it ... look more like the
// fnsm app from the 2nd game").
enum class PhoneScreen : int { home, music, map, gear };
constexpr int phoneApps = 3; // music, map, gear, in that order on the home screen
// The controllers while the phone is out, as steps: the stick pushed one way
// (x: -1 left, 1 right; y: -1 down, 1 up), A, B, and the volume from the
// triggers, a pull of the left one a step down and of the right one a step up
// (the player, October 17: "left trigger for down and right trigger for up").
// Each counts once until let go. Each opening counts everything as held, so
// the press that opened the phone does nothing.
class PhoneControls {
  public:
    static constexpr float pulled = .6f, released = .3f, pushed = .6f, centred = .3f;
    struct Event {
        int x{}, y{};
        bool pressed{}; // A
        bool closed{};  // B
        int volume{};   // -1 a step down, 1 a step up
    };
    // stick: -1 to 1 each way. a, b: held now. triggers: 0 to 1.
    Event update(float stickX, float stickY, bool a, bool b, float leftTrigger = 0, float rightTrigger = 0);
    void reset() {
        aHeld_ = bHeld_ = stickHeld_ = leftHeld_ = rightHeld_ = true;
    }

  private:
    bool aHeld_ = true, bHeld_ = true, stickHeld_ = true, leftHeld_ = true, rightHeld_ = true;
};
// What the phone's screens make of those steps. On the home screen the stick
// picks an app and A opens it, B puts the phone away; in an app B goes home.
// In MUSIC left and right pick a button and A presses it; in GEAR up and down
// scroll the list.
enum class PhoneAction : int { none, close, previous, play, next, autoplay };
class PhoneUi {
  public:
    // Rows GEAR shows at once.
    static constexpr int gearShown = 6;
    // gearRows: how many rows GEAR has now.
    PhoneAction update(const PhoneControls::Event& e, int gearRows);
    PhoneScreen screen() const {
        return screen_;
    }
    int app() const {
        return app_;
    }
    PhoneButton button() const {
        return button_;
    }
    int gearTop() const {
        return gearTop_;
    }
    // Each opening starts on the home screen, MUSIC picked.
    void reset() {
        screen_ = PhoneScreen::home;
        app_ = 0;
        button_ = PhoneButton::play;
        gearTop_ = 0;
    }

  private:
    PhoneScreen screen_ = PhoneScreen::home;
    int app_{}, gearTop_{};
    PhoneButton button_ = PhoneButton::play;
};
// The volume's step, percent.
constexpr int phoneVolumeStep = 10;
// The phone as an image shows it (NativeEyeFrame), in the world.
struct PhoneView {
    bool open{};
    WheelFrame frame{};
    float age{};
    PhoneScreen screen = PhoneScreen::home;
    int app{}; // the app picked on the home screen
    PhoneButton lit = PhoneButton::play;
    // GEAR: the player's gadgets (gadget_wheel.hpp), from gearTop on.
    struct GearRow {
        char name[24]{};
        int ammo{}, most{}; // most 0: none to count
        bool equipped{};
    };
    GearRow gear[gadgetPlaces]{};
    int gearCount{}, gearTop{};
    bool playing{}, autoplay{};
    int track{}, tracks{}; // track counts from 0; tracks 0: no songs
    int volume = 100;      // percent
    char song[64]{};
    // The image's own way from tracking space into its world: a phone in the
    // hand is placed again from the newest hand pose as the image goes out,
    // not where the hand was when the game rendered it (a frame or more
    // behind: "the phone is laggy kind of when its in my hand", October 17).
    Pose toWorld{};
    int hand = -1; // the hand holding it, -1: none
};
// Where the phone opens: in front of the head (tracking space), level with
// the chest, upright and facing it.
WheelFrame phoneFrame(Pose head);
// The phone held in a hand: just above it, facing the eyes, its top up (the
// player, October 17: "make the phone kinda stick to you hand until u press
// b"). In front of the head instead if the hand is not to be had.
WheelFrame phoneFrameAt(Pose head, Vec3 hand);
// The phone drawn from a back pocket (the player, October 17: "like im
// pulling it from my back pocket then the screen pops up"). A grip squeezed
// with its hand behind a hip, then that hand brought up in front within
// drawSeconds, still squeezed: the phone opens there. That grip is the
// draw's from its squeeze until it is let go (hands()), so it shoots no web.
// Let go or too slow, nothing opens. Outside VR play nothing holds.
class PocketDraw {
  public:
    // In the head's level axes, metres: a pocket is `pocketBelow` to
    // `pocketAbove` under the head, `pocketSide` to `pocketOut` to either
    // side, and at least `behind` back of it; the hand is out in front once
    // it is `front` ahead and no lower than `raised` under the head.
    static constexpr float pocketBelow = 1.1f, pocketAbove = .55f, pocketSide = .05f, pocketOut = .4f,
                           behind = .02f, front = .15f, raised = .65f, drawSeconds = 1.5f, pressed = .6f,
                           released = .3f;
    // seconds: since the previous update. play: the controllers play VR.
    // head, grips: in one space; tracked, squeezes: each hand's. True as the
    // phone comes out: hand() says where.
    bool update(float seconds, bool play, const Pose& head, const Pose grips[2], const bool tracked[2],
                const float squeezes[2]);
    // A bit per hand whose grip is the draw's.
    unsigned hands() const {
        return hands_;
    }
    Vec3 hand() const {
        return hand_;
    }
    // The hand that drew the latest phone: 0 the left, 1 the right.
    int drewWith() const {
        return drewWith_;
    }
    unsigned draws() const {
        return draws_;
    }

  private:
    bool down_[2]{};
    unsigned hands_{}, draws_{};
    int drawing_ = -1, drewWith_{};
    float time_{};
    Vec3 hand_{};
};
void appendPhone(std::vector<Vertex>& out, const PhoneView& view);
// What the phone shows for a song's file name: no folder or extension, and
// only what the overlay's font draws.
std::string phoneSongName(const std::string& fileName);
} // namespace spidy

#pragma once
#include "math.hpp"
#include "punch.hpp"
#include "swing.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <span>

// Fighting across the room: the combat of Batman: Arkham Shadow, with
// Spider-Man's own means. There a punch thrown at an enemy anywhere in the
// room slides the player to him while time slows, each blow answers with a
// flash and a kick in the hand, and blows that follow one another build a
// combo toward a finisher (its makers' talk on it: "Punching Across the
// Room"). Here:
//
// - The web strike. A fist thrown at an enemy beyond its reach takes the
//   player to him, along the ground or through the air, and its blow lands
//   as he arrives (WebStrike). Out of a swing, a jump or a fall it is a
//   dive; at an enemy in the air it goes up to him, and the player hangs
//   there for a moment after every blow landed off the ground, to land the
//   next.
// - Blows that carry. The game's melee answers the hardest blow with a
//   flinch (VALIDATION.md, October 8: kMelee with kFlyBack, kSuperFlyBack or
//   kAirborne played BotStateHitReactGame only), where its kinetic blow
//   sends a thug flying. A hard blow, an uppercut, a blow at a thug in the
//   air and a finisher go out as kinetic blows, and his flight gets a
//   velocity (deliver()): an uppercut lifts him off his feet to be followed
//   up, a blow down at a thug in the air slams him into the ground.
// - Flow. Blows within a few seconds of one another count up; each hits a
//   little harder than the one before, and every fifth is a finisher (Flow).
// - What a landed blow feels like. The game's time stops for a moment
//   (HitStop); the burst at the fist (web_visual.hpp), the controller's kick
//   and the sound (impact_sound.hpp) are the headset's (game_xr.cpp).
//
// Engine independent, like punch.hpp, which it builds on: game_punch.cpp
// runs it in the game.
namespace spidy {
// One enemy the player can fight.
struct FightTarget {
    std::uint64_t id{};
    Vec3 feet{}; // his base (a flyer's: its underside)
    float height = 1.8f, radius = .3f;
    // Off his feet in a flight (launched, flung, thrown): a blow juggles him.
    bool airborne{};
    Vec3 middle() const {
        return feet + Vec3{0, height * .5f, 0};
    }
};
// One hand in one input sample.
struct FightFist {
    bool tracked{};
    // The fist in the world, and the arm's own motion (Punches::velocity:
    // relative to the player, in the world's axes).
    Vec3 position{}, velocity{};
    // free: no web of this hand's holds anything or carries the player.
    // holds: the enemy this hand's web holds (0: none; such a hand is not
    // free, and strikes him alone).
    bool free{};
    std::uint64_t holds{};
    // The hand's web button, 0..1 (HandInput::grip).
    float web{};
};
struct WebStrikeConfig {
    // A strike opens with a fist at least this fast relative to the player
    // (a punch lands from 2.2 m/s, PunchConfig), going out from the player:
    // the cosine between its motion and the line to the fist from the base
    // of the neck, `neck` under the head. A hand drawn back and a hook
    // across the body open none; a fist thrown up from the guard at a thug
    // overhead does.
    float minSpeed = 2.6f, outward = .3f, neck = .25f;
    // For this long without a break: a twitch of the tracking is no punch.
    float sustain = .04f;
    // A free hand whose web button went down this recently is shooting a
    // web, not punching: an arm thrown out to swing away from a fight opens
    // no strike back into it. (A web that caught a thug makes the hand his:
    // the same throw then strikes him.)
    float pressSeconds = .3f;
    // A strike whose player takes no step for this long ends (a perch, a
    // ledge the game holds him on): no flight out of nowhere, seconds later.
    float stall = .3f;
    // A flight that goes up more than `liftOff` from where the player
    // stands leaves the ground by the game's own jump first (the game keeps
    // a standing body on its ground, whatever velocity it is given: the
    // swing's takeoff, October 4). The flight's clock waits for that jump
    // (wait()), for maxWait at most.
    float liftOff = .6f, maxWait = .7f;
    // Thrown at an enemy: the cosine between its motion and the line from
    // the fist to his middle (37 degrees), wider for the one enemy this
    // hand's web holds. Of several, the one it points at best.
    float aim = .8f, heldAim = .4f;
    // He is this far from the player's chest at least (nearer, the fist
    // reaches him by itself) and at most, with nothing fixed in between.
    float minDistance = 1.3f, maxDistance = 10, chest = 1.2f;
    // A hand whose web let go of something (a throw's follow-through, the
    // end of a swing) opens none for this long; one that opened a strike or
    // landed a punch opens the next once it slowed below rearmSpeed.
    float freeSeconds = .35f, rearmSpeed = 1.2f;
    // The player stops this far from his axis (wider for a wider enemy than
    // a street thug), feet `middle` below his middle: on the ground beside
    // a thug on his feet, at chest height beside one in the air.
    float standOff = .95f, middle = .9f;
    // The flight: at `speed` (real time, however slow the world runs), but
    // no shorter than minSeconds: a step to a thug two metres off is no
    // jolt. The player arrives within `arrive` of where he stops.
    float speed = 26, minSeconds = .12f, arrive = .08f;
    // A flight that has not arrived `grace` after it should have (a railing,
    // a wall's corner) ends; its blow still lands on a target within `reach`
    // of the chest.
    float grace = .25f, reach = 1.8f;
    // After a blow landed off the ground the player hangs for `hang`; each
    // further blow there starts it again, for maxHang in all between two
    // landings.
    float hang = .45f, maxHang = 3;
    // The blow a strike lands is the opening fist's, no weaker than this
    // (PunchEvent::strength): the flight is behind it.
    float minStrength = .5f;
    // From a flight's end to the next one opening.
    float pause = .08f;
};
class WebStrike {
  public:
    enum class Phase : std::uint8_t { Idle, Flying, Hanging };
    struct Opening {
        int hand{};
        std::uint64_t target{};
        float distance{}; // from the chest to his middle, metres
        bool held{};      // the hand's own web held him
    };
    // One step of the player's body.
    struct Step {
        bool drive{};            // the body is the strike's for this step
        Vec3 velocity{};         // at this velocity, real m/s
        bool landed{}, missed{}; // the flight ended: its blow lands, or it does not
    };
    explicit WebStrike(WebStrikeConfig config = {});
    // One input sample, `seconds` (real) after the previous one. head and
    // feet: the player's, in the world. Opens a strike when a fist asks for
    // one: the flight starts with the next step().
    std::optional<Opening> open(float seconds, const std::array<FightFist, 2>& fists, Vec3 head, Vec3 feet,
                                std::span<const FightTarget> targets, const WorldQueries& world);
    // One step of the player's body, `seconds` (real) long, from `feet`;
    // target: the strike's, where he is now (nullptr: gone).
    Step step(float seconds, Vec3 feet, bool grounded, const FightTarget* target);
    // The flight in progress goes up from `feet` by more than liftOff: a
    // player on the ground must jump first.
    bool lifting(Vec3 feet) const {
        return phase_ == Phase::Flying && destination_.y - feet.y > config_.liftOff;
    }
    // Instead of step(): the body cannot take the flight's step yet (the
    // game's jump is taking it off the ground). `seconds` real.
    void wait(float seconds);
    // A blow landed while the player or the one it hit was off the ground:
    // the player hangs (WebStrikeConfig::hang).
    void hangOn(bool grounded);
    // This hand landed a punch: its swing opens no strike.
    void spent(int hand);
    // Idle at once, with no blow.
    void cancel();
    Phase phase() const {
        return phase_;
    }
    int hand() const {
        return hand_;
    }
    std::uint64_t target() const {
        return target_;
    }
    // Where the flight in progress stops (the player's feet), as of its latest step.
    Vec3 destination() const {
        return destination_;
    }
    // Seconds the flight took, or has taken so far.
    float elapsed() const {
        return elapsed_;
    }
    // The opening fist's motion (unit) and speed: the blow the flight lands.
    Vec3 direction() const {
        return direction_;
    }
    float fistSpeed() const {
        return fistSpeed_;
    }
    const WebStrikeConfig& config() const {
        return config_;
    }

  private:
    Vec3 stop(Vec3 feet, const FightTarget& target);
    WebStrikeConfig config_;
    Phase phase_{};
    int hand_{};
    std::uint64_t target_{};
    Vec3 destination_{}, direction_{}, away_{0, 0, 1};
    float fistSpeed_{}, elapsed_{}, duration_{}, hangLeft_{}, hung_{}, sinceEnd_ = 100, sinceStep_{}, waited_{};
    std::array<float, 2> freeFor_{}, thrownFor_{}, sincePress_{100, 100};
    std::array<bool, 2> armed_{true, true}, pressed_{};
};
// The fist speed whose blow has this strength (PunchEvent::strength).
float speedOf(const PunchConfig& config, float strength);

struct FlowConfig {
    // Real seconds from one blow to the next for the flow to go on.
    float window = 3.5f;
    // Every blow whose count this divides is a finisher.
    int finisher = 5;
    // Each blow of a flow hits this much harder than its first, up to `most`
    // times as hard.
    float gain = .1f, most = 1.5f;
};
class Flow {
  public:
    explicit Flow(FlowConfig config = {});
    void update(float seconds); // real
    // A blow landed: the count it brings the flow to.
    int hit();
    void reset();
    int count() const {
        return count_;
    }
    bool finisher(int count) const {
        return count > 0 && count % config_.finisher == 0;
    }
    // How much harder than a first blow the blow with this count hits.
    float damage(int count) const;

  private:
    FlowConfig config_;
    int count_{};
    float since_{};
};

// What a landed blow is, for how it is delivered and how it feels.
enum class BlowKind : std::uint8_t {
    Hit,      // the game's melee, as hard as the fist was
    Heavy,    // a hard one: he flies off along it
    Launch,   // an uppercut: off his feet and up
    Juggle,   // at a thug in the air: he stays up
    Slam,     // down at a thug in the air: into the ground
    Finisher, // the flow's: he flies far
};
struct DeliveryConfig {
    // The strength from which a blow sends him flying (punch.hpp's FlyBack),
    // how fast along it at that strength and at full strength, and how fast up.
    float heavy = .85f, heavySpeed = 9, heavyFull = 14, heavyLift = 3;
    // An uppercut's launch: up, and a little along the blow. He peaks about
    // 3.5 m up, three quarters of a second later.
    float launchLift = 8.5f, launchCarry = 1.5f;
    // A blow at a thug in the air keeps him there.
    float juggleLift = 4.5f, juggleCarry = 3;
    // One that goes down this steeply (the sine of its pitch) slams him.
    float slamPitch = .5f, slamSpeed = 24, slamCarry = 2, slamDamage = 1.6f;
    float finisherDamage = 2.5f, finisherSpeed = 18, finisherLift = 5;
};
struct Delivery {
    BlowKind kind{};
    float damage{};
    // The game's kinetic blow (DamageType 7 with kFlyBack and an impulse,
    // which it answers with BotStateFlung) instead of its melee.
    bool kinetic{};
    Knockback knockback{};
    // His flight gets this velocity (real m/s), once he is on it.
    bool fly{};
    Vec3 velocity{};
};
// How the blow `blow` goes to the game: the `count`th of its flow, at a
// target on his feet or in the air.
Delivery deliver(const PunchEvent& blow, bool targetAirborne, int count, const Flow& flow,
                 const DeliveryConfig& config = {});

// The game's time around a landed blow: it all but stops for a moment, then
// eases back, as fighting games freeze on a hit. The head and the hands keep
// real time (game_time.hpp slows the game).
struct HitStopTuning {
    // The game's time while stopped, per real second.
    float scale = .1f;
    // Real seconds stopped: for the lightest blow, and more for a full one;
    // and for a finisher, at its own scale.
    float hold = .03f, holdGain = .07f, finisherHold = .4f, finisherScale = .2f;
    // Real seconds easing back.
    float ease = .14f;
};
class HitStop {
  public:
    explicit HitStop(const HitStopTuning& tuning = {});
    // A blow of this strength (0..1) landed.
    void hit(float strength, bool finisher);
    void update(float seconds); // real
    // The game's time per real second now: 1, down to the tuning's scale.
    float timeScale() const;
    bool active() const {
        return since_ < hold_ + tuning_.ease;
    }
    void reset();

  private:
    HitStopTuning tuning_;
    float since_ = 1e9f, hold_{}, scale_ = 1;
};
} // namespace spidy

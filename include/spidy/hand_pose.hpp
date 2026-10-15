#pragma once
#include <algorithm>
#include <cmath>

// What a hand's fingers do in VR (body_ik's Targets::Hand): a fist, or
// Spider-Man's web-shooting hand, the two middle fingers on the web shooter's
// trigger in the palm with the first finger, the little one and the thumb
// out. The web button brings the two fingers down as it goes down, and the
// hand is the shooting hand for as long as the button is held: the web leaves
// the wrist, the fingers keep the trigger down. A web ball makes the pose for
// a moment. A hand that takes hold of the other hand's web grips it: a fist.
// A fast hand is a fist too, as before, but stays the shooting hand while it
// shoots, unless there is somebody to punch.
// Until October 12 a squeezed grip was a fist and nothing else, so every web
// left a clenched hand; and the first build with the pose kept it 0.3 s
// ("the web shooting pose turns almost immidiatly to a fist").
namespace spidy {
struct HandPoseInput {
    bool tracked{};
    // The web button, 0..1 (Input's grip: the controller's grip, or its
    // trigger when the player swapped them). Past `press` it shoots.
    float web{};
    // The controller's own grip, 0..1: a clenched hand.
    float squeeze{};
    // The hand's speed against the head, m/s.
    float speed{};
    // It is on the other hand's web, about to take hold of it or holding it:
    // its button grips, and shoots nothing.
    bool gripping{};
    // A web ball left it since the last frame.
    bool shot{};
    // An enemy is marked or within a fist's reach: a fast hand is a punch.
    bool fighting{};
};
struct HandPose {
    float fist{}, shoot{}; // each 0..1; the shooting pose goes over the fist
};
struct HandPoseTuning {
    // A hand faster than slowSpeed closes, to a fist at fistSpeed (m/s), and
    // opens again at `open` per second.
    float slowSpeed = 1.2f, fistSpeed = 2.5f, open = 3;
    // The web button shoots past `press` and has let go below `release`, as
    // the swing has it (Swing::inputs).
    float press = .65f, release = .35f;
    // A web ball's pose stays whole this long; the pose comes at `in` per
    // second and goes at `out`.
    float ballSeconds = .5f, in = 20, out = 5;
};
class HandPoser {
  public:
    explicit HandPoser(const HandPoseTuning& tuning = {}) : t_(tuning) {}
    // One headset frame, `seconds` after the last.
    HandPose update(float seconds, const HandPoseInput& in) {
        if (!in.tracked) {
            fist_ = shoot_ = ball_ = 0;
            pressed_ = false;
            return {};
        }
        const float dt = std::isfinite(seconds) ? std::clamp(seconds, 0.f, .1f) : 0.f;
        const auto share = [](float v) { return std::isfinite(v) ? std::clamp(v, 0.f, 1.f) : 0.f; };
        const float web = share(in.web), squeeze = share(in.squeeze);
        const float punch = share((in.speed - t_.slowSpeed) / (t_.fistSpeed - t_.slowSpeed));
        pressed_ = pressed_ ? web > t_.release : web > t_.press;
        ball_ = in.shot ? t_.ballSeconds : std::max(0.f, ball_ - dt);
        // The button held, or on its way down; a web ball's moment. Not a
        // hand on the other hand's web.
        const float wanted = in.gripping ? 0.f : pressed_ || ball_ > 0 ? 1.f : std::min(web / t_.press, 1.f);
        shoot_ = wanted > shoot_ ? std::min(wanted, shoot_ + t_.in * dt) : std::max(wanted, shoot_ - t_.out * dt);
        // A clenched or a fast hand, and one that grips a web (with the
        // trigger as the web button the grip itself is open).
        const float closed = std::max({squeeze, punch, in.gripping ? web : 0.f});
        fist_ = std::max(closed, fist_ - t_.open * dt);
        return {fist_, shoot_ * (in.fighting ? 1 - punch : 1.f)};
    }

  private:
    HandPoseTuning t_;
    float fist_{}, shoot_{}, ball_{};
    bool pressed_{};
};
} // namespace spidy

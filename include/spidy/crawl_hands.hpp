#pragma once
#include "swing.hpp"

namespace spidy {
// Hand pulls for the game's supported crawl. Produces stick intent only:
// the native crawl keeps ownership of collision, vent turns and interactions.
class CrawlHands {
  public:
    void update(bool active, float seconds, const Input& in, Vec3 normal, Vec3 inside,
                const WorldQueries& world, unsigned webHands = 0) {
        contacts_ = 0;
        const bool valid = active && in.focused && !in.jump && finite(normal) &&
                           length(normal) > .9f && finite(inside) && finite(in.move) &&
                           length(in.move) <= .2f && std::isfinite(seconds) && seconds >= 0 && seconds <= .1f;
        if (!valid) {
            reset();
            return;
        }
        if (seconds == 0)
            return; // Same headset sample: keep its intent, never count its pull twice.
        if (!active_) {
            active_ = true;
            for (auto& h : hands_) h.held = true; // release once after entering native crawl
        }
        Vec3 pull{};
        unsigned count{};
        for (unsigned i = 0; i < 2; ++i) {
            auto& state = hands_[i];
            const auto& hand = in.hands[i];
            const bool tracked = hand.tracked && finite(hand.wallPosition) && finite(hand.gripRelativeToHead) &&
                                 std::isfinite(hand.wallGrip) && !(webHands & (1u << i));
            const bool held = tracked && hand.wallGrip > (state.held ? .35f : .65f);
            const bool press = held && !state.held;
            state.held = tracked ? held : true;
            if (!held || (state.attached && !world.exists(state.surface))) state.attached = false;
            if (press) {
                if (const auto hit = contact(hand.wallPosition, inside, world)) {
                    state.attached = true;
                    state.surface = hit->surface;
                    state.previous = hand.gripRelativeToHead;
                    contacts_ |= 1u << i;
                }
            }
            if (state.attached) {
                Vec3 rate = limited(trackingTurn(in).rotate(state.previous - hand.gripRelativeToHead) / seconds, 3.f);
                const Vec3 up = normalized(normal);
                rate -= up * dot(rate, up);
                pull += rate;
                ++count;
                state.previous = hand.gripRelativeToHead;
            }
        }
        // The native vent crawl runs at about 2.6 m/s. Two hands share one
        // movement; pulling into the supporting floor never drives through it.
        move_ = count ? limited(pull / (2.6f * static_cast<float>(count)), 1.f) : Vec3{};
    }
    unsigned hands() const { return (hands_[0].attached ? 1u : 0u) | (hands_[1].attached ? 2u : 0u); }
    unsigned contacts() const { return contacts_; }
    Vec3 move() const { return move_; }
    void reset() { *this = {}; }
  private:
    static std::optional<RayHit> contact(Vec3 palm, Vec3 inside, const WorldQueries& world) {
        std::optional<RayHit> nearest;
        float best = 2.f;
        for (const Vec3 axis : {Vec3{1, 0, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0},
                               Vec3{0, -1, 0}, Vec3{0, 0, 1}, Vec3{0, 0, -1}}) {
            // Cast from the duct's interior, aligned with the palm. This also
            // finds palms already inside a wall, floor or ceiling.
            const Vec3 from = palm + axis * dot(inside - palm, axis);
            const auto hit = world.raycast(from, -axis, 3.f);
            if (!hit || !hit->fixed || !world.exists(hit->surface) || !finite(hit->point) ||
                !finite(hit->normal) || dot(normalized(hit->normal), axis) < .8f) continue;
            const float distance = dot(palm - hit->point, normalized(hit->normal));
            if (distance <= .18f && distance >= -1.2f && std::abs(distance) < best) {
                best = std::abs(distance);
                nearest = hit;
            }
        }
        return nearest;
    }
    struct Hand { bool held{}, attached{}; uint64_t surface{}; Vec3 previous{}; } hands_[2];
    bool active_{};
    unsigned contacts_{};
    Vec3 move_{};
};
} // namespace spidy

#pragma once
#include "swing.hpp"

namespace spidy {
// Native floor crawl also appears briefly when cresting an outdoor wall.
// A nearby opposing surface identifies a duct immediately; a rayless scripted
// floor must persist for 0.4 s. Keep the mode through its supported shafts.
class VentCrawlMode {
  public:
    bool update(bool floor, bool wall, bool enclosed, bool standing, float seconds) {
        const float dt = std::isfinite(seconds) ? std::clamp(seconds, 0.f, .1f) : 0.f;
        if (!floor && !wall) {
            gap_ += dt;
            if (standing || gap_ > .25f) reset();
            return false;
        }
        gap_ = 0;
        floorSeconds_ = floor ? floorSeconds_ + dt : 0.f;
        active_ |= enclosed || (floor && floorSeconds_ >= .4f);
        return active_;
    }
    bool active() const { return active_; }
    void reset() { *this = {}; }
  private:
    float floorSeconds_{}, gap_{};
    bool active_{};
};
// Hand pulls for the game's supported crawl. Produces stick intent only:
// the native crawl keeps ownership of collision, vent turns and interactions.
class CrawlHands {
  public:
    void update(bool active, float seconds, const Input& in, Vec3 normal, Vec3 inside,
                const WorldQueries& world, unsigned webHands = 0, Vec3 supportPoint = {}) {
        contacts_ = 0;
        const bool valid = active && in.focused && !in.jump && finite(normal) &&
                           length(normal) > .9f && finite(inside) && finite(in.move) &&
                           std::isfinite(seconds) && seconds >= 0 && seconds <= .1f;
        if (!valid) {
            reset();
            return;
        }
        if (seconds == 0)
            return; // Same headset sample: keep its intent, never count its pull twice.
        Vec3 pull{};
        unsigned count{};
        for (unsigned i = 0; i < 2; ++i) {
            auto& state = hands_[i];
            const auto& hand = in.hands[i];
            const bool tracked = hand.tracked && finite(hand.wallPosition) &&
                                 finite(hand.gripRelativeToHead) && std::isfinite(hand.wallGrip) &&
                                 !(webHands & (1u << i));
            const bool held = tracked && hand.wallGrip > (state.held ? .35f : .65f);
            state.held = tracked ? held : true;
            if (!held || (state.attached && state.surface && !world.exists(state.surface)))
                state.attached = false;
            // Keep trying while squeezed: entering the duct or touching its
            // floor after pressing must work without another release/press.
            if (held && !state.attached) {
                if (const auto hit = contact(hand.wallPosition, inside, world, normal, supportPoint)) {
                    state.attached = true;
                    state.surface = hit->surface;
                    state.normal = normalized(hit->normal);
                    state.previous = hand.gripRelativeToHead;
                    contacts_ |= 1u << i;
                }
            }
            if (state.attached) {
                if (in.wallRotate || length(in.move) > .2f) {
                    state.previous = hand.gripRelativeToHead;
                    continue; // deliberate camera roll cannot pull the body
                }
                Vec3 rate = trackingTurn(in).rotate(state.previous - hand.gripRelativeToHead) / seconds;
                const Vec3 up = normalized(normal);
                // A hand can reach the next wall while the native actor is
                // still on the floor. Lay that wall's pull into the current
                // crawl plane, so it approaches the bend and the game turns
                // onto it, rather than discarding the upward pull entirely.
                rate -= state.normal * dot(rate, state.normal);
                rate = arc(state.normal, up).rotate(rate);
                rate -= up * dot(rate, up);
                pull += limited(rate, 3.f);
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
    static std::optional<RayHit> contact(Vec3 palm, Vec3 inside, const WorldQueries& world,
                                       Vec3 supportNormal, Vec3 supportPoint) {
        if (length(palm - inside) > 2.2f) return {};
        // Scripted floors can omit world-ray collision. Native support is
        // authoritative there; check it first and avoid unnecessary queries.
        const Vec3 n = normalized(supportNormal);
        const float depth = dot(palm - supportPoint, n);
        const Vec3 along = palm - supportPoint - n * depth;
        if (finite(supportPoint) && depth <= .22f && depth >= -1.2f && length(along) < 1.8f)
            return RayHit{palm - n * depth, n, 0, true};
        std::optional<RayHit> nearest;
        float best = 2.f;
        for (const Vec3 axis : {Vec3{1, 0, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0},
                               Vec3{0, -1, 0}, Vec3{0, 0, 1}, Vec3{0, 0, -1}}) {
            // Cast from the duct's interior, aligned with the palm. This also
            // finds palms already inside a wall, floor or ceiling.
            const Vec3 from = palm + axis * dot(inside - palm, axis);
            const auto hit = world.raycast(from, -axis, 3.f);
            if (!hit || !hit->fixed || !world.exists(hit->surface) || !finite(hit->point) ||
                !finite(hit->normal) || dot(normalized(hit->normal), axis) < .5f) continue;
            const float distance = dot(palm - hit->point, normalized(hit->normal));
            if (distance <= .18f && distance >= -1.2f && std::abs(distance) < best) {
                best = std::abs(distance);
                nearest = hit;
            }
        }
        return nearest;
    }
    struct Hand { bool held{}, attached{}; uint64_t surface{}; Vec3 normal{}, previous{}; } hands_[2];
    unsigned contacts_{};
    Vec3 move_{};
};
} // namespace spidy

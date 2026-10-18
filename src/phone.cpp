#include "spidy/phone.hpp"
#include "spidy/overlay_text.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace spidy {
namespace {
constexpr float pi = 3.14159265358979f;
// Sizes in metres and colours (linear RGB), as the other panels have them.
constexpr float width = .085f, height = .17f, corner = .012f, bezel = .005f;
// In the FNSM app's colours (Spider-Man 2's phone): near-black, cards a shade
// up, one bold red for what is picked and for the bar along the top.
constexpr Vec3 body{.012f, .012f, .016f}, screen{.008f, .008f, .01f}, white{1, 1, 1}, soft{.55f, .55f, .6f},
    dim{.055f, .055f, .065f}, amber{.85f, .04f, .05f}, red{.62f, .02f, .03f};
struct Plane {
    WheelFrame f;
    float scale = 1;
    Vec3 at(float x, float y, float z = 0) const {
        return f.centre + f.right * (x * scale) + f.up * (y * scale) + f.out * z;
    }
    void triangle(std::vector<Vertex>& v, float x0, float y0, float x1, float y1, float x2, float y2, float z,
                  Vec3 color) const {
        v.insert(v.end(), {Vertex{at(x0, y0, z), color}, Vertex{at(x1, y1, z), color}, Vertex{at(x2, y2, z), color}});
    }
    void rect(std::vector<Vertex>& v, float x, float y, float w, float h, float z, Vec3 color) const {
        triangle(v, x - w / 2, y - h / 2, x + w / 2, y - h / 2, x + w / 2, y + h / 2, z, color);
        triangle(v, x - w / 2, y - h / 2, x + w / 2, y + h / 2, x - w / 2, y + h / 2, z, color);
    }
    void disc(std::vector<Vertex>& v, float x, float y, float radius, float z, Vec3 color) const {
        constexpr int segments = 24;
        for (int i = 0; i < segments; ++i) {
            const float a0 = 2 * pi * i / segments, a1 = 2 * pi * (i + 1) / segments;
            triangle(v, x, y, x + radius * std::sin(a0), y + radius * std::cos(a0), x + radius * std::sin(a1),
                     y + radius * std::cos(a1), z, color);
        }
    }
    void ring(std::vector<Vertex>& v, float x, float y, float inner, float outer, float z, Vec3 color) const {
        constexpr int segments = 32;
        for (int i = 0; i < segments; ++i) {
            const float a0 = 2 * pi * i / segments, a1 = 2 * pi * (i + 1) / segments;
            const float s0 = std::sin(a0), c0 = std::cos(a0), s1 = std::sin(a1), c1 = std::cos(a1);
            triangle(v, x + inner * s0, y + inner * c0, x + outer * s0, y + outer * c0, x + outer * s1, y + outer * c1,
                     z, color);
            triangle(v, x + inner * s0, y + inner * c0, x + outer * s1, y + outer * c1, x + inner * s1, y + inner * c1,
                     z, color);
        }
    }
    // A rectangle with round corners of radius r.
    void rounded(std::vector<Vertex>& v, float x, float y, float w, float h, float r, float z, Vec3 color) const {
        rect(v, x, y, w - 2 * r, h, z, color);
        rect(v, x - w / 2 + r / 2, y, r, h - 2 * r, z, color);
        rect(v, x + w / 2 - r / 2, y, r, h - 2 * r, z, color);
        for (const float sx : {-1.f, 1.f})
            for (const float sy : {-1.f, 1.f})
                disc(v, x + sx * (w / 2 - r), y + sy * (h / 2 - r), r, z, color);
    }
    // Text on its baseline at (x, y), shrunk to `w`: `align` 0 from x, .5
    // centred on it, 1 ending at it.
    void text(std::vector<Vertex>& v, const std::string& s, float x, float y, float h, float w, Vec3 color,
              float align = .5f) const {
        TextStyle style;
        style.height = fittedHeight(s, h * scale, w * scale);
        style.color = color;
        appendText(v, s, at(x, y, .004f) - f.right * (textWidth(s, style.height) * align), f.right, f.up, style);
    }
};
// A button's symbol, white on its disc: previous and next two triangles,
// play one, pause two bars.
void symbol(std::vector<Vertex>& v, const Plane& p, PhoneButton b, bool playing, float x, float y, Vec3 color,
            float s = .0045f) {
    constexpr float z = .0035f;
    switch (b) {
    case PhoneButton::previous:
        p.triangle(v, x, y, x + 2 * s, y + s * 1.2f, x + 2 * s, y - s * 1.2f, z, color);
        p.triangle(v, x - 2 * s, y, x, y + s * 1.2f, x, y - s * 1.2f, z, color);
        break;
    case PhoneButton::next:
        p.triangle(v, x, y, x - 2 * s, y - s * 1.2f, x - 2 * s, y + s * 1.2f, z, color);
        p.triangle(v, x + 2 * s, y, x, y - s * 1.2f, x, y + s * 1.2f, z, color);
        break;
    case PhoneButton::play:
        if (playing) {
            p.rect(v, x - s * .7f, y, s * .8f, s * 2.6f, z, color);
            p.rect(v, x + s * .7f, y, s * .8f, s * 2.6f, z, color);
        } else {
            p.triangle(v, x + s * 1.6f, y, x - s, y - s * 1.5f, x - s, y + s * 1.5f, z, color);
        }
        break;
    default:
        break;
    }
}
// A spider, its body at (x, y) about `s` across: two discs and eight legs.
void spider(std::vector<Vertex>& v, const Plane& p, float x, float y, float s, Vec3 c) {
    constexpr float z = .0035f;
    p.disc(v, x, y + s * .22f, s * .16f, z, c);
    p.disc(v, x, y - s * .08f, s * .24f, z, c);
    for (const float side : {-1.f, 1.f})
        for (int k = 0; k < 4; ++k) {
            // Each leg out from the body, then bent down or up at its knee.
            const float a = (k - 1.5f) * .55f, kx = x + side * s * .45f, ky = y + std::sin(a) * s * .35f;
            const float fx = x + side * s * .62f, fy = ky + (k < 2 ? -1 : 1) * s * .28f;
            p.triangle(v, x, y, kx, ky + s * .03f, kx, ky - s * .03f, z, c);
            p.triangle(v, kx, ky + s * .03f, kx, ky - s * .03f, fx, fy, z, c);
        }
}
// The apps' icons, about `s` across, centred on (x, y).
void noteIcon(std::vector<Vertex>& v, const Plane& p, float x, float y, float s, Vec3 c) {
    constexpr float z = .0035f;
    p.disc(v, x - s * .18f, y - s * .25f, s * .2f, z, c);
    p.rect(v, x, y + s * .05f, s * .07f, s * .6f, z, c);
    p.triangle(v, x + s * .03f, y + s * .35f, x + s * .32f, y + s * .2f, x + s * .03f, y + s * .1f, z, c);
}
void pinIcon(std::vector<Vertex>& v, const Plane& p, float x, float y, float s, Vec3 c, Vec3 hole) {
    constexpr float z = .0035f;
    p.disc(v, x, y + s * .12f, s * .3f, z, c);
    p.triangle(v, x - s * .26f, y, x + s * .26f, y, x, y - s * .42f, z, c);
    p.disc(v, x, y + s * .12f, s * .12f, z + .0003f, hole);
}
void gearIcon(std::vector<Vertex>& v, const Plane& p, float x, float y, float s, Vec3 c, Vec3 hole) {
    constexpr float z = .0035f;
    p.disc(v, x, y, s * .3f, z, c);
    for (int k = 0; k < 8; ++k) {
        const float a = 2 * pi * k / 8;
        p.disc(v, x + std::sin(a) * s * .32f, y + std::cos(a) * s * .32f, s * .09f, z, c);
    }
    p.disc(v, x, y, s * .12f, z + .0003f, hole);
}
// The home screen: a card for each app, the picked one red; under them what plays.
void homeScreen(std::vector<Vertex>& v, const Plane& p, const PhoneView& view) {
    const char* const names[phoneApps] = {"MUSIC", "MAP", "GEAR"};
    const char* const notes[phoneApps] = {"YOUR SONGS", "HIDEOUTS", "GADGETS AND XP"};
    for (int i = 0; i < phoneApps; ++i) {
        const float y = .04f - i * .026f;
        const bool lit = view.app == i;
        p.rounded(v, 0, y, .068f, .021f, .004f, .002f, lit ? amber : dim);
        const Vec3 ink = white, back = lit ? amber : dim;
        p.rounded(v, -.0235f, y, .015f, .015f, .003f, .0025f, lit ? red : Vec3{.1f, .1f, .12f});
        if (i == 0)
            noteIcon(v, p, -.0235f, y, .012f, ink);
        else if (i == 1)
            pinIcon(v, p, -.0235f, y, .012f, ink, back);
        else
            gearIcon(v, p, -.0235f, y, .012f, ink, back);
        p.text(v, names[i], -.012f, y, .0045f, .04f, white, 0);
        p.text(v, notes[i], -.012f, y - .0058f, .0025f, .04f, lit ? white : soft, 0);
    }
    if (view.tracks > 0) {
        p.rounded(v, 0, -.05f, .068f, .016f, .004f, .002f, dim);
        p.text(v, view.playing ? "NOW PLAYING" : "PAUSED", -.03f, -.0465f, .0024f, .04f, amber, 0);
        p.text(v, view.song, -.03f, -.0545f, .0038f, .06f, white, 0);
    }
}
// MUSIC: the cover, the song and where it is, the volume, the buttons, AUTOPLAY.
void musicScreen(std::vector<Vertex>& v, const Plane& p, const PhoneView& view) {
    p.rounded(v, 0, .037f, .034f, .034f, .004f, .002f, red);
    noteIcon(v, p, 0, .037f, .026f, white);
    if (view.tracks <= 0) {
        p.text(v, "NO SONGS YET", 0, .008f, .0055f, .066f, white);
        p.text(v, "ADD SONGS TO THE MUSIC FOLDER", 0, -.002f, .003f, .066f, soft);
    } else {
        p.text(v, view.song, 0, .008f, .006f, .066f, white);
        char line[48];
        std::snprintf(line, sizeof line, "%d/%d  %s", view.track + 1, view.tracks, view.playing ? "PLAYING" : "PAUSED");
        p.text(v, line, 0, -.0025f, .0034f, .06f, soft);
        // The volume: a bar filling to it, VOL before it.
        const float barLeft = -.019f, barWidth = .05f, barY = -.012f;
        const float filled = barWidth * std::clamp(view.volume, 0, 100) / 100.f;
        p.text(v, "VOL", -.0255f, barY - .0014f, .0028f, .012f, soft);
        p.rect(v, barLeft + barWidth / 2, barY, barWidth, .0022f, .002f, dim);
        if (filled > 0)
            p.rect(v, barLeft + filled / 2, barY, filled, .0022f, .0025f, amber);
    }
    // PREVIOUS, PLAY and NEXT in a row, AUTOPLAY under them.
    const float row = -.033f, gap = .024f;
    const PhoneButton row3[] = {PhoneButton::previous, PhoneButton::play, PhoneButton::next};
    for (int i = 0; i < 3; ++i) {
        const float x = (i - 1) * gap, r = row3[i] == PhoneButton::play ? .011f : .0085f;
        const bool lit = view.lit == row3[i];
        p.disc(v, x, row, r, .002f, lit ? amber : dim);
        if (lit)
            p.ring(v, x, row, r, r + .0013f, .0025f, white);
        // Inside its disc: two triangles span four times this.
        symbol(v, p, row3[i], view.playing, x, row, white, r * (row3[i] == PhoneButton::play ? .4f : .32f));
    }
    const bool litAuto = view.lit == PhoneButton::autoplay;
    p.rounded(v, 0, -.055f, .056f, .01f, .005f, .002f, litAuto ? amber : dim);
    p.text(v, view.autoplay ? "AUTOPLAY ON" : "AUTOPLAY OFF", 0, -.0569f, .0038f, .046f, white);
}
// MAP: the hideouts and the game's waypoint to them, once Spidy knows how.
void mapScreen(std::vector<Vertex>& v, const Plane& p) {
    p.rounded(v, 0, .03f, .034f, .034f, .004f, .002f, dim);
    pinIcon(v, p, 0, .03f, .026f, amber, dim);
    p.text(v, "HIDEOUTS", 0, .0f, .006f, .066f, white);
    p.text(v, "TRACK THEM ON THE MAP", 0, -.011f, .0032f, .066f, soft);
    p.rounded(v, 0, -.035f, .062f, .014f, .004f, .002f, dim);
    p.text(v, "COMING SOON", 0, -.0368f, .004f, .05f, amber);
}
// GEAR: each gadget, the equipped one marked red, its charges; the level under them.
void gearScreen(std::vector<Vertex>& v, const Plane& p, const PhoneView& view) {
    if (view.gearCount <= 0) {
        p.text(v, "NO GADGETS YET", 0, .03f, .005f, .066f, soft);
    } else {
        for (int r = 0; r < PhoneUi::gearShown; ++r) {
            const int i = view.gearTop + r;
            if (i >= view.gearCount || i >= gadgetPlaces)
                break;
            const auto& g = view.gear[i];
            const float y = .049f - r * .0125f;
            p.rounded(v, 0, y, .068f, .0105f, .003f, .002f, dim);
            if (g.equipped)
                p.rect(v, -.0315f, y, .002f, .0075f, .0025f, amber);
            p.text(v, g.name, -.028f, y - .0017f, .0034f, .044f, g.equipped ? white : soft, 0);
            if (g.most > 0) {
                char count[16];
                std::snprintf(count, sizeof count, "%d/%d", g.ammo, g.most);
                p.text(v, count, .031f, y - .0017f, .0034f, .016f, white, 1);
            }
        }
        // More above or below: a small red arrow at that end.
        if (view.gearTop > 0)
            p.triangle(v, -.003f, .0565f, .003f, .0565f, 0, .0595f, .003f, amber);
        if (view.gearTop + PhoneUi::gearShown < view.gearCount)
            p.triangle(v, -.003f, -.0225f, .003f, -.0225f, 0, -.0255f, .003f, amber);
    }
    p.rounded(v, 0, -.046f, .068f, .03f, .004f, .002f, dim);
    p.text(v, "LEVEL", -.03f, -.038f, .003f, .03f, soft, 0);
    p.text(v, "--", -.03f, -.048f, .007f, .03f, white, 0);
    p.text(v, "XP", .005f, -.038f, .003f, .03f, soft, 0);
    p.text(v, "COMING SOON", .005f, -.047f, .0032f, .024f, amber, 0);
}
} // namespace

PhoneControls::Event PhoneControls::update(float stickX, float stickY, bool a, bool b, float leftTrigger,
                                           float rightTrigger) {
    Event e;
    // A trigger counts as it is pulled, once, until let go.
    const auto pull = [](float t, bool& held) {
        t = std::isfinite(t) ? t : 0.f;
        const bool was = held;
        held = t > (held ? released : pulled);
        return held && !was;
    };
    const bool down = pull(leftTrigger, leftHeld_), up = pull(rightTrigger, rightHeld_);
    e.volume = (up ? 1 : 0) - (down ? 1 : 0);
    if (!std::isfinite(stickX))
        stickX = 0;
    if (!std::isfinite(stickY))
        stickY = 0;
    // One step a push, along the way it is pushed most.
    if (std::max(std::abs(stickX), std::abs(stickY)) < centred)
        stickHeld_ = false;
    if (!stickHeld_ && std::max(std::abs(stickX), std::abs(stickY)) > pushed) {
        if (std::abs(stickX) >= std::abs(stickY))
            e.x = stickX > 0 ? 1 : -1;
        else
            e.y = stickY > 0 ? 1 : -1;
        stickHeld_ = true;
    }
    e.pressed = a && !aHeld_;
    e.closed = b && !bHeld_;
    aHeld_ = a;
    bHeld_ = b;
    return e;
}

PhoneAction PhoneUi::update(const PhoneControls::Event& e, int gearRows) {
    switch (screen_) {
    case PhoneScreen::home:
        if (e.closed)
            return PhoneAction::close;
        // The apps stand in a column: up and down, or left and right, go through them.
        if (const int step = e.y ? -e.y : e.x)
            app_ = (app_ + step + phoneApps) % phoneApps;
        if (e.pressed)
            screen_ = static_cast<PhoneScreen>(app_ + 1);
        return PhoneAction::none;
    case PhoneScreen::music:
        if (e.closed) {
            screen_ = PhoneScreen::home;
            return PhoneAction::none;
        }
        if (e.x)
            button_ = static_cast<PhoneButton>((static_cast<int>(button_) + e.x + phoneButtons) % phoneButtons);
        if (e.y < 0)
            button_ = PhoneButton::autoplay;
        else if (e.y > 0 && button_ == PhoneButton::autoplay)
            button_ = PhoneButton::play;
        if (!e.pressed)
            return PhoneAction::none;
        switch (button_) {
        case PhoneButton::previous:
            return PhoneAction::previous;
        case PhoneButton::play:
            return PhoneAction::play;
        case PhoneButton::next:
            return PhoneAction::next;
        case PhoneButton::autoplay:
            return PhoneAction::autoplay;
        }
        return PhoneAction::none;
    case PhoneScreen::gear:
        gearTop_ = std::clamp(gearTop_ - e.y, 0, std::max(0, gearRows - gearShown));
        [[fallthrough]];
    case PhoneScreen::map:
        if (e.closed)
            screen_ = PhoneScreen::home;
        return PhoneAction::none;
    }
    return PhoneAction::none;
}

WheelFrame phoneFrame(Pose head) {
    Vec3 ahead = head.orientation.rotate({0, 0, -1});
    ahead.y = 0;
    ahead = length(ahead) > .1f ? normalized(ahead) : Vec3{0, 0, -1};
    const Vec3 up{0, 1, 0}, out = ahead * -1.f;
    return {head.position + ahead * .4f + Vec3{0, -.22f, 0}, cross(up, out), up, out};
}

WheelFrame phoneFrameAt(Pose head, Vec3 hand) {
    if (!finite(hand) || !finite(head.position))
        return phoneFrame(head);
    const Vec3 centre = hand + Vec3{0, .07f, 0};
    const Vec3 toEyes = head.position - centre;
    if (length(toEyes) < .1f)
        return phoneFrame(head);
    const Vec3 out = normalized(toEyes);
    // Its top as near world up as facing the eyes leaves it.
    Vec3 up = Vec3{0, 1, 0} - out * out.y;
    if (length(up) < .1f)
        return phoneFrame(head);
    up = normalized(up);
    return {centre, cross(up, out), up, out};
}

bool PocketDraw::update(float seconds, bool play, const Pose& head, const Pose grips[2], const bool tracked[2],
                        const float squeezes[2]) {
    const float dt = std::isfinite(seconds) ? std::clamp(seconds, 0.f, .1f) : 0.f;
    Vec3 forward = head.orientation.rotate({0, 0, -1});
    forward.y = 0;
    forward = length(forward) > .1f ? normalized(forward) : Vec3{0, 0, -1};
    const Vec3 right = cross(forward, {0, 1, 0});
    bool out{};
    for (int i = 0; i < 2; ++i) {
        const float squeeze = std::isfinite(squeezes[i]) ? squeezes[i] : 0.f;
        const bool down = tracked[i] && squeeze > (down_[i] ? released : pressed);
        const bool press = down && !down_[i];
        down_[i] = down;
        const unsigned bit = 1u << i;
        if (!down) {
            hands_ &= ~bit;
            if (drawing_ == i)
                drawing_ = -1;
            continue;
        }
        if (!play || !finite(head.position) || !finite(grips[i].position))
            continue;
        // The grip in the head's level axes: x right, y up, z ahead.
        const Vec3 d = grips[i].position - head.position;
        const float x = dot(d, right), y = d.y, z = dot(d, forward);
        if (press && drawing_ < 0 && -y >= pocketAbove && -y <= pocketBelow && std::abs(x) >= pocketSide &&
            std::abs(x) <= pocketOut && z <= -behind) {
            drawing_ = i;
            time_ = 0;
            hands_ |= bit;
        }
        if (drawing_ == i) {
            time_ += dt;
            if (z >= front && -y <= raised) {
                out = true;
                hand_ = grips[i].position;
                drewWith_ = i;
                ++draws_;
                drawing_ = -1; // the grip stays the draw's until let go
            } else if (time_ > drawSeconds) {
                drawing_ = -1;
            }
        }
    }
    if (!play)
        drawing_ = -1;
    return out;
}

std::string phoneSongName(const std::string& fileName) {
    std::string name = fileName;
    if (const auto slash = name.find_last_of("\\/"); slash != std::string::npos)
        name.erase(0, slash + 1);
    if (const auto dot = name.find_last_of('.'); dot != std::string::npos && dot > 0)
        name.erase(dot);
    for (char& c : name)
        if (c == '_')
            c = ' ';
        else if (!drawable(c))
            c = ' ';
    // Runs of spaces as one, none at either end.
    std::string out;
    for (const char c : name)
        if (c != ' ' || (!out.empty() && out.back() != ' '))
            out += c;
    while (!out.empty() && out.back() == ' ')
        out.pop_back();
    return out.empty() ? "SONG" : out;
}

void appendPhone(std::vector<Vertex>& out, const PhoneView& view) {
    if (!view.open || !finite(view.frame.centre))
        return;
    // It grows out of its middle over an eighth of a second.
    const Plane p{view.frame, std::clamp(std::isfinite(view.age) ? view.age / .12f : 1.f, .2f, 1.f)};
    p.rounded(out, 0, 0, width, height, corner, 0, body);
    p.rounded(out, 0, 0, width - 2 * bezel, height - 2 * bezel, corner - bezel / 2, .001f, screen);
    // The red bar along the top, the screen's name in it; the controls' hint
    // along the bottom.
    const char* const titles[] = {"FNSM", "MUSIC", "MAP", "GEAR"};
    p.rounded(out, 0, .0695f, width - 2 * bezel, .019f, corner - bezel / 2, .0015f, amber);
    p.text(out, titles[static_cast<int>(view.screen) & 3], -.031f, .0655f, .0065f, .05f, white, 0);
    spider(out, p, .027f, .0695f, .006f, white);
    const char* hint = view.screen == PhoneScreen::home    ? "A OPEN   B PUT AWAY"
                       : view.screen == PhoneScreen::music ? "A PRESS   B HOME   TRIGGERS VOL"
                                                           : "B HOME";
    p.text(out, hint, 0, -.0755f, .0026f, .066f, soft);
    switch (view.screen) {
    case PhoneScreen::home:
        homeScreen(out, p, view);
        break;
    case PhoneScreen::music:
        musicScreen(out, p, view);
        break;
    case PhoneScreen::map:
        mapScreen(out, p);
        break;
    case PhoneScreen::gear:
        gearScreen(out, p, view);
        break;
    }
}
} // namespace spidy

// Draws the phone's screens (phone.hpp's appendPhone) flat, face on, side by
// side - home, MUSIC, MAP, GEAR - with sample songs and gadgets, as a BMP, to
// look at its layout without a headset.
//
//   spidy_phone_preview <out.bmp>
#include "spidy/phone.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <vector>
using namespace spidy;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::fprintf(stderr, "spidy_phone_preview <out.bmp>\n");
        return 1;
    }
    constexpr float pixels = 5000; // a metre
    constexpr int each = 520, w = each * 4, h = 960;
    std::vector<float> rgb(static_cast<size_t>(w) * h * 3, .03f);
    PhoneView view;
    view.open = true;
    view.age = 1;
    view.tracks = 42;
    view.track = 2;
    view.playing = true;
    view.volume = 70;
    std::snprintf(view.song, sizeof view.song, "%s", "SUNFLOWER");
    const char* names[] = {"WEB SHOOTER", "IMPACT WEB", "WEB BOMB", "ELECTRIC WEB", "TRIP MINE",
                           "CONCUSSION BLAST", "SPIDER-DRONE", "SUSPENSION MATRIX"};
    view.gearCount = 8;
    for (int i = 0; i < 8; ++i) {
        std::snprintf(view.gear[i].name, sizeof view.gear[i].name, "%s", names[i]);
        view.gear[i].most = i ? 4 + i % 3 : 0;
        view.gear[i].ammo = i ? 2 + i % 2 : 0;
        view.gear[i].equipped = i == 2;
    }
    const PhoneScreen screens[] = {PhoneScreen::home, PhoneScreen::music, PhoneScreen::map, PhoneScreen::gear};
    for (int s = 0; s < 4; ++s) {
        view.screen = screens[s];
        view.app = 0;
        view.lit = PhoneButton::play;
        view.gearTop = s == 3 ? 1 : 0;
        std::vector<Vertex> v;
        appendPhone(v, view);
        // Face on: x right, y up; deeper layers drawn later, as given.
        const float ox = each * (s + .5f), oy = h / 2.f;
        for (size_t t = 0; t + 2 < v.size(); t += 3) {
            float px[3], py[3];
            for (int k = 0; k < 3; ++k) {
                px[k] = ox + v[t + k].position.x * pixels;
                py[k] = oy - v[t + k].position.y * pixels;
            }
            const int x0 = std::max(0, static_cast<int>(std::floor(std::min({px[0], px[1], px[2]})))),
                      x1 = std::min(w - 1, static_cast<int>(std::ceil(std::max({px[0], px[1], px[2]})))),
                      y0 = std::max(0, static_cast<int>(std::floor(std::min({py[0], py[1], py[2]})))),
                      y1 = std::min(h - 1, static_cast<int>(std::ceil(std::max({py[0], py[1], py[2]}))));
            const float area = (px[1] - px[0]) * (py[2] - py[0]) - (px[2] - px[0]) * (py[1] - py[0]);
            if (std::abs(area) < 1e-6f)
                continue;
            for (int y = y0; y <= y1; ++y)
                for (int x = x0; x <= x1; ++x) {
                    const float cx = x + .5f, cy = y + .5f;
                    const float a = ((px[1] - cx) * (py[2] - cy) - (px[2] - cx) * (py[1] - cy)) / area,
                                b = ((px[2] - cx) * (py[0] - cy) - (px[0] - cx) * (py[2] - cy)) / area;
                    if (a < 0 || b < 0 || a + b > 1)
                        continue;
                    float* p = &rgb[(static_cast<size_t>(y) * w + x) * 3];
                    const auto& c = v[t].color;
                    const float al = std::clamp(v[t].alpha, 0.f, 1.f);
                    p[0] = p[0] * (1 - al) + c.x * al;
                    p[1] = p[1] * (1 - al) + c.y * al;
                    p[2] = p[2] * (1 - al) + c.z * al;
                }
        }
    }
    std::ofstream out(argv[1], std::ios::binary);
    const uint32_t row = (w * 3 + 3) & ~3u, size = 54 + row * h;
    const auto put = [&](uint32_t value, int bytes) {
        for (int b = 0; b < bytes; ++b)
            out.put(static_cast<char>(value >> (8 * b)));
    };
    out.write("BM", 2);
    put(size, 4);
    put(0, 4);
    put(54, 4);
    put(40, 4);
    put(w, 4);
    put(h, 4);
    put(1, 2);
    put(24, 2);
    put(0, 4);
    put(row * h, 4);
    put(2835, 4);
    put(2835, 4);
    put(0, 4);
    put(0, 4);
    const auto encode = [](float c) {
        c = std::clamp(c, 0.f, 1.f);
        return static_cast<char>(std::lround(255 * (c <= .0031308f ? c * 12.92f : 1.055f * std::pow(c, 1 / 2.4f) - .055f)));
    };
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            const float* p = &rgb[(static_cast<size_t>(y) * w + x) * 3];
            out.put(encode(p[2]));
            out.put(encode(p[1]));
            out.put(encode(p[0]));
        }
        for (uint32_t pad = w * 3; pad < row; ++pad)
            out.put(0);
    }
    return 0;
}

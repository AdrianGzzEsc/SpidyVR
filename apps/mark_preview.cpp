// Draws the combat marks (web_visual.hpp's appendCue, appendImpact and
// appendKnockout) over a
// headset's eye captures, without a headset: the overlay's own vertices, one
// sample a pixel with its blending, at about the headset's pixel density.
// It writes sheets of the marks' states and poses as BMP files.
// tools/preview-marks.ps1 feeds it captures and turns the sheets into PNG.
//
//   spidy_mark_preview <out folder> <scale> <far.bmp|-> <u> <v> <metres> <near.bmp|-> <u> <v> <metres>
//
// Each backdrop is a 24-bit BMP of an eye capture enlarged `scale` times
// (the captures are 512 pixels wide, about 235 pixels a unit of tangent),
// with the capture pixel (u, v) of the enemy's chest and how far off he is;
// "-" for a plain backdrop, dark to the left and pale to the right.
#include "spidy/overlay_text.hpp"
#include "spidy/web_visual.hpp"
#include "preview_image.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
using namespace spidy;
using namespace spidy::preview;

namespace {
Image plain(int w, int h) {
    Image image(w, h);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            const bool pale = x >= w / 2;
            float* p = image.at(x, y);
            p[0] = pale ? .55f : .012f;
            p[1] = pale ? .5f : .012f;
            p[2] = pale ? .42f : .02f;
        }
    return image;
}
void label(Image& image, const std::string& text, int x, int y) {
    constexpr float height = 15;
    const Camera flat{1, 0, 0};
    for (const bool shadow : {true, false}) {
        TextStyle style;
        style.height = height;
        style.weight = shadow ? .4f : .14f;
        style.color = shadow ? Vec3{0, 0, 0} : Vec3{1, .9f, .2f};
        std::vector<Vertex> v;
        appendText(v, text, {static_cast<float>(x), -static_cast<float>(y) - height, -1}, {1, 0, 0}, {0, 1, 0}, style);
        draw(image, v, flat, 0, 0, 0, 0, image.w, image.h);
    }
}
struct Cell {
    std::string name;
    std::vector<Vertex> vertices;
};
// A sheet of cells, each a crop of `backdrop` `side` pixels square about
// camera pixel (u, v), with its marks and its name.
bool sheet(const std::string& path, const Image& backdrop, const Camera& camera, float u, float v, int side,
           int columns, const std::vector<Cell>& cells) {
    constexpr int gap = 6;
    const int rows = (static_cast<int>(cells.size()) + columns - 1) / columns;
    Image image(columns * (side + gap) + gap, rows * (side + gap) + gap);
    const int u0 = static_cast<int>(u) - side / 2, v0 = static_cast<int>(v) - side / 2;
    for (size_t i = 0; i < cells.size(); ++i) {
        const int x0 = gap + static_cast<int>(i % columns) * (side + gap);
        const int y0 = gap + static_cast<int>(i / columns) * (side + gap);
        for (int y = 0; y < side; ++y)
            for (int x = 0; x < side; ++x) {
                const int su = u0 + x, sv = v0 + y;
                const bool inside = su >= 0 && sv >= 0 && su < backdrop.w && sv < backdrop.h;
                for (int k = 0; k < 3; ++k)
                    image.at(x0 + x, y0 + y)[k] = inside ? backdrop.at(su, sv)[k] : .01f;
            }
        draw(image, cells[i].vertices, camera, static_cast<float>(u0), static_cast<float>(v0), x0, y0, side, side);
        label(image, cells[i].name, x0 + 6, y0 + 6);
    }
    return save(image, path);
}
} // namespace

int main(int argc, char** argv) {
    if (argc != 11) {
        std::printf("spidy_mark_preview <out folder> <scale> <far.bmp|-> <u> <v> <metres> <near.bmp|-> <u> <v> <metres>\n");
        return 1;
    }
    const std::string out = argv[1];
    const float scale = std::clamp(static_cast<float>(std::atof(argv[2])), 1.f, 8.f);
    struct Scene {
        Image backdrop;
        float u{}, v{}, metres{};
    } scenes[2];
    for (int i = 0; i < 2; ++i) {
        auto& scene = scenes[i];
        const std::string path = argv[3 + i * 4];
        if (path == "-")
            scene.backdrop = plain(static_cast<int>(512 * scale), static_cast<int>(544 * scale));
        else if (!load(path, scene.backdrop)) {
            std::printf("%s is not a 24-bit BMP\n", path.c_str());
            return 2;
        }
        scene.u = static_cast<float>(std::atof(argv[4 + i * 4])) * scale;
        scene.v = static_cast<float>(std::atof(argv[5 + i * 4])) * scale;
        scene.metres = std::clamp(static_cast<float>(std::atof(argv[6 + i * 4])), .2f, 50.f);
    }
    const Vec3 viewer{};
    constexpr float pose = 1.f / 12;
    const auto at = [&](int i) { return pose * (static_cast<float>(i) + .5f); };
    bool ok = true;
    for (int i = 0; i < 2; ++i) {
        const auto& scene = scenes[i];
        const Camera camera{235 * scale, static_cast<float>(scene.backdrop.w) / 2, static_cast<float>(scene.backdrop.h) / 2};
        const float pixelAngle = 1 / camera.f;
        const Vec3 p = camera.ray(scene.u, scene.v) * scene.metres;
        const auto cue = [&](CueWay way, bool finisher, float left, float age, bool locked = false) {
            std::vector<Vertex> v;
            appendCue(v, {p, way, finisher, left, age, locked}, viewer, pixelAngle);
            return v;
        };
        const auto knockout = [&](float age) {
            std::vector<Vertex> v;
            appendKnockout(v, {p, age}, viewer, pixelAngle);
            return v;
        };
        const auto impact = [&](BlowKind kind, float strength, int count, float age, bool called = false) {
            std::vector<Vertex> v;
            appendImpact(v, {p, strength, kind, count, age, called}, viewer, pixelAngle);
            return v;
        };
        const std::string name = out + (i ? "/near-" : "/far-");
        // A cell holds the mark with room around it at either distance.
        const int side = static_cast<int>((i ? 150 : 120) * scale);
        ok &= sheet(name + "marks.bmp", scene.backdrop, camera, scene.u, scene.v, side, 5,
                    {{"LOOKED AT", cue(CueWay::None, false, 1, 1)},
                     {"LOCKED", cue(CueWay::None, false, 1, 1, true)},
                     {"CALL LEFT", cue(CueWay::Left, false, .85f, 1, true)},
                     {"CALL UP 50%", cue(CueWay::Up, false, .5f, 1.17f, true)},
                     {"FINISHER RIGHT", cue(CueWay::Right, true, .9f, 1, true)},
                     {"LOCKED POSE 1", cue(CueWay::None, false, 1, at(0), true)},
                     {"CALL DOWN 20%", cue(CueWay::Down, false, .2f, 1.34f, true)},
                     {"CALL POSE 1", cue(CueWay::Right, false, 1, at(0), true)},
                     {"CALL POSE 2", cue(CueWay::Right, false, 1, at(1), true)},
                     {"FINISHER POSE 2", cue(CueWay::Up, true, 1, at(1), true)}});
        std::vector<Cell> blows;
        for (int k = 0; k < 5; ++k)
            blows.push_back({"JAB X2 POSE " + std::to_string(k + 1), impact(BlowKind::Hit, .5f, 2, at(k))});
        for (int k = 0; k < 5; ++k)
            blows.push_back({"CALLED X3 POSE " + std::to_string(k + 1), impact(BlowKind::Heavy, 1, 3, at(k), true)});
        for (int k = 0; k < 5; ++k)
            blows.push_back({"FINISHER X5 POSE " + std::to_string(k + 1), impact(BlowKind::Finisher, 1, 5, at(k))});
        blows.push_back({"FIRST JAB POSE 2", impact(BlowKind::Hit, .2f, 1, at(1))});
        blows.push_back({"HEAVY POSE 2", impact(BlowKind::Heavy, 1, 1, at(1))});
        blows.push_back({"COUNT HELD", impact(BlowKind::Hit, .5f, 4, at(5))});
        blows.push_back({"COUNT LEAVING 1", impact(BlowKind::Hit, .5f, 4, at(7))});
        blows.push_back({"COUNT LEAVING 2", impact(BlowKind::Hit, .5f, 4, at(8))});
        blows.push_back({"KNOCKOUT POSE 1", knockout(at(0))});
        blows.push_back({"KNOCKOUT POSE 2", knockout(at(1))});
        blows.push_back({"KNOCKOUT HELD", knockout(at(4))});
        blows.push_back({"KNOCKOUT LEAVING 1", knockout(at(6))});
        blows.push_back({"KNOCKOUT LEAVING 2", knockout(at(7))});
        ok &= sheet(name + "blows.bmp", scene.backdrop, camera, scene.u, scene.v, side, 5, blows);
        // The whole eye image with a call on him.
        Image whole = scene.backdrop;
        draw(whole, cue(CueWay::Left, false, .7f, 1), camera, 0, 0, 0, 0, whole.w, whole.h);
        ok &= save(whole, name + "eye.bmp");
    }
    return ok ? 0 : 3;
}

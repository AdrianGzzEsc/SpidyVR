#pragma once
#include <string>
#include <vector>

// The phone's music (phone.hpp): the songs in the Music folder beside the
// launcher, played through Windows' own media player (MCI), on the thread
// that calls it. Every call comes from the XR worker's frame.
namespace spidy {
class PhoneMusic {
  public:
    ~PhoneMusic();
    // Looks for songs again (mp3, wav, m4a, wma, aac) in `folder`, unless one
    // is playing or paused, whose list stays.
    void load(const std::wstring& folder);
    int tracks() const {
        return static_cast<int>(files_.size());
    }
    int track() const {
        return track_;
    }
    bool playing() const {
        return playing_;
    }
    bool autoplay = true;
    int volume() const {
        return volume_;
    }
    // Percent, 0 to 100: the song playing at once, and every one after it.
    void setVolume(int percent);
    // The shown name of the current song ("" with none).
    std::string name() const;
    // Play from where it was paused, or pause.
    void toggle();
    void next();
    void previous();
    // Seen as the frame goes: a song that has ended starts the next one
    // (autoplay) or leaves the phone paused at its start.
    void poll(float seconds);
    void stop();

  private:
    bool start(int track);
    void close();
    std::vector<std::wstring> files_;
    std::vector<std::string> names_;
    int track_{}, volume_ = 100;
    bool playing_{}, opened_{};
    float sincePoll_{};
};
// The launcher's folder: the nearest one up from this module with the
// launcher in it (the game loads a copy of the module from
// reports\bridge-modules\<hash>, tools/bridge_game.py).
std::wstring spidyHome();
// The Music folder in it (made if missing).
std::wstring phoneMusicFolder();
} // namespace spidy

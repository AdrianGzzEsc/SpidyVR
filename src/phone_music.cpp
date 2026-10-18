#include "spidy/phone_music.hpp"
#include "spidy/phone.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#include <algorithm>
#include <cmath>
#include <cwctype>

namespace spidy {
namespace {
constexpr const wchar_t* alias = L"spidyphone";
bool mci(const std::wstring& command, std::wstring* reply = nullptr) {
    wchar_t buffer[128]{};
    const MCIERROR error = mciSendStringW(command.c_str(), buffer, static_cast<UINT>(std::size(buffer)), nullptr);
    if (reply)
        *reply = buffer;
    return error == 0;
}
bool isSong(const std::wstring& name) {
    const auto dot = name.find_last_of(L'.');
    if (dot == std::wstring::npos)
        return false;
    std::wstring ext = name.substr(dot + 1);
    for (auto& c : ext)
        c = static_cast<wchar_t>(std::towlower(c));
    return ext == L"mp3" || ext == L"wav" || ext == L"m4a" || ext == L"wma" || ext == L"aac";
}
std::string narrow(const std::wstring& w) {
    std::string s;
    for (const wchar_t c : w)
        s += c < 128 ? static_cast<char>(c) : ' ';
    return s;
}
} // namespace

PhoneMusic::~PhoneMusic() {
    close();
}

void PhoneMusic::load(const std::wstring& folder) {
    if (opened_)
        return;
    files_.clear();
    names_.clear();
    WIN32_FIND_DATAW found{};
    const HANDLE find = FindFirstFileW((folder + L"\\*").c_str(), &found);
    if (find == INVALID_HANDLE_VALUE)
        return;
    std::vector<std::wstring> names;
    do {
        if (!(found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && isSong(found.cFileName))
            names.push_back(found.cFileName);
    } while (FindNextFileW(find, &found));
    FindClose(find);
    std::sort(names.begin(), names.end(), [](const std::wstring& a, const std::wstring& b) {
        return _wcsicmp(a.c_str(), b.c_str()) < 0;
    });
    for (const auto& n : names) {
        files_.push_back(folder + L"\\" + n);
        names_.push_back(phoneSongName(narrow(n)));
    }
    track_ = std::clamp(track_, 0, std::max(0, tracks() - 1));
}

std::string PhoneMusic::name() const {
    return track_ < static_cast<int>(names_.size()) ? names_[static_cast<size_t>(track_)] : std::string{};
}

void PhoneMusic::close() {
    if (opened_)
        mci(std::wstring(L"close ") + alias);
    opened_ = playing_ = false;
}

bool PhoneMusic::start(int track) {
    close();
    if (files_.empty())
        return false;
    track_ = (track % tracks() + tracks()) % tracks();
    const std::wstring file = files_[static_cast<size_t>(track_)];
    if (!mci(L"open \"" + file + L"\" type mpegvideo alias " + alias))
        return false;
    opened_ = true;
    setVolume(volume_);
    playing_ = mci(std::wstring(L"play ") + alias);
    sincePoll_ = 0;
    return playing_;
}

void PhoneMusic::toggle() {
    if (!opened_) {
        start(track_);
    } else if (playing_) {
        playing_ = !mci(std::wstring(L"pause ") + alias);
    } else {
        playing_ = mci(std::wstring(L"resume ") + alias) || mci(std::wstring(L"play ") + alias);
    }
}

void PhoneMusic::setVolume(int percent) {
    volume_ = std::clamp(percent, 0, 100);
    // MCI's volume runs 0 to 1000.
    if (opened_)
        mci(std::wstring(L"setaudio ") + alias + L" volume to " + std::to_wstring(volume_ * 10));
}

void PhoneMusic::next() {
    if (!files_.empty())
        start(track_ + 1);
}

void PhoneMusic::previous() {
    if (!files_.empty())
        start(track_ - 1);
}

void PhoneMusic::poll(float seconds) {
    if (!opened_ || !playing_)
        return;
    sincePoll_ += std::isfinite(seconds) ? seconds : 0.f;
    if (sincePoll_ < .5f)
        return;
    sincePoll_ = 0;
    std::wstring mode;
    if (!mci(std::wstring(L"status ") + alias + L" mode", &mode) || mode != L"stopped")
        return;
    // The song ended.
    if (autoplay && tracks() > 1) {
        start(track_ + 1);
    } else {
        close();
    }
}

void PhoneMusic::stop() {
    close();
}

std::wstring phoneMusicFolder() {
    const std::wstring folder = spidyHome() + L"\\Music";
    CreateDirectoryW(folder.c_str(), nullptr);
    return folder;
}

std::wstring spidyHome() {
    HMODULE self{};
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCWSTR>(&spidyHome), &self);
    wchar_t path[MAX_PATH]{};
    GetModuleFileNameW(self, path, MAX_PATH);
    std::wstring folder = path, root;
    while (root.empty()) {
        const auto slash = folder.find_last_of(L"\\/");
        if (slash == std::wstring::npos)
            break;
        folder.erase(slash);
        for (const wchar_t* launcher : {L"\\Launch Spidy VR.cmd", L"\\Spidy Launcher.exe"})
            if (GetFileAttributesW((folder + launcher).c_str()) != INVALID_FILE_ATTRIBUTES)
                root = folder;
    }
    return root.empty() ? std::wstring(path).substr(0, std::wstring(path).find_last_of(L"\\/")) : root;
}
} // namespace spidy

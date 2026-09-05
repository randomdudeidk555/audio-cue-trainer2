#include "AudioCueBank.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <cmath>
#include <algorithm>

using namespace geode::prelude;

namespace act {

AudioCueBank& AudioCueBank::get() {
    static AudioCueBank instance;
    return instance;
}

namespace {
    constexpr int kSampleRate = 44100;
    constexpr double kDurationSeconds = 0.045;
    constexpr double kToneHz = 1400.0;

    template <typename T>
    void appendLE(std::vector<uint8_t>& buf, T value) {
        auto* p = reinterpret_cast<uint8_t*>(&value);
        buf.insert(buf.end(), p, p + sizeof(T));
    }
}

std::vector<uint8_t> AudioCueBank::buildClickWavBytes() {
    const int numSamples = static_cast<int>(kSampleRate * kDurationSeconds);
    const int byteRate = kSampleRate * 1 * 2;
    const int dataSize = numSamples * 2;

    std::vector<uint8_t> wav;
    wav.reserve(44 + dataSize);

    wav.insert(wav.end(), {'R', 'I', 'F', 'F'});
    appendLE<uint32_t>(wav, 36 + dataSize);
    wav.insert(wav.end(), {'W', 'A', 'V', 'E'});
    wav.insert(wav.end(), {'f', 'm', 't', ' '});
    appendLE<uint32_t>(wav, 16);
    appendLE<uint16_t>(wav, 1);
    appendLE<uint16_t>(wav, 1);
    appendLE<uint32_t>(wav, kSampleRate);
    appendLE<uint32_t>(wav, byteRate);
    appendLE<uint16_t>(wav, 2);
    appendLE<uint16_t>(wav, 16);
    wav.insert(wav.end(), {'d', 'a', 't', 'a'});
    appendLE<uint32_t>(wav, dataSize);

    constexpr double attack = 0.003;
    constexpr double release = 0.020;
    for (int i = 0; i < numSamples; i++) {
        const double t = static_cast<double>(i) / kSampleRate;
        double envelope;
        if (t < attack) {
            envelope = t / attack;
        } else if (t > kDurationSeconds - release) {
            envelope = (kDurationSeconds - t) / release;
        } else {
            envelope = 1.0;
        }
        envelope = std::clamp(envelope, 0.0, 1.0);

        const double sample = std::sin(2.0 * M_PI * kToneHz * t) * envelope;
        const int16_t pcm = static_cast<int16_t>(sample * 32000.0);
        appendLE<int16_t>(wav, pcm);
    }

    return wav;
}

void AudioCueBank::ensureLoaded() {
    if (m_loaded) return;

    auto* fmod = FMODAudioEngine::sharedEngine();
    if (!fmod || !fmod->m_system) {
        geode::log::error("AudioCueTrainer: FMOD system unavailable, cannot load cue sound.");
        return;
    }

    const auto wavBytes = buildClickWavBytes();

    FMOD_CREATESOUNDEXINFO exinfo{};
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.length = static_cast<unsigned int>(wavBytes.size());

    const auto result = fmod->m_system->createSound(
        reinterpret_cast<const char*>(wavBytes.data()),
        FMOD_OPENMEMORY | FMOD_CREATESAMPLE | FMOD_LOOP_OFF,
        &exinfo,
        &m_sound
    );

    if (result != FMOD_OK) {
        geode::log::error("AudioCueTrainer: failed to create cue sound (FMOD error {})", static_cast<int>(result));
        return;
    }

    m_loaded = true;
}

void AudioCueBank::play(float volume) {
    ensureLoaded();
    if (!m_loaded || !m_sound) return;

    auto* fmod = FMODAudioEngine::sharedEngine();
    if (!fmod || !fmod->m_system) return;

    FMOD::Channel* channel = nullptr;
    const auto result = fmod->m_system->playSound(m_sound, nullptr, false, &channel);
    if (result == FMOD_OK && channel) {
        const float sfxVolume = fmod->m_effectsVolume;
        channel->setVolume(std::clamp(sfxVolume * volume, 0.f, 1.f));
    }
}

void AudioCueBank::unload() {
    if (m_sound) {
        m_sound->release();
        m_sound = nullptr;
    }
    m_loaded = false;
}

} // namespace act

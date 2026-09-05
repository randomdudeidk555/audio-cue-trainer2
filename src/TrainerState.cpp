#include "TrainerState.hpp"
#include "PatternManager.hpp"
#include "AudioCueBank.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/GJGameLevel.hpp>

using namespace geode::prelude;

namespace act {

TrainerState& TrainerState::get() {
    static TrainerState instance;
    return instance;
}

namespace {
    void onCueFire(double scheduledClickTime, double actualFireTime) {
        if (!Mod::get()->getSettingValue<bool>("enabled")) return;

        const float volume = static_cast<float>(Mod::get()->getSettingValue<double>("cue-volume"));
        AudioCueBank::get().play(volume);

        geode::log::debug(
            "AudioCueTrainer: cue for click@{:.3f}s fired at t={:.3f}s",
            scheduledClickTime, actualFireTime
        );
    }

    double leadTimeSecondsFromSettings() {
        return static_cast<double>(Mod::get()->getSettingValue<int64_t>("lead-time-ms")) / 1000.0;
    }
    double latencyCompSecondsFromSettings() {
        return static_cast<double>(Mod::get()->getSettingValue<int64_t>("latency-comp-ms")) / 1000.0;
    }
}

void TrainerState::setCurrentLevel(GJGameLevel* level) {
    currentLevelKey = makeLevelKey(level);
    currentLevelDisplayName = level ? std::string(level->m_levelName) : "Unknown Level";
    reloadPatternForCurrentLevel();
}

void TrainerState::reloadPatternForCurrentLevel() {
    auto loaded = PatternManager::get().loadForLevel(currentLevelKey);
    currentPatternPath = loaded.sourcePath;
    currentPatternCount = static_cast<int>(loaded.timestampsSeconds.size());
    currentPatternRejected = loaded.rejectedLineCount;

    scheduler.setPattern(std::move(loaded.timestampsSeconds));
    scheduler.setFireCallback(&onCueFire);
    scheduler.setLeadTimeSeconds(leadTimeSecondsFromSettings());
    scheduler.setLatencyCompSeconds(latencyCompSecondsFromSettings());

    scheduler.resyncToTime(0.0);
}

} // namespace act

#pragma once
#include "CueScheduler.hpp"
#include <string>

class GJGameLevel;

namespace act {

// ---------------------------------------------------------------------------
// TrainerState
//
// Single shared instance tying together: which level is active, which
// pattern file was loaded for it, and the CueScheduler driving playback.
// ---------------------------------------------------------------------------
class TrainerState {
public:
    static TrainerState& get();

    CueScheduler scheduler;

    std::string currentLevelKey;
    std::string currentLevelDisplayName;
    std::string currentPatternPath;
    int currentPatternCount = 0;
    int currentPatternRejected = 0;

    void setCurrentLevel(GJGameLevel* level);
    void reloadPatternForCurrentLevel();
};

} // namespace act

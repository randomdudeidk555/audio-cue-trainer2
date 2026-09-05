#pragma once
#include <vector>
#include <cstddef>

namespace act {

// ---------------------------------------------------------------------------
// CueScheduler
//
// Pure timing logic - no FMOD, no Geode, no cocos2d. This is deliberate:
// it makes the actual "when does a cue fire" decision trivially easy to
// reason about and keeps audio/engine concerns entirely out of the
// timing math.
//
// The scheduler is fed the current, most-accurate game/song time once per
// frame from outside (see main.cpp -> currentLevelTime()). It never reads
// a clock itself and never counts frames - "how many frames since the
// last cue" is exactly the kind of frame-based timer this mod avoids,
// because that drifts as FPS changes. Instead every decision is made by
// comparing the *actual elapsed level time* against each click's
// precomputed fire time, so cue timing stays correct whether the game is
// running at 30, 60, 144 or 240 FPS.
// ---------------------------------------------------------------------------
class CueScheduler {
public:
    using FireCallback = void (*)(double scheduledClickTime, double actualFireTime);

    void setPattern(std::vector<double> sortedTimestampsSeconds);
    void clear();
    void update(double currentGameTimeSeconds);
    void resyncToTime(double currentGameTimeSeconds);

    void setLeadTimeSeconds(double seconds) { m_leadTimeSeconds = seconds; }
    void setLatencyCompSeconds(double seconds) { m_latencyCompSeconds = seconds; }
    void setFireCallback(FireCallback cb) { m_onFire = cb; }

    size_t patternSize() const { return m_timestamps.size(); }
    size_t nextIndex() const { return m_nextIndex; }

private:
    std::vector<double> m_timestamps;
    size_t m_nextIndex = 0;
    double m_leadTimeSeconds = 0.170;
    double m_latencyCompSeconds = 0.0;
    FireCallback m_onFire = nullptr;
};

} // namespace act

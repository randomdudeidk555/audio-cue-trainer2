#include "CueScheduler.hpp"
#include <algorithm>

namespace act {

void CueScheduler::setPattern(std::vector<double> sortedTimestampsSeconds) {
    m_timestamps = std::move(sortedTimestampsSeconds);
    m_nextIndex = 0;
}

void CueScheduler::clear() {
    m_timestamps.clear();
    m_nextIndex = 0;
}

void CueScheduler::update(double currentGameTimeSeconds) {
    if (m_timestamps.empty()) return;

    while (m_nextIndex < m_timestamps.size()) {
        const double clickTime = m_timestamps[m_nextIndex];
        const double fireTime = clickTime - m_leadTimeSeconds - m_latencyCompSeconds;

        if (currentGameTimeSeconds + 1e-9 >= fireTime) {
            if (m_onFire) {
                m_onFire(clickTime, currentGameTimeSeconds);
            }
            m_nextIndex++;
        } else {
            break;
        }
    }
}

void CueScheduler::resyncToTime(double currentGameTimeSeconds) {
    const double threshold = currentGameTimeSeconds + m_leadTimeSeconds + m_latencyCompSeconds;
    auto it = std::lower_bound(m_timestamps.begin(), m_timestamps.end(), threshold);
    m_nextIndex = static_cast<size_t>(it - m_timestamps.begin());
}

} // namespace act

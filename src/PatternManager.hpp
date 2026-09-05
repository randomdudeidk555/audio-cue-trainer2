#pragma once
#include <string>
#include <vector>

class GJGameLevel;

namespace act {

struct LoadedPattern {
    std::string sourcePath;
    std::vector<double> timestampsSeconds;
    int rejectedLineCount = 0;
};

// ---------------------------------------------------------------------------
// PatternManager
//
// Reads plain-text click-timestamp files: one decimal number of seconds
// per line, '#' for comments, blank lines ignored, invalid lines skipped
// (and counted), auto-sorted ascending regardless of input order.
//
// Layout on disk (under the mod's save directory):
//   clicks.txt                  <- generic/default pattern (always created)
//   patterns/id_<levelID>.txt   <- per-level pattern for uploaded levels
//   patterns/name_<slug>.txt    <- per-level pattern for local/editor levels
// ---------------------------------------------------------------------------
class PatternManager {
public:
    static PatternManager& get();

    LoadedPattern loadForLevel(const std::string& levelKey);
    void ensureExampleFilesExist();

private:
    PatternManager() = default;
    std::vector<double> parseFile(const std::string& path, int& rejectedCount);
};

std::string makeLevelKey(GJGameLevel* level);

} // namespace act

#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

namespace act {

// ---------------------------------------------------------------------------
// TrainerPopup
//
// The in-game control panel, opened from a button added to the pause
// menu. Provides Test Cue / Reload Pattern / status, while numeric
// settings (enable, lead time, latency comp, volume) live in the mod's
// normal Settings screen.
// ---------------------------------------------------------------------------
class TrainerPopup : public geode::Popup<> {
public:
    static TrainerPopup* create();

protected:
    bool setup() override;
    void refreshInfoLabel();

    void onTestCue(cocos2d::CCObject*);
    void onReloadPattern(cocos2d::CCObject*);
    void onOpenPatternFolder(cocos2d::CCObject*);

    cocos2d::CCLabelBMFont* m_infoLabel = nullptr;
};

} // namespace act

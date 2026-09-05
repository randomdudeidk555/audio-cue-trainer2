#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/binding/GJGameLevel.hpp>

#include "TrainerState.hpp"
#include "AudioCueBank.hpp"
#include "TrainerPopup.hpp"

using namespace geode::prelude;
using namespace act;

namespace {

// The single point in this whole mod that reads "what time is it in the
// level right now". PlayLayer::m_time is GD's own level-elapsed-time
// field, kept synced to the song when one is playing, and it does not
// drift when FPS changes - unlike a hand-rolled frame counter would.
double currentLevelTime(PlayLayer* pl) {
    if (!pl) return 0.0;
    return static_cast<double>(pl->m_time);
}

} // namespace

class $modify(ACT_PlayLayer, PlayLayer) {
    bool init(GJGameLevel* level) {
        if (!PlayLayer::init(level)) return false;

        AudioCueBank::get().ensureLoaded();
        TrainerState::get().setCurrentLevel(level);
        TrainerState::get().scheduler.resyncToTime(0.0);

        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();

        const double t = currentLevelTime(this);
        TrainerState::get().scheduler.resyncToTime(t);
    }

    void update(float dt) {
        PlayLayer::update(dt);
        TrainerState::get().scheduler.update(currentLevelTime(this));
    }

    void onQuit() {
        PlayLayer::onQuit();
        TrainerState::get().scheduler.clear();
    }
};

class $modify(ACT_PauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto sprite = CircleButtonSprite::createWithSpriteFrameName(
            "GJ_infoIcon_001.png",
            1.0f,
            CircleBaseColor::Green,
            CircleBaseSize::Small
        );

        auto btn = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(ACT_PauseLayer::onOpenTrainerPanel)
        );
        btn->setID("audio-cue-trainer-button"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onOpenTrainerPanel(CCObject*) {
        TrainerPopup::create()->show();
    }
};

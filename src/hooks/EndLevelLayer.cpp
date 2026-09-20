#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"

using namespace geode::prelude;

// The level-complete ("LEVEL COMPLETE" / 100%) screen. Gives the same
// one-key/one-click swap here, so finishing a linked level's run makes it
// trivial to hop straight to its pair.
class $modify(LLEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        auto pl = m_playLayer;
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // GJ_replayBtn_001.png is part of the always-loaded core UI sheet (unlike
        // the "d_"-prefixed decoration sprites, which only load inside the editor
        // and otherwise silently render as a blank placeholder).
        auto spr = CircleButtonSprite::createWithSpriteFrameName(
            "GJ_replayBtn_001.png", 1.f, CircleBaseColor::Green, CircleBaseSize::Medium);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLEndLevelLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);

        // Deliberately NOT injected into "button-menu": that menu isn't laid out
        // the same way on every variant of this screen (e.g. the editor's
        // test-play completion popup put an injected child dead-center instead
        // of alongside retry/edit/exit). A standalone menu anchored to a screen
        // corner is predictable everywhere this layer shows up.
        auto menu = CCMenu::create();
        menu->setID("level-link-switch-menu"_spr);
        menu->addChild(btn);
        this->addChildAtPosition(menu, Anchor::BottomRight, ccp(-40.f, 40.f), false);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

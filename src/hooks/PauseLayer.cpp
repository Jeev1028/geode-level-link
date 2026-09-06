#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"

using namespace geode::prelude;

class $modify(LLPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // "right-button-menu" is assigned by the geode.node-ids dependency.
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto spr = CircleButtonSprite::createWithSpriteFrameName(
            "GJ_replayBtn_001.png", 1.f, CircleBaseColor::Green, CircleBaseSize::Medium);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

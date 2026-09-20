#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

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

        // Our own bundled icon (resources/link-icon.png). See LevelInfoLayer.cpp
        // for why the scale factor looks large - it corrects for how Geode's
        // resource pipeline defines the sprite's 1x point size.
        auto spr = CCSprite::create("link-icon.png"_spr);
        spr->setScale(1.6f);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

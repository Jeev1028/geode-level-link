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

        // A standard circular button base (matching every other GD/mod button)
        // with our own icon as the top content. CircleButtonSprite auto-fits
        // the top node to size, so no manual scale tuning is needed here.
        auto icon = CCSprite::create("link-icon.png"_spr);
        auto spr = CircleButtonSprite::create(icon, CircleBaseColor::Green, CircleBaseSize::Medium);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

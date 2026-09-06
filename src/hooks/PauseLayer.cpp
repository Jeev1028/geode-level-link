#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"

#ifndef GEODE_IS_MOBILE
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

using namespace geode::prelude;

class $modify(LLPauseLayer, PauseLayer) {
    struct Fields {
        bool hasLink = false;
    };

    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;

        m_fields->hasLink = LinkStore::get()->getLink(pl->m_level).has_value();
        if (!m_fields->hasLink) return;

        // ADJUST: confirm this node id against Geode's PauseLayer node IDs
        // (alternatives: "left-button-menu", "bottom-button-menu").
        if (auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("right-button-menu"))) {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
            spr->setScale(0.7f);
            auto btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(LLPauseLayer::onSwitchLinked));
            btn->setID("switch-linked-level"_spr);
            menu->addChild(btn);
            menu->updateLayout();
        }

#ifndef GEODE_IS_MOBILE
        this->addEventListener<InvokeBindFilter>(
            [this](InvokeBindEvent* event) {
                if (event->isDown() && m_fields->hasLink) this->onSwitchLinked(nullptr);
                return ListenerResult::Propagate;
            },
            "switch-linked-level"_spr);
#endif
    }

    void onSwitchLinked(CCObject*) {
        if (auto pl = PlayLayer::get(); pl && pl->m_level) {
            LinkManager::get()->requestSwitch(pl->m_level);
        }
    }
};

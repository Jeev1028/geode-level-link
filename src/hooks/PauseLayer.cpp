#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/LinkIcon.hpp"

using namespace geode::prelude;

class $modify(LLPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // "right-button-menu" is a narrow (40pt-wide) column Geode's node-ids
        // creates specifically for small utility icons (gear/eye/percent/
        // scroll, ~87pt each) - NOT the big circular action buttons. Match
        // those, not the bottom-row buttons.
        auto menu = this->getChildByID("right-button-menu");
        if (!menu) return;

        auto btn = LinkIcon::createButton(88.f, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

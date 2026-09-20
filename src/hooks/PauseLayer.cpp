#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/CornerSpot.hpp"
#include "../ui/LinkIcon.hpp"

using namespace geode::prelude;

class $modify(LLPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // Deliberately NOT injected into "right-button-menu": that's a narrow
        // full-height column node-ids creates for small utility icons, and
        // other mods (geode-in-pause-menu, quests-in-pause-menu, etc.) also
        // add their own buttons to it. Adding a child there changes the
        // ColumnLayout's child order relative to those other mods' hooks and
        // was displacing their buttons from their normal slot. A standalone
        // menu we position ourselves never touches that column at all, so
        // nothing else already in it can be pushed around.
        auto btn = LinkIcon::createButton(88.f, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);

        auto menu = CCMenu::create();
        menu->setID("level-link-switch-menu"_spr);
        menu->addChild(btn);
        menu->setAnchorPoint({1.f, 0.f});

        // Extra horizontal margin (vs. the ~24pt used elsewhere) so we clear
        // the full-height "right-button-menu" column running down that edge.
        const auto winSize = CCDirector::sharedDirector()->getWinSize();
        menu->setPosition(CornerSpot::findBottomRight(this, winSize, /*marginX*/ 76.f,
                                                        /*marginY*/ 24.f));

        this->addChild(menu);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

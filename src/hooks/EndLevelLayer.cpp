#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

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

        // Our own bundled icon (resources/link-icon.png). Scale calibrated
        // against a screenshot: the ~175px-diameter neighboring circular
        // buttons vs. our ~190px-tall badge at scale 1.6.
        auto spr = CCSprite::create("link-icon.png"_spr);
        spr->setScale(1.3f);
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
        this->addChildAtPosition(menu, Anchor::BottomRight, ccp(-40.f, 33.f), false);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

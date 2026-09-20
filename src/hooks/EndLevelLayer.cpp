#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/CornerSpot.hpp"
#include "../ui/LinkIcon.hpp"

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

        // Same size as the level-page buttons (88pt) - the only size we've
        // been able to confirm precisely, pixel-for-pixel, against a real
        // screenshot. This screen's own bottom-row buttons are much bigger
        // (~195pt), but our own standalone corner button doesn't need to
        // match those exactly.
        auto btn =
            LinkIcon::createButton(88.f, this, menu_selector(LLEndLevelLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);

        auto menu = CCMenu::create();
        menu->setID("level-link-switch-menu"_spr);
        menu->addChild(btn);
        // Anchor at bottom-right so the position we compute is that corner,
        // regardless of the menu's own (auto-sized) content size.
        menu->setAnchorPoint({1.f, 0.f});

        // Deliberately NOT injected into "button-menu": that menu isn't laid out
        // the same way on every variant of this screen (e.g. the editor's
        // test-play completion popup put an injected child dead-center instead
        // of alongside retry/edit/exit). A standalone menu we place ourselves
        // is predictable everywhere this layer shows up, and adjusts around
        // whatever other mods have already added to this screen.
        const auto winSize = CCDirector::sharedDirector()->getWinSize();
        menu->setPosition(CornerSpot::findBottomRight(this, winSize));

        this->addChild(menu);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

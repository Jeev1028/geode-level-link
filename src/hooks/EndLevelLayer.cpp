#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"

using namespace geode::prelude;

namespace {

// Find a free bottom-right spot for our button, stacking above anything else
// another mod may have already placed in that corner (e.g. run-stat trackers)
// instead of a hardcoded offset that only works when nothing else is there.
// Returns the position for a node whose anchor point is (1, 0) (bottom-right).
CCPoint findBottomRightSpot(EndLevelLayer* layer, const CCSize& winSize) {
    // Comfortably inside the corner - not flush against the edge, which was
    // getting clipped by the panel border.
    const float margin = 24.f;
    float x = winSize.width - margin;
    float y = margin;

    // Only look at reasonably small, direct children near that corner - skip
    // huge full-layer containers (like m_mainLayer) so we don't mistake the
    // whole screen for "something occupying the corner".
    const CCRect corner{winSize.width - 260.f, 0.f, 260.f, 260.f};
    for (auto child : CCArrayExt<CCNode*>(layer->getChildren())) {
        if (!child) continue;
        auto size = child->getContentSize();
        if (size.width <= 0.f || size.height <= 0.f) continue;
        if (size.width > winSize.width * 0.5f || size.height > winSize.height * 0.5f) continue;

        auto pos = child->getPosition();
        auto anchor = child->getAnchorPoint();
        CCRect rect{pos.x - size.width * anchor.x, pos.y - size.height * anchor.y, size.width,
                    size.height};
        if (!rect.intersectsRect(corner)) continue;

        y = std::max(y, rect.getMaxY() + 16.f);
    }

    return {x, y};
}

}  // namespace

// The level-complete ("LEVEL COMPLETE" / 100%) screen. Gives the same
// one-key/one-click swap here, so finishing a linked level's run makes it
// trivial to hop straight to its pair.
class $modify(LLEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        auto pl = m_playLayer;
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // Our own bundled icon (resources/link-icon.png). Same scale as the
        // level-page/edit-level buttons - confirmed to look right there.
        auto spr = CCSprite::create("link-icon.png"_spr);
        spr->setScale(0.65f);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLEndLevelLayer::onSwitchLinked));
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
        menu->setPosition(findBottomRightSpot(this, winSize));

        this->addChild(menu);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

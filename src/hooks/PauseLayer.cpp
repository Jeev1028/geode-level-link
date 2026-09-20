#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/LinkIcon.hpp"

using namespace geode::prelude;

namespace {

// The world-space (converted into `relativeTo`'s local space) bottom-center
// point of whichever child in `menu` currently sits lowest. Reading the
// actual children rather than trusting the menu's own (possibly oversized,
// not auto-fit) contentSize means this adapts automatically to however many
// other mods (geode-in-pause-menu, quests-in-pause-menu, xdbot, etc.) have
// already added their own buttons to this same column.
std::optional<CCPoint> findLowestChildBottom(CCMenu* menu, CCNode* relativeTo) {
    std::optional<CCRect> lowest;
    for (auto child : CCArrayExt<CCNode*>(menu->getChildren())) {
        if (!child) continue;
        auto rect = child->boundingBox();
        if (!lowest || rect.getMinY() < lowest->getMinY()) lowest = rect;
    }
    if (!lowest) return std::nullopt;

    CCPoint localBottomCenter{lowest->getMidX(), lowest->getMinY()};
    return relativeTo->convertToNodeSpace(menu->convertToWorldSpace(localBottomCenter));
}

}  // namespace

class $modify(LLPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto pl = PlayLayer::get();
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // Deliberately NOT injected into "right-button-menu": that's the
        // narrow column node-ids creates for small utility icons, and other
        // mods (geode-in-pause-menu, quests-in-pause-menu, xdbot, etc.) also
        // add their own buttons to it. Adding a child there changes that
        // ColumnLayout's child order relative to those other mods' hooks and
        // was displacing their buttons from their normal slot.
        //
        // Instead, match how those other mods behave: a standalone button
        // placed in the same right-edge column, at a consistent margin below
        // whichever button currently sits lowest - never touching the shared
        // menu itself, so nothing already in it can be pushed around.
        auto rightMenu = typeinfo_cast<CCMenu*>(this->getChildByID("right-button-menu"));
        if (!rightMenu) return;

        constexpr float kDiameter = 88.f;
        constexpr float kMargin = 20.f;

        auto anchor = findLowestChildBottom(rightMenu, this);
        if (!anchor) return;

        auto btn = LinkIcon::createButton(kDiameter, this, menu_selector(LLPauseLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);

        auto menu = CCMenu::create();
        menu->setID("level-link-switch-menu"_spr);
        menu->addChild(btn);
        // menu's own position IS the button's visual center (no contentSize
        // set, so anchor offset is zero) - place it `kMargin` below the
        // lowest existing button's bottom edge, minus our own radius so our
        // *top* edge (not center) is what sits `kMargin` away.
        menu->setPosition({anchor->x, anchor->y - kMargin - kDiameter / 2.f});

        this->addChild(menu);
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

#include <Geode/Geode.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"

using namespace geode::prelude;

// The level-complete ("LEVEL COMPLETE" / 100%) screen. Gives the same
// one-key/one-click swap here, so finishing a linked level's run makes it
// trivial to hop straight to its pair (e.g. jump from the StartPos practice
// copy you just finished into the rated level, or vice versa).
class $modify(LLEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        auto pl = m_playLayer;
        if (!pl || !pl->m_level) return;
        if (!LinkStore::get()->getLink(pl->m_level).has_value()) return;

        // "button-menu" (holding retry/exit/edit/...) is assigned by the
        // geode.node-ids dependency, inside GJDropDownLayer's m_mainLayer.
        auto menu =
            m_mainLayer ? typeinfo_cast<CCMenu*>(m_mainLayer->getChildByID("button-menu")) : nullptr;
        if (!menu) return;

        auto spr = CircleButtonSprite::createWithSpriteFrameName(
            "d_link_01_001.png", 1.f, CircleBaseColor::Green, CircleBaseSize::Medium);
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(LLEndLevelLayer::onSwitchLinked));
        btn->setID("switch-linked-level"_spr);
        menu->addChild(btn);
        menu->updateLayout();
    }

    void onSwitchLinked(CCObject*) { LinkManager::get()->requestSwitchFromCurrent(); }
};

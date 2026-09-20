#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/LinkPopup.hpp"

using namespace geode::prelude;

// The screen you get when you tap one of your own created levels
// (Edit / Play / Share buttons). Adds a "link" button to the right-hand
// action column so local levels can be linked too.
class $modify(LLEditLevelLayer, EditLevelLayer) {
    bool init(GJGameLevel * level) {
        if (!EditLevelLayer::init(level)) return false;

        auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("level-actions-menu"));
        if (!menu) {
            menu = typeinfo_cast<CCMenu*>(this->getChildByID("info-button-menu"));
        }
        if (!menu) return true;

        // Our own bundled icon (resources/link-icon.png). See LevelInfoLayer.cpp
        // for why the scale factor looks large - it corrects for how Geode's
        // resource pipeline defines the sprite's 1x point size.
        auto spr = CCSprite::create("link-icon.png"_spr);
        spr->setScale(1.3f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this,
                                                 menu_selector(LLEditLevelLayer::onLevelLink));
        btn->setID("level-link-button"_spr);
        menu->addChild(btn);
        menu->updateLayout();

        if (const auto link = LinkStore::get()->getLink(level)) {
            LinkManager::get()->precache(*link);
        }
        return true;
    }

    void onLevelLink(CCObject*) {
        if (auto popup = LinkPopup::create(m_level)) popup->show();
    }
};

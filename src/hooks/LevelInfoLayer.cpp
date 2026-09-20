#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/LinkPopup.hpp"

using namespace geode::prelude;

class $modify(LLLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel * level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        // "left-side-menu" is assigned by the geode.node-ids dependency.
        auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("left-side-menu"));
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("level-link-menu"_spr);
            menu->setContentSize({40.f, 180.f});
            menu->setLayout(ColumnLayout::create()->setAxisReverse(true));
            this->addChildAtPosition(menu, Anchor::Left, ccp(28.f, 0.f), false);
        }

        // Our own bundled icon (resources/link-icon.png). Geode's resource
        // pipeline treats the packaged "normal" (non-hd/uhd) file's pixel size
        // as the sprite's 1x point size, which for this asset is small - scale
        // up to roughly match the neighboring circular buttons.
        auto spr = CCSprite::create("link-icon.png"_spr);
        spr->setScale(1.3f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this,
                                                 menu_selector(LLLevelInfoLayer::onLevelLink));
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

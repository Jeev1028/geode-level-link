#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

#include "../LinkManager.hpp"
#include "../LinkStore.hpp"
#include "../ui/LinkPopup.hpp"

using namespace geode::prelude;

// ADJUST: for older GD the signature is init(GJGameLevel*) with no `challenge`.
class $modify(LLLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel * level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        auto menu = typeinfo_cast<CCMenu*>(this->getChildByID("left-side-menu"));
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("level-link-menu"_spr);
            const auto win = CCDirector::sharedDirector()->getWinSize();
            menu->setPosition(28.f, win.height - 30.f);
            this->addChild(menu, 100);
        }

        auto spr = CCSprite::createWithSpriteFrameName("GJ_linkBtn_001.png");
        if (!spr) spr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
        spr->setScale(0.8f);
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

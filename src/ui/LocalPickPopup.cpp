#include "LocalPickPopup.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace {
constexpr float POPUP_W = 320.f;
constexpr float POPUP_H = 250.f;
}  // namespace

bool LocalPickPopup::init(std::function<void(GJGameLevel*)> cb) {
    if (!Popup::init(POPUP_W, POPUP_H)) return false;
    m_callback = std::move(cb);
    this->setTitle("Pick a Local Level");

    const CCSize listSize{264.f, 180.f};

    auto scroll = ScrollLayer::create(listSize);
    scroll->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    m_mainLayer->addChildAtPosition(scroll, Anchor::Center,
                                    ccp(-listSize.width / 2.f, -listSize.height / 2.f - 8.f),
                                    ccp(0.f, 0.f));

    std::vector<GJGameLevel*> levels;
    if (auto mgr = LocalLevelManager::get(); mgr && mgr->m_localLevels) {
        for (auto lvl : CCArrayExt<GJGameLevel*>(mgr->m_localLevels)) levels.push_back(lvl);
    }

    for (auto lvl : levels) {
        auto row = CCMenu::create();
        row->setContentSize({listSize.width, 30.f});

        auto item = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(std::string(lvl->m_levelName).c_str(), 240, true, "bigFont.fnt",
                                 "GJ_button_05.png", 30.f, 0.5f),
            this, menu_selector(LocalPickPopup::onPick));
        item->setUserObject("level", lvl);
        row->addChildAtPosition(item, Anchor::Center);

        scroll->m_contentLayer->addChild(row);
    }

    if (levels.empty()) {
        auto empty = CCLabelBMFont::create("No local levels", "bigFont.fnt");
        empty->setScale(0.5f);
        empty->setOpacity(150);
        auto row = CCMenu::create();
        row->setContentSize({listSize.width, 30.f});
        row->addChildAtPosition(empty, Anchor::Center);
        scroll->m_contentLayer->addChild(row);
    }

    scroll->m_contentLayer->updateLayout();
    scroll->scrollToTop();
    return true;
}

void LocalPickPopup::onPick(CCObject* sender) {
    auto node = static_cast<CCNode*>(sender);
    auto level = static_cast<GJGameLevel*>(node->getUserObject("level"));
    if (level && m_callback) m_callback(level);
    this->onClose(nullptr);
}

LocalPickPopup* LocalPickPopup::create(std::function<void(GJGameLevel*)> cb) {
    auto ret = new LocalPickPopup();
    if (ret->init(std::move(cb))) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

#include "LocalPickPopup.hpp"

using namespace geode::prelude;

bool LocalPickPopup::setup(std::function<void(GJGameLevel*)> cb) {
    m_callback = std::move(cb);
    this->setTitle("Pick a Local Level");

    const auto size = m_mainLayer->getContentSize();
    const CCSize listSize{260.f, 150.f};

    auto scroll = ScrollLayer::create(listSize);
    scroll->setPosition((size.width - listSize.width) / 2.f, 20.f);
    m_mainLayer->addChild(scroll);

    std::vector<GJGameLevel*> levels;
    if (auto mgr = LocalLevelManager::get(); mgr && mgr->m_localLevels) {
        for (auto lvl : CCArrayExt<GJGameLevel*>(mgr->m_localLevels)) levels.push_back(lvl);
    }

    const float rowH = 32.f;
    const float totalH = std::max(listSize.height, levels.size() * rowH);
    scroll->m_contentLayer->setContentSize({listSize.width, totalH});

    int i = 0;
    for (auto lvl : levels) {
        auto row = CCMenu::create();
        row->setPosition(listSize.width / 2.f, totalH - rowH / 2.f - i * rowH);
        row->setContentSize({listSize.width, rowH});

        auto btn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(lvl->m_levelName.c_str(), 230, true, "bigFont.fnt",
                                 "GJ_button_05.png", 26.f, 0.55f),
            this, menu_selector(LocalPickPopup::onPick));
        btn->setUserObject("level", lvl);
        row->addChild(btn);

        scroll->m_contentLayer->addChild(row);
        ++i;
    }

    if (levels.empty()) {
        auto empty = CCLabelBMFont::create("No local levels", "bigFont.fnt");
        empty->setScale(0.5f);
        empty->setPosition(listSize.width / 2.f, listSize.height / 2.f);
        scroll->m_contentLayer->addChild(empty);
    }

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
    if (ret->initAnchored(320.f, 220.f, std::move(cb))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

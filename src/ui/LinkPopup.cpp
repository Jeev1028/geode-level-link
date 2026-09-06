#include "LinkPopup.hpp"

#include <Geode/Geode.hpp>

#include "../LinkStore.hpp"
#include "LocalPickPopup.hpp"

using namespace geode::prelude;

namespace {
constexpr float POPUP_W = 340.f;
constexpr float POPUP_H = 220.f;
}  // namespace

bool LinkPopup::init(GJGameLevel* level) {
    if (!Popup::init(POPUP_W, POPUP_H)) return false;
    m_level = level;
    this->setTitle("Link Level");

    m_statusLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_statusLabel->setScale(0.45f);
    m_mainLayer->addChildAtPosition(m_statusLabel, Anchor::Top, ccp(0.f, -30.f));

    m_idInput = TextInput::create(200.f, "Online Level ID");
    m_idInput->setCommonFilter(CommonFilter::Uint);
    m_mainLayer->addChildAtPosition(m_idInput, Anchor::Center, ccp(0.f, 30.f));

    auto actionMenu = CCMenu::create();
    actionMenu->setContentWidth(POPUP_W - 40.f);
    actionMenu->addChild(CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Link by ID"), this, menu_selector(LinkPopup::onLinkById)));
    actionMenu->addChild(CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Link Local"), this, menu_selector(LinkPopup::onLinkLocal)));
    actionMenu->setLayout(RowLayout::create()->setGap(10.f));
    m_mainLayer->addChildAtPosition(actionMenu, Anchor::Center, ccp(0.f, -12.f));

    auto unlinkMenu = CCMenu::create();
    auto unlinkBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Unlink", "goldFont.fnt", "GJ_button_06.png"), this,
        menu_selector(LinkPopup::onUnlink));
    unlinkBtn->setScale(0.7f);
    unlinkMenu->addChild(unlinkBtn);
    unlinkMenu->setLayout(RowLayout::create());
    m_mainLayer->addChildAtPosition(unlinkMenu, Anchor::Bottom, ccp(0.f, 26.f));

    this->refreshStatus();
    return true;
}

void LinkPopup::onLinkById(CCObject*) {
    const int id =
        numFromString<int>(std::string(m_idInput->getString())).unwrapOr(0);
    if (id <= 0) {
        FLAlertLayer::create("Invalid", "Enter a valid online level ID.", "OK")->show();
        return;
    }
    if (id == LinkStore::refFor(m_level).id) {
        FLAlertLayer::create("Invalid", "A level can't be linked to itself.", "OK")->show();
        return;
    }

    LinkStore::get()->setLink(m_level, LevelRef{"online", id, fmt::format("Online #{}", id)});
    Notification::create("Linked!", NotificationIcon::Success)->show();
    this->refreshStatus();
}

void LinkPopup::onLinkLocal(CCObject*) {
    LocalPickPopup::create([this](GJGameLevel* picked) {
        if (picked == m_level) {
            FLAlertLayer::create("Invalid", "A level can't be linked to itself.", "OK")->show();
            return;
        }
        LinkStore::get()->setLink(
            m_level, LevelRef{"local", 0, std::string(picked->m_levelName)});
        Notification::create("Linked to local level", NotificationIcon::Success)->show();
        this->refreshStatus();
    })->show();
}

void LinkPopup::onUnlink(CCObject*) {
    LinkStore::get()->removeLink(m_level);
    Notification::create("Unlinked", NotificationIcon::Success)->show();
    this->refreshStatus();
}

void LinkPopup::refreshStatus() {
    if (const auto link = LinkStore::get()->getLink(m_level)) {
        const auto text = link->isOnline()
                              ? fmt::format("Linked to: online #{}", link->id)
                              : fmt::format("Linked to: local \"{}\"", link->name);
        m_statusLabel->setString(text.c_str());
    } else {
        m_statusLabel->setString("Not linked");
    }
}

LinkPopup* LinkPopup::create(GJGameLevel* level) {
    auto ret = new LinkPopup();
    if (ret->init(level)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

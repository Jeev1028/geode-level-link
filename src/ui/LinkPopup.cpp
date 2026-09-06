#include "LinkPopup.hpp"

#include "../LinkStore.hpp"
#include "LocalPickPopup.hpp"

using namespace geode::prelude;

bool LinkPopup::setup(GJGameLevel* level) {
    m_level = level;
    this->setTitle("Link Level");

    const auto size = m_mainLayer->getContentSize();

    m_status = CCLabelBMFont::create("", "bigFont.fnt");
    m_status->setScale(0.45f);
    m_status->setPosition(size.width / 2.f, size.height - 48.f);
    m_mainLayer->addChild(m_status);

    m_idInput = TextInput::create(190.f, "Online Level ID");
    m_idInput->setCommonFilter(CommonFilter::Uint);
    m_idInput->setPosition(size.width / 2.f, size.height / 2.f + 12.f);
    m_mainLayer->addChild(m_idInput);

    auto menu = CCMenu::create();
    menu->setPosition(size.width / 2.f, size.height / 2.f - 34.f);
    m_mainLayer->addChild(menu);

    auto byId = CCMenuItemSpriteExtra::create(ButtonSprite::create("Link by ID"), this,
                                              menu_selector(LinkPopup::onLinkById));
    auto local = CCMenuItemSpriteExtra::create(ButtonSprite::create("Link Local"), this,
                                               menu_selector(LinkPopup::onLinkLocal));
    menu->addChild(byId);
    menu->addChild(local);
    menu->setLayout(RowLayout::create()->setGap(10.f));

    auto bottomMenu = CCMenu::create();
    bottomMenu->setPosition(size.width / 2.f, 26.f);
    m_mainLayer->addChild(bottomMenu);
    auto unlink = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Unlink", "goldFont.fnt", "GJ_button_06.png"), this,
        menu_selector(LinkPopup::onUnlink));
    bottomMenu->addChild(unlink);
    bottomMenu->setLayout(RowLayout::create());

    this->refreshStatus();
    return true;
}

void LinkPopup::onLinkById(CCObject*) {
    const auto parsed = geode::utils::numFromString<int>(m_idInput->getString());
    const int id = parsed.unwrapOr(0);
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
        LinkStore::get()->setLink(m_level,
                                  LevelRef{"local", 0, std::string(picked->m_levelName)});
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
        m_status->setString(
            fmt::format("Linked to: {}", link->isOnline()
                                             ? fmt::format("online #{}", link->id)
                                             : fmt::format("local \"{}\"", link->name))
                .c_str());
    } else {
        m_status->setString("Not linked");
    }
}

LinkPopup* LinkPopup::create(GJGameLevel* level) {
    auto ret = new LinkPopup();
    if (ret->initAnchored(340.f, 220.f, level)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

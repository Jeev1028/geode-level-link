#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

// "Link this level to another level" dialog, opened from the level info page.
// Geode 5.x: geode::Popup is no longer templated - subclass + custom init().
class LinkPopup : public geode::Popup {
protected:
    GJGameLevel* m_level = nullptr;
    geode::TextInput* m_idInput = nullptr;
    cocos2d::CCLabelBMFont* m_statusLabel = nullptr;

    bool init(GJGameLevel* level);

    void onLinkById(cocos2d::CCObject*);
    void onLinkLocal(cocos2d::CCObject*);
    void onUnlink(cocos2d::CCObject*);
    void refreshStatus();

public:
    static LinkPopup* create(GJGameLevel* level);
};

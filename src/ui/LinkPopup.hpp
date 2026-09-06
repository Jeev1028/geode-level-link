#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

// "Link this level to another level" dialog, opened from the level info page.
class LinkPopup : public geode::Popup<GJGameLevel*> {
protected:
    GJGameLevel* m_level = nullptr;
    geode::TextInput* m_idInput = nullptr;
    cocos2d::CCLabelBMFont* m_status = nullptr;

    bool setup(GJGameLevel* level) override;

    void onLinkById(cocos2d::CCObject*);
    void onLinkLocal(cocos2d::CCObject*);
    void onUnlink(cocos2d::CCObject*);
    void refreshStatus();

public:
    static LinkPopup* create(GJGameLevel* level);
};

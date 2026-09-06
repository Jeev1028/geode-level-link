#pragma once

#include <Geode/ui/Popup.hpp>

#include <functional>

// A scrollable list of the player's local levels. Calls back with the picked one.
class LocalPickPopup : public geode::Popup {
protected:
    std::function<void(GJGameLevel*)> m_callback;

    bool init(std::function<void(GJGameLevel*)> cb);
    void onPick(cocos2d::CCObject* sender);

public:
    static LocalPickPopup* create(std::function<void(GJGameLevel*)> cb);
};

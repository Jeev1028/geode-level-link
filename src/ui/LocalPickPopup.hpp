#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>

#include <functional>

// A scrollable list of the player's local levels. Calls back with the picked one.
class LocalPickPopup : public geode::Popup<std::function<void(GJGameLevel*)>> {
protected:
    std::function<void(GJGameLevel*)> m_callback;

    bool setup(std::function<void(GJGameLevel*)> cb) override;
    void onPick(cocos2d::CCObject* sender);

public:
    static LocalPickPopup* create(std::function<void(GJGameLevel*)> cb);
};

#pragma once

#include <Geode/Geode.hpp>

#include "LinkStore.hpp"

// Owns the "load the other level and swap the scene" flow.
class LinkManager : public LevelDownloadDelegate {
public:
    static LinkManager* get();

    // True when the player is paused inside a level that has a linked level.
    static bool canSwitchNow();

    // Entry point from the pause-menu button and the keybind.
    void requestSwitchFromCurrent();

    // If a LevelInfoLayer or EditLevelLayer is currently on screen, opens its
    // LinkPopup (same as tapping the level page's link button). Entry point
    // for the "open link menu" keybind. Returns true if a popup was opened.
    static bool openLinkMenuFromCurrent();

    // Fire-and-forget background download so a later switch is instant.
    void precache(const LevelRef& ref);

    // LevelDownloadDelegate
    void levelDownloadFinished(GJGameLevel* level) override;
    void levelDownloadFailed(int reason) override;

private:
    void requestSwitch(GJGameLevel* current);
    void doSwitch(GJGameLevel* target);

    static GJGameLevel* findLocal(const std::string& name);

    bool m_switchAfterDownload = false;
    float m_pendingMatchX = 0.f;
};

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "LinkManager.hpp"

using namespace geode::prelude;

$execute {
    // Native Geode 5.x keybind setting - no external keybind mod needed.
    geode::listenForKeybindSettingPresses(
        "switch-key",
        [](Keybind const&, bool down, bool repeat, double) -> bool {
            if (!down || repeat) return false;
            if (!LinkManager::canSwitchNow()) return false;
            LinkManager::get()->requestSwitchFromCurrent();
            return true;  // consume the keypress
        });

    // Separate keybind for opening the link/unlink menu from a level's info
    // or edit page - independent of "switch-key" since it's active in a
    // different context (browsing a level page, not paused inside one).
    geode::listenForKeybindSettingPresses(
        "open-link-menu-key",
        [](Keybind const&, bool down, bool repeat, double) -> bool {
            if (!down || repeat) return false;
            return LinkManager::openLinkMenuFromCurrent();
        });

    log::info("Level Link loaded");
}

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

    log::info("Level Link loaded");
}

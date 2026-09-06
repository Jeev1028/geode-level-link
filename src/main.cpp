#include <Geode/Geode.hpp>

#ifndef GEODE_IS_MOBILE
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

using namespace geode::prelude;

$execute {
#ifndef GEODE_IS_MOBILE
    // ADJUST: the 5th arg is the keybind category. Depending on the
    // custom-keybinds version this is a `Category` enum or a string path.
    BindManager::get()->registerBindable({
        "switch-linked-level"_spr,
        "Switch Linked Level",
        "While paused in a linked level, swap to its linked version.",
        {Keybind::create(KEY_T, Modifier::None)},
        "Level Link",
    });
#endif
    log::info("Level Link loaded");
}

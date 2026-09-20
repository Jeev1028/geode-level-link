#pragma once

#include <Geode/Geode.hpp>

// Builds the mod's icon/button visuals from bundled part-sprites, tinted at
// runtime per the mod's color settings. A custom composite rather than
// CircleButtonSprite because the latter only supports a fixed 8-color
// palette, not the arbitrary colors these settings let the user pick.
namespace LinkIcon {

// The two-square / chain / swap-arrow icon on its own, tinted per settings.
cocos2d::CCNode* createIcon();

// A full circular button (drop shadow + white ring + tinted fill + icon),
// `diameter` points across, wrapped as a clickable menu item.
// (CCMenuItemSpriteExtra is a GD/global-namespace binding, not cocos2d/geode.)
::CCMenuItemSpriteExtra* createButton(float diameter, cocos2d::CCObject* target,
                                      cocos2d::SEL_MenuHandler selector);

}  // namespace LinkIcon

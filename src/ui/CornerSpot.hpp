#pragma once

#include <Geode/Geode.hpp>

// Finds a place to put our own button that doesn't depend on joining another
// menu's layout (which risks displacing other mods' buttons already in that
// menu, since node-ids-created menus don't auto-arrange around oversized or
// unexpected children).
namespace CornerSpot {

// Returns the position for a node whose anchor point is (1, 0) (bottom-right),
// placed `marginX`/`marginY` inside that corner of `layer`, and pushed above
// anything else small already occupying that general area (e.g. another
// mod's button) so we stack instead of overlapping.
cocos2d::CCPoint findBottomRight(cocos2d::CCNode* layer, const cocos2d::CCSize& winSize,
                                 float marginX = 24.f, float marginY = 24.f,
                                 float scanSize = 260.f);

}  // namespace CornerSpot

#include "CornerSpot.hpp"

using namespace geode::prelude;

namespace CornerSpot {

CCPoint findBottomRight(CCNode* layer, const CCSize& winSize, float marginX, float marginY,
                        float scanSize) {
    float x = winSize.width - marginX;
    float y = marginY;

    // Only look at reasonably small, direct children near that corner - skip
    // huge full-layer containers (like m_mainLayer, or a full-height side
    // column like "right-button-menu") so we don't mistake the whole screen
    // for "something occupying the corner", and don't try to stack above menus
    // we're deliberately avoiding rather than colliding with.
    const CCRect corner{winSize.width - scanSize, 0.f, scanSize, scanSize};
    for (auto child : CCArrayExt<CCNode*>(layer->getChildren())) {
        if (!child) continue;
        auto size = child->getContentSize();
        if (size.width <= 0.f || size.height <= 0.f) continue;
        if (size.width > winSize.width * 0.5f || size.height > winSize.height * 0.5f) continue;

        auto pos = child->getPosition();
        auto anchor = child->getAnchorPoint();
        CCRect rect{pos.x - size.width * anchor.x, pos.y - size.height * anchor.y, size.width,
                    size.height};
        if (!rect.intersectsRect(corner)) continue;

        y = std::max(y, rect.getMaxY() + 16.f);
    }

    return {x, y};
}

}  // namespace CornerSpot

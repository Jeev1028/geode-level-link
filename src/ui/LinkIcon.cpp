#include "LinkIcon.hpp"

using namespace geode::prelude;

namespace {

// Design-space size of the icon composite, matching the part-sprites' real
// layout as extracted from the source artwork (see the positions below).
constexpr float ICON_W = 285.f;
constexpr float ICON_H = 157.f;

// How much bigger the swap-circle/arrows render than their native size, so
// they overlap onto the two squares instead of just sitting in the gap.
constexpr float MIDDLE_SCALE = 1.45f;

// The base part-sprites (link-base-*.png) are authored so that, after
// Geode's resource pipeline downsamples them, they land at this diameter -
// i.e. setScale(diameter / BASE_REF) gives an exact final size.
constexpr float BASE_REF = 200.f;

ccColor3B settingColor(const char* key) { return Mod::get()->getSettingValue<ccColor3B>(key); }

}  // namespace

CCNode* LinkIcon::createIcon() {
    // Layout (design units, origin bottom-left) - matches each part's real
    // position as extracted from the source artwork:
    //   left square:  centered (77.5, 78)
    //   right square: centered (211.5, 78)
    //   chain: centered (142.5, 80.5), behind everything else
    //   swap-circle + arrows: centered (143, 79), enlarged so they overlap
    //   onto both squares instead of just filling the gap.
    auto container = CCNode::create();
    container->setContentSize({ICON_W, ICON_H});

    auto left = CCSprite::create("link-square-left.png"_spr);
    left->setColor(settingColor("icon-left-color"));
    left->setPosition({77.5f, 78.f});
    container->addChild(left);

    auto right = CCSprite::create("link-square-right.png"_spr);
    right->setColor(settingColor("icon-right-color"));
    right->setPosition({211.5f, 78.f});
    container->addChild(right);

    auto chain = CCSprite::create("link-chain.png"_spr);
    chain->setPosition({142.5f, 80.5f});
    container->addChild(chain);

    auto middle = CCSprite::create("link-swap-circle.png"_spr);
    middle->setColor(settingColor("icon-middle-color"));
    middle->setScale(MIDDLE_SCALE);
    middle->setPosition({143.f, 79.f});
    container->addChild(middle);

    auto arrows = CCSprite::create("link-swap-arrows.png"_spr);
    arrows->setScale(MIDDLE_SCALE);
    arrows->setPosition({143.f, 79.f});
    container->addChild(arrows);

    return container;
}

CCMenuItemSpriteExtra* LinkIcon::createButton(float diameter, CCObject* target,
                                              SEL_MenuHandler selector) {
    const float baseScale = diameter / BASE_REF;

    auto node = CCNode::create();
    node->setContentSize({diameter, diameter});
    node->setAnchorPoint({0.5f, 0.5f});
    node->ignoreAnchorPointForPosition(false);

    const CCPoint center{diameter / 2.f, diameter / 2.f};

    auto shadow = CCSprite::create("link-base-shadow.png"_spr);
    shadow->setScale(baseScale);
    shadow->setPosition(center);
    node->addChild(shadow);

    auto ring = CCSprite::create("link-base-ring.png"_spr);
    ring->setScale(baseScale);
    ring->setPosition(center);
    node->addChild(ring);

    auto fill = CCSprite::create("link-base-fill.png"_spr);
    fill->setColor(settingColor("circle-color"));
    fill->setScale(baseScale);
    fill->setPosition(center);
    node->addChild(fill);

    // Icon container has default (0,0) anchor, so position its bottom-left
    // corner such that its scaled footprint ends up centered in the button.
    auto icon = createIcon();
    const float iconScale = (diameter * 0.72f) / ICON_W;
    icon->setScale(iconScale);
    icon->setPosition({center.x - (ICON_W * iconScale) / 2.f,
                       center.y - (ICON_H * iconScale) / 2.f});
    node->addChild(icon);

    return CCMenuItemSpriteExtra::create(node, target, selector);
}

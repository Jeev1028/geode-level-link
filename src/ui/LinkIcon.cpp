#include "LinkIcon.hpp"

using namespace geode::prelude;

namespace {

// Design-space size of the icon composite, matching the part-sprites' real
// layout as extracted from the source artwork (see the positions below).
constexpr float ICON_W = 285.f;
constexpr float ICON_H = 157.f;

// The base part-sprites (link-base-*.png) are authored so that, after
// Geode's resource pipeline downsamples them, they should land at this
// diameter in points.
//
// In practice, measured directly against neighboring vanilla buttons in a
// real screenshot, every sprite loaded through Geode's "resources.sprites"
// pipeline here is rendering ~2.3x bigger than that theoretical size (a
// button requested at 88pt measured ~190-203px against an ~83px vanilla
// neighbor in the same shot). Rather than guess why, RENDER_BUG_FACTOR
// corrects for it empirically so requested sizes match reality.
constexpr float BASE_REF = 200.f;
constexpr float RENDER_BUG_FACTOR = 2.3f;

ccColor3B settingColor(const char* key) { return Mod::get()->getSettingValue<ccColor3B>(key); }

// Our source art is authored much larger than it's ever displayed (buttons
// render at well under 100pt from source textures hundreds of pixels
// across), which is a strong minification ratio. Plain bilinear minification
// (cocos2d's default, no mipmaps) doesn't box-average properly at ratios
// like that - it aliases/shimmers on fine detail (the chain links, gloss
// highlights), which is what was showing up as "pixelated" even at 1080p.
// Generating mipmaps and sampling trilinearly fixes that the same way
// vanilla GD textures already look smooth when scaled down.
//
// Desktop-only (confirmed fine on Windows via real screenshots): our source
// PNGs are not power-of-two (800x800, 328x328, etc.), and mipmap generation
// for NPOT textures is unreliable specifically on OpenGL ES (iOS/Android) -
// some GPU drivers silently produce garbage/solid-black textures instead of
// erroring. Confirmed via a real iPad screenshot: the whole button rendered
// as a solid black square there. Desktop GL doesn't have this restriction.
void smoothMinification(CCSprite* sprite) {
#if defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_MACOS)
    if (!sprite) return;
    auto tex = sprite->getTexture();
    if (!tex) return;
    tex->generateMipmap();
    ccTexParams params = {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    tex->setTexParameters(&params);
#else
    (void)sprite;
#endif
}

}  // namespace

CCNode* LinkIcon::createIcon() {
    // Layout (design units, origin bottom-left) - matches each part's real
    // position as extracted from the source artwork:
    //   left square:  centered (77.5, 78)
    //   right square: centered (211.5, 78)
    //   chain: its anchor is offset to the point in the source crop that
    //   lines up with the swap-circle's center, so positioning it there
    //   (same point as the circle/arrows) aligns it correctly - the chain
    //   image's own bounding box isn't centered on that point.
    //   swap-circle + arrows: centered (143, 79), at native size so the
    //   chain links stay visible on either side instead of being covered.
    auto container = CCNode::create();
    container->setContentSize({ICON_W, ICON_H});

    auto left = CCSprite::create("link-square-left.png"_spr);
    left->setColor(settingColor("icon-left-color"));
    left->setPosition({77.5f, 78.f});
    smoothMinification(left);
    container->addChild(left);

    auto right = CCSprite::create("link-square-right.png"_spr);
    right->setColor(settingColor("icon-right-color"));
    right->setPosition({211.5f, 78.f});
    smoothMinification(right);
    container->addChild(right);

    auto chain = CCSprite::create("link-chain.png"_spr);
    chain->setColor(settingColor("icon-chain-color"));
    chain->setAnchorPoint({0.482f, 0.302f});
    chain->setPosition({143.f, 79.f});
    smoothMinification(chain);
    container->addChild(chain);

    auto middle = CCSprite::create("link-swap-circle.png"_spr);
    middle->setColor(settingColor("icon-middle-color"));
    middle->setPosition({143.f, 79.f});
    smoothMinification(middle);
    container->addChild(middle);

    auto arrows = CCSprite::create("link-swap-arrows.png"_spr);
    arrows->setPosition({143.f, 79.f});
    smoothMinification(arrows);
    container->addChild(arrows);

    return container;
}

CCMenuItemSpriteExtra* LinkIcon::createButton(float diameter, CCObject* target,
                                              SEL_MenuHandler selector) {
    const float baseScale = diameter / (BASE_REF * RENDER_BUG_FACTOR);

    auto node = CCNode::create();
    node->setContentSize({diameter, diameter});
    node->setAnchorPoint({0.5f, 0.5f});
    node->ignoreAnchorPointForPosition(false);

    const CCPoint center{diameter / 2.f, diameter / 2.f};

    auto shadow = CCSprite::create("link-base-shadow.png"_spr);
    shadow->setScale(baseScale);
    shadow->setPosition(center);
    smoothMinification(shadow);
    node->addChild(shadow);

    auto ring = CCSprite::create("link-base-ring.png"_spr);
    ring->setScale(baseScale);
    ring->setPosition(center);
    smoothMinification(ring);
    node->addChild(ring);

    auto fill = CCSprite::create("link-base-fill.png"_spr);
    fill->setColor(settingColor("circle-color"));
    fill->setScale(baseScale);
    fill->setPosition(center);
    smoothMinification(fill);
    node->addChild(fill);

    // Icon container has default (0,0) anchor, so position its bottom-left
    // corner such that its scaled footprint ends up centered in the button.
    auto icon = createIcon();
    const float iconScale = (diameter * 0.72f) / (ICON_W * RENDER_BUG_FACTOR);
    icon->setScale(iconScale);
    icon->setPosition({center.x - (ICON_W * iconScale) / 2.f,
                       center.y - (ICON_H * iconScale) / 2.f});
    node->addChild(icon);

    return CCMenuItemSpriteExtra::create(node, target, selector);
}

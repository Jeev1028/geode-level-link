#pragma once

#include <Geode/Geode.hpp>

#include <optional>
#include <string>

// A reference to "the other" level a level is linked to.
struct LevelRef {
    std::string kind;  // "online" or "local"
    int id = 0;        // online level id (0 for local levels)
    std::string name;  // display name; primary key for local levels

    bool isOnline() const { return kind == "online"; }
};

// Persists level <-> level links inside the mod's savedata.
//
// Storage: one flat saved-value per level. Key   = "link/" + <levelKey>,
//                                          value = the linked level's key.
// A level key is "online:<id>" or "local:<name>". Links are written in both
// directions so one action toggles either way.
class LinkStore {
public:
    static LinkStore* get();

    // Stable lookup key for a level: "online:<id>" or "local:<name>".
    static std::string keyFor(GJGameLevel* level);

    // Build a LevelRef that points *at* the given level.
    static LevelRef refFor(GJGameLevel* level);

    std::optional<LevelRef> getLink(GJGameLevel* level);
    void setLink(GJGameLevel* from, const LevelRef& to);
    void removeLink(GJGameLevel* level);

private:
    static std::string savedKey(const std::string& levelKey);
    static std::string refToKey(const LevelRef& ref);
    static std::optional<LevelRef> keyToRef(const std::string& key);
};

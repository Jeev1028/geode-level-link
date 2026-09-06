#pragma once

#include <Geode/Geode.hpp>

#include <optional>
#include <string>

// A reference to "the other" level a level is linked to.
struct LevelRef {
    std::string kind;     // "online" or "local"
    int id = 0;           // online level id (0 for local levels)
    std::string name;     // display name; primary key for local levels

    bool isOnline() const { return kind == "online"; }
};

// Persists level <-> level links inside the mod's savedata.
// Links are stored two-directionally so one action toggles both ways.
class LinkStore {
public:
    static LinkStore* get();

    // Stable lookup key for a level: "online:<id>" or "local:<name>".
    static std::string keyFor(GJGameLevel* level);

    // Build a LevelRef that points *at* the given level.
    static LevelRef refFor(GJGameLevel* level);

    std::optional<LevelRef> getLink(GJGameLevel* level);

    // Links `from` <-> `to` (both directions).
    void setLink(GJGameLevel* from, const LevelRef& to);

    void removeLink(GJGameLevel* level);

private:
    matjson::Value& root();
    void save();
};

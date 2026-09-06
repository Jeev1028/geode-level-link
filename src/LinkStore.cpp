#include "LinkStore.hpp"

using namespace geode::prelude;

LinkStore* LinkStore::get() {
    static LinkStore inst;
    return &inst;
}

LevelRef LinkStore::refFor(GJGameLevel* level) {
    LevelRef r;
    // m_levelID is a geode::SeedValueRSV (anti-cheat wrapper) on GD 2.2 - .value() unwraps it.
    const int id = level->m_levelID.value();
    const bool local = level->m_levelType == GJLevelType::Editor || id <= 0;
    r.kind = local ? "local" : "online";
    r.id = local ? 0 : id;
    r.name = std::string(level->m_levelName);
    return r;
}

std::string LinkStore::refToKey(const LevelRef& ref) {
    return ref.isOnline() ? fmt::format("online:{}", ref.id) : fmt::format("local:{}", ref.name);
}

std::string LinkStore::keyFor(GJGameLevel* level) { return refToKey(refFor(level)); }

std::optional<LevelRef> LinkStore::keyToRef(const std::string& key) {
    const auto colon = key.find(':');
    if (colon == std::string::npos) return std::nullopt;

    LevelRef r;
    r.kind = key.substr(0, colon);
    const auto rest = key.substr(colon + 1);
    if (r.kind == "online") {
        r.id = numFromString<int>(rest).unwrapOr(0);
        r.name = fmt::format("Online #{}", r.id);
        if (r.id <= 0) return std::nullopt;
    } else if (r.kind == "local") {
        r.name = rest;
    } else {
        return std::nullopt;
    }
    return r;
}

std::string LinkStore::savedKey(const std::string& levelKey) { return "link/" + levelKey; }

std::optional<LevelRef> LinkStore::getLink(GJGameLevel* level) {
    const auto stored =
        Mod::get()->getSavedValue<std::string>(savedKey(keyFor(level)), std::string{});
    if (stored.empty()) return std::nullopt;
    return keyToRef(stored);
}

void LinkStore::setLink(GJGameLevel* from, const LevelRef& to) {
    const auto fromKey = keyFor(from);
    const auto toKey = refToKey(to);
    Mod::get()->setSavedValue<std::string>(savedKey(fromKey), toKey);
    Mod::get()->setSavedValue<std::string>(savedKey(toKey), fromKey);
}

void LinkStore::removeLink(GJGameLevel* level) {
    const auto fromKey = keyFor(level);
    const auto stored =
        Mod::get()->getSavedValue<std::string>(savedKey(fromKey), std::string{});
    if (!stored.empty()) {
        Mod::get()->setSavedValue<std::string>(savedKey(stored), std::string{});
    }
    Mod::get()->setSavedValue<std::string>(savedKey(fromKey), std::string{});
}

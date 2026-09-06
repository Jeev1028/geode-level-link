#include "LinkStore.hpp"

using namespace geode::prelude;

namespace {

matjson::Value toJson(const LevelRef& r) {
    matjson::Value v;
    v["kind"] = r.kind;
    v["id"] = r.id;
    v["name"] = r.name;
    return v;
}

LevelRef fromJson(const matjson::Value& v) {
    LevelRef r;
    // ADJUST: matjson v3 accessor names. If these don't compile, try
    // v["kind"].as<std::string>() / v["id"].as<int>().
    r.kind = v["kind"].asString().unwrapOr("online");
    r.id = static_cast<int>(v["id"].asInt().unwrapOr(0));
    r.name = v["name"].asString().unwrapOr("");
    return r;
}

std::string keyForRef(const LevelRef& r) {
    return r.isOnline() ? "online:" + std::to_string(r.id) : "local:" + r.name;
}

}  // namespace

LinkStore* LinkStore::get() {
    static LinkStore inst;
    return &inst;
}

LevelRef LinkStore::refFor(GJGameLevel* level) {
    LevelRef r;
    // ADJUST: on some bindings m_levelID is wrapped; use `.value()` if needed.
    const int id = level->m_levelID;
    const bool online = id > 0 && level->m_levelType == GJLevelType::Saved;
    r.kind = online ? "online" : "local";
    r.id = online ? id : 0;
    r.name = std::string(level->m_levelName);
    return r;
}

std::string LinkStore::keyFor(GJGameLevel* level) { return keyForRef(refFor(level)); }

matjson::Value& LinkStore::root() {
    static matjson::Value cache =
        Mod::get()->getSavedValue<matjson::Value>("links", matjson::Value::object());
    if (!cache.isObject()) cache = matjson::Value::object();
    return cache;
}

void LinkStore::save() { Mod::get()->setSavedValue("links", root()); }

std::optional<LevelRef> LinkStore::getLink(GJGameLevel* level) {
    auto& r = root();
    const auto key = keyFor(level);
    if (!r.contains(key)) return std::nullopt;
    return fromJson(r[key]);
}

void LinkStore::setLink(GJGameLevel* from, const LevelRef& to) {
    auto& r = root();

    // forward: from -> to
    r[keyFor(from)] = toJson(to);

    // reverse: to -> from
    r[keyForRef(to)] = toJson(refFor(from));

    save();
}

void LinkStore::removeLink(GJGameLevel* level) {
    auto& r = root();
    const auto key = keyFor(level);
    if (!r.contains(key)) return;

    const auto target = fromJson(r[key]);
    r.erase(keyForRef(target));  // ADJUST: matjson erase signature
    r.erase(key);
    save();
}

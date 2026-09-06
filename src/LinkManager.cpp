#include "LinkManager.hpp"

using namespace geode::prelude;

LinkManager* LinkManager::get() {
    static LinkManager inst;
    return &inst;
}

GJGameLevel* LinkManager::findLocal(const std::string& name) {
    auto mgr = LocalLevelManager::get();
    if (!mgr || !mgr->m_localLevels) return nullptr;
    for (auto lvl : CCArrayExt<GJGameLevel*>(mgr->m_localLevels)) {
        if (std::string(lvl->m_levelName) == name) return lvl;
    }
    return nullptr;
}

GJGameLevel* LinkManager::findCachedOnline(int id) {
    // ADJUST: returns a stored GJGameLevel* if the level was previously
    // downloaded/saved this session; otherwise nullptr.
    return GameLevelManager::sharedState()->getSavedLevel(id);
}

void LinkManager::requestSwitch(GJGameLevel* current) {
    const auto link = LinkStore::get()->getLink(current);
    if (!link) {
        Notification::create("This level has no linked level", NotificationIcon::Warning)->show();
        return;
    }

    float matchX = 0.f;
    if (Mod::get()->getSettingValue<bool>("match-position")) {
        if (auto pl = PlayLayer::get(); pl && pl->m_player1) {
            matchX = pl->m_player1->getPositionX();
        }
    }

    auto proceed = [this, link, matchX]() {
        m_pendingMatchX = matchX;

        if (link->isOnline()) {
            if (auto cached = findCachedOnline(link->id)) {
                doSwitch(cached);
                return;
            }
            Notification::create("Downloading linked level…", NotificationIcon::Loading, 0.f)
                ->show();
            m_switchAfterDownload = true;
            // ADJUST: delegate member + method names.
            GameLevelManager::sharedState()->m_levelDownloadDelegate = this;
            GameLevelManager::sharedState()->downloadLevel(link->id, false);
        } else {
            if (auto local = findLocal(link->name)) {
                doSwitch(local);
            } else {
                Notification::create("Linked local level not found", NotificationIcon::Error)
                    ->show();
            }
        }
    };

    if (Mod::get()->getSettingValue<bool>("confirm-switch")) {
        createQuickPopup(
            "Switch Level", "Switch to the <cy>linked</c> version of this level?", "Cancel",
            "Switch", [proceed](auto, bool second) {
                if (second) proceed();
            });
    } else {
        proceed();
    }
}

void LinkManager::levelDownloadFinished(GJGameLevel* level) {
    GameLevelManager::sharedState()->m_levelDownloadDelegate = nullptr;
    if (std::exchange(m_switchAfterDownload, false)) {
        doSwitch(level);
    }
}

void LinkManager::levelDownloadFailed(int reason) {
    GameLevelManager::sharedState()->m_levelDownloadDelegate = nullptr;
    m_switchAfterDownload = false;
    Notification::create(fmt::format("Linked level download failed ({})", reason),
                         NotificationIcon::Error)
        ->show();
}

void LinkManager::doSwitch(GJGameLevel* target) {
    if (!target) return;

    // ADJUST: PlayLayer::scene(level, useReplay, dontCreateObjects) — confirm
    // arg order/meaning against your bindings.
    auto scene = PlayLayer::scene(target, false, false);
    CCDirector::sharedDirector()->replaceScene(scene);

    if (m_pendingMatchX > 1.f && Mod::get()->getSettingValue<bool>("match-position")) {
        const float x = m_pendingMatchX;
        Loader::get()->queueInMainThread([x]() {
            auto pl = PlayLayer::get();
            if (!pl || !pl->m_player1) return;
            // EXPERIMENTAL: raw teleport only. Triggers / moving objects are not
            // simulated, so gameplay past this point is not physically accurate.
            // Do not use for legitimate leaderboard runs.
            pl->m_player1->setPositionX(x);
        });
    }
    m_pendingMatchX = 0.f;
}

void LinkManager::precache(const LevelRef& ref) {
    if (!ref.isOnline()) return;
    if (!Mod::get()->getSettingValue<bool>("precache")) return;
    if (findCachedOnline(ref.id)) return;
    GameLevelManager::sharedState()->downloadLevel(ref.id, false);
}

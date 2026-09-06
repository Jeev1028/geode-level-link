#include "LinkManager.hpp"

using namespace geode::prelude;

LinkManager* LinkManager::get() {
    static LinkManager inst;
    return &inst;
}

bool LinkManager::canSwitchNow() {
    auto pl = PlayLayer::get();
    if (!pl || !pl->m_level) return false;

    auto scene = CCScene::get();
    if (!scene || !scene->getChildByType<PauseLayer>(0)) return false;

    return LinkStore::get()->getLink(pl->m_level).has_value();
}

GJGameLevel* LinkManager::findLocal(const std::string& name) {
    auto mgr = LocalLevelManager::get();
    if (!mgr || !mgr->m_localLevels) return nullptr;
    for (auto lvl : CCArrayExt<GJGameLevel*>(mgr->m_localLevels)) {
        if (std::string(lvl->m_levelName) == name) return lvl;
    }
    return nullptr;
}

void LinkManager::requestSwitchFromCurrent() {
    auto pl = PlayLayer::get();
    if (!pl || !pl->m_level) return;
    this->requestSwitch(pl->m_level);
}

void LinkManager::requestSwitch(GJGameLevel* current) {
    const auto link = LinkStore::get()->getLink(current);
    if (!link) {
        Notification::create("This level has no linked level", NotificationIcon::Warning)->show();
        return;
    }

    m_pendingMatchX = 0.f;
    if (Mod::get()->getSettingValue<bool>("match-position")) {
        if (auto pl = PlayLayer::get(); pl && pl->m_player1) {
            m_pendingMatchX = pl->m_player1->getPositionX();
        }
    }

    auto start = [this, ref = *link]() {
        if (ref.isOnline()) {
            if (auto cached = GameLevelManager::get()->getSavedLevel(ref.id)) {
                this->doSwitch(cached);
                return;
            }
            Notification::create("Downloading linked level…", NotificationIcon::Loading, 0.f)
                ->show();
            m_switchAfterDownload = true;
            GameLevelManager::get()->m_levelDownloadDelegate = this;
            GameLevelManager::get()->downloadLevel(ref.id, false, 0);
        } else {
            if (auto local = findLocal(ref.name)) {
                this->doSwitch(local);
            } else {
                Notification::create("Linked local level not found", NotificationIcon::Error)
                    ->show();
            }
        }
    };

    if (Mod::get()->getSettingValue<bool>("confirm-switch")) {
        createQuickPopup("Switch Level", "Switch to the <cy>linked</c> version of this level?",
                         "Cancel", "Switch", [start](FLAlertLayer*, bool btn2) {
                             if (btn2) start();
                         });
    } else {
        start();
    }
}

void LinkManager::levelDownloadFinished(GJGameLevel* level) {
    GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
    if (std::exchange(m_switchAfterDownload, false)) {
        this->doSwitch(level);
    }
}

void LinkManager::levelDownloadFailed(int reason) {
    GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
    m_switchAfterDownload = false;
    Notification::create(fmt::format("Linked level download failed ({})", reason),
                         NotificationIcon::Error)
        ->show();
}

void LinkManager::doSwitch(GJGameLevel* target) {
    if (!target) return;
    const float matchX = std::exchange(m_pendingMatchX, 0.f);

    Loader::get()->queueInMainThread([target, matchX]() {
        CCDirector::sharedDirector()->replaceScene(PlayLayer::scene(target, false, false));

        if (matchX > 1.f) {
            // EXPERIMENTAL: raw teleport a couple of frames after load. Triggers /
            // moving objects are not simulated - visual only, never for leaderboard runs.
            Loader::get()->queueInMainThread([matchX]() {
                Loader::get()->queueInMainThread([matchX]() {
                    if (auto pl = PlayLayer::get(); pl && pl->m_player1) {
                        pl->m_player1->setPositionX(matchX);
                    }
                });
            });
        }
    });
}

void LinkManager::precache(const LevelRef& ref) {
    if (!ref.isOnline()) return;
    if (!Mod::get()->getSettingValue<bool>("precache")) return;

    auto glm = GameLevelManager::get();
    if (glm->getSavedLevel(ref.id)) return;
    if (glm->m_levelDownloadDelegate) return;  // don't stomp an in-progress download

    glm->downloadLevel(ref.id, false, 0);
}

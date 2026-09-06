# Level Link (Geode mod)

Quickly switch between two linked versions of a level (e.g. a StartPos copy and
the rated version) from the pause menu, via a rebindable key or a button.

## Status

This is a **working scaffold**, not a finished/tested mod. All the structure,
hooks, storage and UI are here; a few GD-internal symbols are version-sensitive
and should be checked against your installed Geode SDK headers before the first
successful build. Search the source for `ADJUST:` — each marks one such spot:

| Spot | File | What to verify |
|---|---|---|
| `PlayLayer::scene(level, false, false)` | `src/LinkManager.cpp` | arg order/meaning in your bindings |
| `GJGameLevel::m_levelID` type | `src/LinkStore.cpp` | plain `int` vs wrapped |
| `GameLevelManager::m_levelDownloadDelegate` / `getSavedLevel` | `src/LinkManager.cpp` | member + method names |
| `matjson` `asString` / `asInt` / `erase` / `object()` | `src/LinkStore.cpp` | matjson v3 API surface |
| `BindManager::registerBindable` category arg | `src/main.cpp` | `Category` type vs string |
| `right-button-menu` node id | `src/hooks/PauseLayer.cpp` | Geode's PauseLayer node IDs |
| `LevelInfoLayer::init(level, challenge)` | `src/hooks/LevelInfoLayer.cpp` | signature for your GD version |

## Layout

```
src/
  main.cpp                  entry point + keybind registration
  LinkStore.{hpp,cpp}       persistent level<->level links (mod savedata)
  LinkManager.{hpp,cpp}     fetch / download / scene-swap logic
  ui/LinkPopup.{hpp,cpp}    "link this level" popup (online ID or local pick)
  ui/LocalPickPopup.{hpp,cpp} scrollable list of your local levels
  hooks/LevelInfoLayer.cpp  adds the link button + background precache
  hooks/PauseLayer.cpp      adds the swap button + pause-only keybind listener
```

## Build

Requires the [Geode CLI](https://docs.geode-sdk.org/getting-started/) and SDK.

```bash
# one-time
geode sdk install
geode sdk install-binaries
export GEODE_SDK=$(geode sdk path)

# build
geode build
```

Or with CMake directly:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo
```

The resulting `jeevin.level-link.geode` can be dropped into
`<GD>/geode/mods/` or installed with `geode install`.

## Design decisions

- **Switching = a fresh `PlayLayer` for the linked level.** Completions of the
  real/rated level therefore submit to leaderboards normally.
- **Two-way links.** Linking A→B also writes B→A, so one key toggles both ways.
- **Precache.** Opening a linked level's info page kicks off a background
  download of its partner so the pause-menu swap has no loading hitch.
- **Match Position is opt-in and experimental** — GD has no real "seek", so it
  is a raw teleport with no physics/trigger simulation.

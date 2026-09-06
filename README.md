# Level Link (Geode mod)

Quickly switch between two linked versions of a level (e.g. a StartPos copy and
the rated version) from the pause menu, via a rebindable key or a button.

**Status:** builds clean and loads against **Geode 5.10.1 / GD 2.2081**.
`jeevin.level-link.geode` is produced and installed by `geode build`.

## How it works

- On a level's info page there's a link button (left side). Tap it and either
  enter an **online level ID** or pick one of **your local levels**.
- Links are stored per-level in the mod's savedata and are **two-directional**,
  so the same action toggles either way.
- While **paused** in a linked level, press the **Switch to Linked Level**
  keybind (default `T`, rebindable in the mod's settings) or click the swap
  button in the pause menu's right-hand button group.
- Switching creates a fresh `PlayLayer` for the linked level, so completions of
  the real/rated version still submit to leaderboards normally.
- Opening a linked level's info page pre-downloads its partner in the
  background so the pause-menu swap is instant.
- `Match Position` (setting, off by default) is experimental and visual-only —
  a raw teleport with no physics/trigger simulation. Do not use it for
  legitimate leaderboard runs.

## Layout

```
src/
  main.cpp                    keybind listener (native Geode 5 keybind setting)
  LinkStore.{hpp,cpp}         persistent level<->level links
  LinkManager.{hpp,cpp}       resolve / download / scene-swap logic
  ui/LinkPopup.{hpp,cpp}      "link this level" popup
  ui/LocalPickPopup.{hpp,cpp} scrollable list of local levels
  hooks/LevelInfoLayer.cpp    link button + background precache
  hooks/PauseLayer.cpp        pause-menu swap button
```

## Build

Requires the Geode SDK (`geode sdk install`), CMake, Ninja, and MSVC.

```bash
# from a shell where MSVC is on PATH (run vcvars64.bat first) and GEODE_SDK is set
geode build --ninja
```

Or open the folder in a "Developer" prompt / VS and use the default generator.
`geode run` launches GD with the mod installed.

## Notes

- `geode.node-ids` is a required dependency (used to find the pause menu / level
  page menus by ID). It is fetched automatically from the mod index at build.
- The pause-menu keybind only fires when a `PauseLayer` is present in the
  running scene and the current level has a link.

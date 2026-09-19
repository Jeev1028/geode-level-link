# Level Link (Geode mod)

Quickly switch between two linked versions of a level (e.g. a StartPos copy and
the rated version) from the pause menu or the level-complete screen, via a
rebindable key or a button.

**Status:** builds clean and loads against **Geode 5.10.1 / GD 2.2081**.
`jeevin.level-link.geode` is produced and installed by `geode build`.
Published releases (Windows/macOS/Android) at
https://github.com/Jeev1028/geode-level-link/releases.

## How it works

- On a level's info page (`LevelInfoLayer`) or your own created-level screen
  (`EditLevelLayer`) there's a cyan link button. Tap it and either enter an
  **online level ID** or pick one of **your local levels**.
- Links are stored per-level in the mod's savedata and are **two-directional**,
  so the same action toggles either way.
- While **paused**, or on the **level-complete ("LEVEL COMPLETE")** screen
  after finishing a linked level, press the **Switch to Linked Level** keybind
  (default `T`, rebindable in the mod's settings) or click the green swap
  button.
- Switching creates a fresh `PlayLayer` for the linked level, so completions of
  the real/rated version still submit to leaderboards normally.
- Opening a linked level's info page pre-downloads its partner in the
  background so the swap is instant.
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
  hooks/LevelInfoLayer.cpp    link button + background precache (online level page)
  hooks/EditLevelLayer.cpp    link button (your own created-level screen)
  hooks/PauseLayer.cpp        swap button (pause menu)
  hooks/EndLevelLayer.cpp     swap button (level-complete screen)
```

## Build

Requires the Geode SDK (`geode sdk install`), CMake, Ninja, and MSVC (Windows)
or Xcode (macOS/iOS).

```bash
# Windows, from a shell where MSVC is on PATH (run vcvars64.bat first) and
# GEODE_SDK is set
geode build --ninja
```

Or open the folder in a "Developer" prompt / VS and use the default generator.
`geode run` launches GD with the mod installed.

macOS/iOS: `geode build` from Xcode's command line tools once the SDK is
installed (`geode sdk install`, `geode sdk install-binaries`).

## CI / Releases

`.github/workflows/build.yml` builds Windows, macOS, Android32, Android64 (and
iOS, allowed to fail — GitHub's `macos-latest` runner currently has a
CMake/Xcode-26 SDK-layout bug unrelated to this mod's code) on every push, and
publishes a GitHub Release with the combined `.geode` when a `v*` tag is
pushed:

```bash
git tag vX.Y.Z
git push origin vX.Y.Z
```

## Notes

- `geode.node-ids` is a required dependency (used to find menus by ID in
  `LevelInfoLayer`/`EditLevelLayer`/`PauseLayer`/`EndLevelLayer`). It is
  fetched automatically from the mod index at build time.
- The keybind/button only fire when a `PauseLayer` or `EndLevelLayer` is
  present in the running scene and the current level has a link.

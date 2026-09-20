# v1.2.0

- All buttons now use the mod's own bundled icon instead of a borrowed GD
  sprite (the previous icon, `d_link_01_001.png`, turned out to be an
  editor-only decoration sprite whose texture sheet isn't loaded outside the
  level editor, so it silently rendered as a blank placeholder)
- Fixed the level-complete screen's switch button/keybind not working in some
  contexts (e.g. the level editor's test-play completion popup) - overlay
  detection now searches the whole scene tree instead of only direct children
- The level-complete button no longer overlaps the run summary text; it's now
  anchored to the bottom-right corner

# v1.1.0

- Added support for the level-complete (100%) screen: the swap button and
  keybind now work there too, not just the pause menu
- Unified all buttons to a single chain-link icon (cyan = manage a link,
  green = execute a switch) instead of a mismatched pair of icons
- Added a proper mod logo

# v1.0.0

- Initial release
- Link two levels (online or one of your own local levels) from the level
  info page or your own created-level screen
- Swap between linked levels from the pause menu, via a button or a
  rebindable keybind (default `T`)
- Links are two-directional and stored per level
- Optional experimental "Match Position" teleport (off by default, visual
  only, not for leaderboard runs)
- Background pre-download of linked online levels so switching is instant

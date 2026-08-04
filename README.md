# markdownpart-bh

A fork of KDE's [markdownpart](https://invent.kde.org/utilities/markdownpart) - the
Markdown viewer KPart, with blackjack. And configurable fonts. Mostly configurable fonts.

## Why this fork exists

The stock part renders Markdown via `QTextDocument::setMarkdown()`. Qt's Markdown
importer stamps code spans and code blocks with an **explicitly sized** fixed-pitch
font at import time. In practice that means:

* Ctrl+wheel zoom (Qt's built-in `QTextEdit` zoom) scales everything **except**
  code, which stays frozen at its imported size.
* Changing the system fixed-width font (kdeglobals `fixed=`) doesn't reach the
  preview either.
* The part itself has no font or zoom configuration surface at all.

If you've ever zoomed a Markdown preview in Kate and watched the code blocks
refuse to move, that's this.

## What the fork adds

* **Configurable fonts** - body and monospace family/size, via a config file.
* **Zoom that scales code** - the part owns Ctrl+wheel zoom and rescales body
  and monospace fonts together. Sub-notch wheel deltas (touchpads, forwarded
  remote-desktop scrolls) are accumulated instead of dropped.
* **Persistent zoom** - the zoom level survives restarts.
* **Zoom UI** - a Zoom submenu (with current percentage) in the context menu,
  plus `view_zoom_in` / `view_zoom_out` / `view_actual_size` actions declared in
  the part's XMLGUI (View menu + toolbar) for hosts that merge part GUIs. The
  actions deliberately carry no default shortcuts - hosts like Kate already own
  Ctrl+= / Ctrl+- / Ctrl+0 and ambiguous-shortcut warnings help no one.

## Configuration

`~/.config/markdownpartbhrc`:

```ini
[Fonts]
bodyFamily=          # empty = application default
bodySize=11          # 0 = application default
monoFamily=Hack      # default: monospace
monoSize=13          # 0 = same as body size

[View]
zoom=1.0             # written automatically as you zoom
```

Font settings are read at part creation (restart the host app after editing);
zoom applies live.

## Installing via an AI agent

If you use a coding agent (Claude Code, Codex, etc.), point it at
[AGENTS.md](./AGENTS.md) - a complete clone-to-verified-working install guide
written for agents, including compatibility checks and the failure modes we hit
so yours doesn't have to.

## Branches

* **`release/23.08`** - Qt5/KF5, for Kate ≤ 23.x (e.g. stock Ubuntu 24.04).
  Verified end-to-end.
* **`qt6`** - the same features on upstream master, Qt6/KF6, for Kate 24+.
  Compiles clean (Qt 6.8 / KF 6.10); runtime reports welcome.

Prebuilt amd64 debs for both are on the
[Releases page](https://github.com/jaidhyani/markdownpart-bh/releases).

## Building (Qt5/KF5 branch shown)

The `release/23.08` branch builds against Qt5/KF5, matching e.g. Kate 23.08 on
Ubuntu 24.04:

```sh
sudo apt install cmake extra-cmake-modules qtbase5-dev libkf5parts-dev \
    libkf5i18n-dev libkf5coreaddons-dev libkf5widgetsaddons-dev gettext
cmake -B build -DCMAKE_BUILD_TYPE=Release -DQT_MAJOR_VERSION=5
cmake --build build -j$(nproc)
sudo cmake --install build
```

Installs as `markdownpartbh.so` alongside the stock part; it declares
`InitialPreference: 15` so KParts-using hosts (like Kate's Document Preview
plugin) pick it over the stock part (preference 0) automatically. No need to
uninstall stock markdownpart.

## Using with Kate

Enable the **Document Preview** plugin (Settings → Configure Kate → Plugins),
open a Markdown file, click **Preview** on the right sidebar. Ctrl+wheel over
the preview zooms; right-click for the Zoom menu.

## License

LGPL-2.1-or-later, same as upstream. Original code copyright
Friedrich W. H. Kossebau and KDE contributors; fork additions 2026 by
Jai Dhyani and Claude (Anthropic's Claude Fable 5, working via Claude Code).

Upstream issues belong in the [KDE tracker](https://bugs.kde.org/enter_bug.cgi?product=markdownpart);
issues with the fork's additions belong here.

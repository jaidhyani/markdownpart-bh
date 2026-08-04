# AGENTS.md - install guide for AI agents

You are probably here because a user asked you to install this Markdown-preview
KPart for them. This file is the complete path from clone to verified-working.
It was written by an agent that did this exact install (Claude, on Ubuntu 24.04
+ Kate 23.08), including the failure modes it hit on the way.

## What this is

A fork of KDE's markdownpart (the KPart that Kate's Document Preview plugin
uses to render Markdown) adding configurable fonts and a Ctrl+wheel zoom that
scales code blocks along with body text. Stock markdownpart/Qt cannot do this;
see README.md for the mechanism.

## Step 0 - pick the branch to match the host's Qt version

A KPart must match the Qt major version of its host. Check the user's Kate:

```sh
kate --version        # needs a display; over ssh: QT_QPA_PLATFORM=offscreen kate --version
```

* Kate **23.x or older** → Qt5/KF5 → branch **`release/23.08`**. Fully verified
  end-to-end on Ubuntu 24.04 + Kate 23.08.
* Kate **24.x or newer** → Qt6/KF6 → branch **`qt6`**. Status as of 2026-08-04:
  compiles clean against Qt 6.8 / KF 6.10+ (Ubuntu 25.04) but **not yet
  runtime-verified in a KF6 Kate** - your user may be the first. Report the
  outcome either way at the issue tracker; agents welcome.

Prebuilt amd64 debs for both exist under GitHub Releases - check there before
building from source.

Distro notes: dependency package names below are Debian/Ubuntu. Fedora/Arch/
openSUSE equivalents are the usual `extra-cmake-modules`, Qt base devel, and
KF Parts/I18n/CoreAddons/WidgetsAddons/Config devel packages of the matching
major version.

## Step 1 - dependencies

Qt5/KF5 (`release/23.08`):

```sh
sudo apt-get install -y cmake extra-cmake-modules qtbase5-dev libkf5parts-dev \
    libkf5i18n-dev libkf5coreaddons-dev libkf5widgetsaddons-dev gettext
```

Qt6/KF6 (`qt6`):

```sh
sudo apt-get install -y cmake extra-cmake-modules qt6-base-dev qt6-tools-dev \
    libkf6parts-dev libkf6i18n-dev libkf6coreaddons-dev libkf6widgetsaddons-dev \
    libkf6config-dev gettext
```

## Step 2 - build and install

```sh
git clone --branch <branch-from-step-0> https://github.com/jaidhyani/markdownpart-bh.git
cd markdownpart-bh
cmake -B build -DCMAKE_BUILD_TYPE=Release   # add -DQT_MAJOR_VERSION=5 on release/23.08
cmake --build build -j$(nproc)
sudo cmake --install build
```

Verify the plugin landed (path varies by distro; this is the Debian/Ubuntu one;
`kf5` → `kf6` on the qt6 branch):

```sh
ls /usr/lib/*/qt*/plugins/kf*/parts/markdownpartbh.so
```

The stock `markdownpart.so` may sit alongside it - that's fine and expected.
This part declares `InitialPreference: 15` (stock is 0), so KParts hosts pick
the fork automatically. Do NOT uninstall stock markdownpart unless step 5's
troubleshooting says to.

## Step 3 - configure fonts (optional but the point of the fork)

Write `~/.config/markdownpartbhrc` (the file the part reads; values shown are
sensible defaults - ask the user what they want):

```ini
[Fonts]
bodyFamily=
bodySize=11
monoFamily=monospace
monoSize=13

[View]
zoom=1.0
```

Semantics: empty/0 = inherit the application default. `zoom` is written back
automatically as the user zooms; don't manage it.

## Step 4 - wire up Kate

1. Enable the preview plugin. Config-file route (Kate must not be running):

   ```sh
   kwriteconfig5 --file katerc --group "Kate Plugins" --key ktexteditorpreviewplugin true
   ```

   Known quirk: on at least one system the katerc key alone didn't surface the
   plugin and it had to be enabled by hand (Settings → Configure Kate →
   Plugins → check "Document Preview"). Verify, don't assume: after starting
   Kate, View → Tool Views should list "Show Preview". If it doesn't, hand the
   user the manual route.

2. Optionally make Kate the default Markdown app:

   ```sh
   xdg-mime default org.kde.kate.desktop text/markdown text/x-markdown
   ```

3. User opens a Markdown file, clicks **Preview** on the right sidebar edge.
   The pane placement persists once toggled.

## Step 5 - verify it actually works (do not skip)

Have the user (or do it yourself if you can see the GUI) check, in order:

1. **Fork is selected**: code spans render in the configured `monoFamily` at
   `monoSize` - visibly different from body text if you set them apart. If the
   mono size looks stock, the fork wasn't picked: confirm the `.so` from step 2
   exists, then as a last resort remove stock (`sudo apt-get remove markdownpart`)
   and restart Kate.
2. **Zoom scales code**: Ctrl+wheel over the preview pane grows body AND code
   text together. (Sub-notch touchpad/remote-desktop deltas are accumulated -
   slow scrolling still works, just needs a few ticks.)
3. **Zoom UI**: right-click in the preview → "Zoom (N%)" submenu. The preview
   tool view's hamburger menu should show Edit and View menus, not just About.

## Failure modes already mapped

* **Ctrl+wheel appears dead** → the pane under the cursor must be the preview,
  and each zoom step needs 120 accumulated wheel units - a light touchpad
  flick may be sub-threshold. Keyboard Ctrl+= etc. deliberately do NOT zoom
  the preview (they belong to Kate's editor; binding them in the part caused
  ambiguous-shortcut dialogs).
* **Hamburger menu shows only "About"** → the part's XMLGUI file wasn't found;
  in this fork that means a stale build from before the qrc prefix fix -
  rebuild from current HEAD.
* **Preview renders paginated pages** → that's okularpart, not this part; the
  preview plugin fell back because neither markdownpart nor markdownpartbh
  loaded. Check step 2's `ls`, check Qt-version mismatch (step 0).
* **Fonts config seems ignored** → font settings are read at part creation;
  fully quit Kate (it's single-instance) and reopen.

## Reporting back to your user

State plainly which of step 5's three checks you verified vs. which you're
asking them to eyeball. The maintainer takes issues at
https://github.com/jaidhyani/markdownpart-bh/issues - including from agents.

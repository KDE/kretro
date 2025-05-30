<!--
- SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>
- SPDX-License-Identifier: FSFAP
-->

# KRetro - Libretro Emulation Frontend for Plasma

Play your favourite games across Plasma platforms: in your pocket, desktop or TV!

> KRetro is a work in progress! Contributions are welcome!

<div style="display: flex; flex-direction: row; gap: 10px; overflow-x: auto; margin: 20px 0;">
  <img src="./screenshots/screenshot-home.png" alt="KRetro Game Library" width="30%" />
  <img src="./screenshots/screenshot-desktop.png" alt="KRetro Desktop Player" width="30%" />
  <img src="./screenshots/screenshot-mobile.png" alt="KRetro Mobile Player" width="30%" />
</div>

## Cores

Libretro cores (which are .so files) are dynamically loaded from your system (by default, from `/usr/lib/libretro`).
Therefore you will need them installed either from your distro's package manager (ex. "pacman -S libretro"), or you will need to download them yourselves.

> You can find a [bundle of x86_64 libretro cores](http://buildbot.libretro.com/nightly/linux/x86_64/RetroArch_cores.7z) from the libtretro buildbot, as well as [individual cores for armv7+](http://buildbot.libretro.com/nightly/linux/armv7-neon-hf/latest/).

The following consoles are currently supported:

- GBA
- NES
- SNES
- SMS
- Genesis/MD

There is also a built in demo core for x86_64 and arm64, 2048_libretro.

## Building and Testing
It is recommend to use `kde-builder`. Add the following to your `~/.config/kde-builder/config.yaml` file:

```yaml
project kretro:
  repository: git@invent.kde.org:seshpenguin/kretro.git
```
Then, you can build kretro: `kde-builder kretro`

To run kretro, first source the prefix, then run the app:

```bash
source ~/kde/build/kretro/prefix.sh
kretro

# To test the mobile player:
QT_QUICK_CONTROLS_MOBILE=1 QT_QUICK_CONTROLS_STYLE=org.kde.breeze kretro
```
# SPDX-FileCopyrightText: 2025 Seshan Ravikumar <seshan@sineware.ca>
# SPDX-License-Identifier: BSD-3-Clause

pkgname=kretro
pkgver=1.0.0_git
pkgrel=2
pkgdesc="Libretro frontend for Plasma"
arch="all !ppc64le !s390x !armhf !riscv64"
url="https://invent.kde.org/games/kretro.git"
license="GPL-2.0"
depends="
	kirigami
	kirigami-addons
	sdl3
	"
makedepends="
    extra-cmake-modules
	kconfig-dev
	ki18n-dev
	kirigami-addons-dev
	kirigami-dev
	qt6-qtbase-dev
	qt6-qtmultimedia-dev
	qt6-qtsvg-dev
	samurai
    sdl3-dev
	"
source="https://invent.kde.org/games/kretro.git"
options="!check"

build() {
    cmake -B $builddir -G Ninja \
		-DCMAKE_BUILD_TYPE=RelWithDebInfo \
		-DCMAKE_INSTALL_PREFIX=/usr
	cmake --build $builddir
}

package() {
    DESTDIR="$pkgdir" cmake --install build

}
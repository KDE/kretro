pkgname=kretro
pkgver=1.0.0_git$GIT_VERNUM
pkgrel=0
pkgdesc="KRetro Plasma Mobile Retro Game Player"
arch="all !ppc64le !s390x !armhf !riscv64"
url="https://sineware.ca/labs"
license="GPL-2.0"
depends="
	kirigami2
	"
makedepends="
    extra-cmake-modules
	kconfig-dev
	kcoreaddons-dev
	kdbusaddons-dev
	ki18n-dev
	kio-dev
	kirigami-addons-dev
	kirigami2-dev
	knotifications-dev
	qqc2-desktop-style-dev
	qt5-qtbase-dev
	qtkeychain-dev
	qt5-qtmultimedia-dev
	qt5-qtquickcontrols2-dev
	qt5-qtsvg-dev
	qt5-qtwebsockets-dev
	samurai
    alsa-lib-dev
	"
source="https://invent.kde.org/seshpenguin/kretro.git"
options="!check"

build() {
    cmake -B build -G Ninja \
		-DCMAKE_BUILD_TYPE=RelWithDebInfo \
		-DCMAKE_INSTALL_PREFIX=/usr
	cmake --build build
}

package() {
    DESTDIR="$pkgdir" cmake --install build

}
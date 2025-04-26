pkgname=kretro
pkgver=1.0.0_git$GIT_VERNUM
pkgrel=0
pkgdesc="Libretro frontend for Plasma"
arch="all !ppc64le !s390x !armhf !riscv64"
url="https://sineware.ca/labs"
license="GPL-2.0"
depends="
	kirigami
	kirigami-addons
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
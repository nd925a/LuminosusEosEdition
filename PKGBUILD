#Maintainer: Nate DeMare <24489063+nd925a@users.noreply.github.com> 
pkgname=luminosus
_uspkgname=LuminosusEosEdition
pkgver=1.3.0
pkgrel=1
pkgdesc="A \"companion app\" for consoles of the ETC Eos family."
arch=('i686' 'x86_64')
url="https://www.luminosus.org/"
license=('Custom')
groups=()
depends=('qt5-quickcontrols' 'qt5-quickcontrols2' 'qt5-graphicaleffects')
makedepends=('qt5-base' 'qt5-declarative' 'qt5-networkauth' 'qt5-multimedia' 'qt5-svg' 'libvlc-qt')
optdepends=()
options=()
install=
changelog=
source=("https://github.com/ETCLabs/LuminosusEosEdition/archive/v${pkgver}.tar.gz")
md5sums=("1209757dd5a55fcf4cd82141ddaa0140")

build() {
   cd "$srcdir/$_uspkgname-$pkgver"
   qmake src/luminosus.pro
   make
}

package() {
   cd "$srcdir/$_uspkgname-$pkgver"
   make DESTDIR="$pkgdir" install
}


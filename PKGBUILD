#Maintainer: Nate DeMare <24489063+nd925a@users.noreply.github.com> 
pkgname=luminosus
pkgver=1.3.0
pkgrel=1
pkgdesc="A \"companion app\" for consoles of the ETC Eos family."
arch=('i686' 'x86_64')
url="https://www.luminosus.org/"
license=('Custom')
groups=()
depends=()
makedepends=()
optdepends=()
options=()
install=
changelog=
source=("https://github.com/ETCLabs/LuminosusEosEdition/archive/v${pkgver}.tar.gz")
md5sums=("1209757dd5a55fcf4cd82141ddaa0140")

build() {
   cd "$pkgname-$pkgver"
   qmake src/
   make
}

package() {
   cd "$pkgname-$pkgver"
   make DESTDIR="$pkgdir/" install
}


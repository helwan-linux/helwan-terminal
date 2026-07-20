# Maintainer: Saeed Badreldin <helwanlinux@gmail.com>

pkgname=helwan-terminal
pkgver=0.1.4
pkgrel=6
pkgdesc="A powerful and customizable terminal emulator for Helwan Linux."
arch=('x86_64')
url="https://github.com/helwan-linux/helwan-terminal"
license=('GPL3')
depends=('gtk3' 'vte3' 'glib2')
makedepends=('meson' 'ninja')

source=("$pkgname::git+file://$PWD")
sha256sums=('SKIP')

build() {
  cd "$srcdir/$pkgname"
  meson setup build --prefix=/usr
  ninja -C build
}

package() {
  cd "$srcdir/$pkgname"
  DESTDIR="${pkgdir}" ninja -C build install

  # حذف ملف gschemas.compiled لو اتولد
  rm -f "${pkgdir}/usr/share/glib-2.0/schemas/gschemas.compiled"
}

post_install() {
  glib-compile-schemas /usr/share/glib-2.0/schemas/
}

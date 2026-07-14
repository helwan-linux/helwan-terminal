# Maintainer: Saeed <your_email@example.com>

pkgname=hel-terminal
pkgver=0.1.2
pkgrel=3
pkgdesc="A powerful and customizable terminal emulator for Helwan Linux."
arch=('x86_64')
url="https://github.com/helwan-linux/helwan-terminal"
license=('GPL3')
depends=('gtk3' 'vte3' 'glib2')
makedepends=('meson' 'ninja' 'git')

source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  cd "helwan-terminal"
  meson setup build . --prefix=/usr --reconfigure
  ninja -C build
}

package() {
  cd "helwan-terminal"
  DESTDIR="${pkgdir}" ninja -C build install

  # تثبيت الملف التنفيذي
  install -Dm755 build/helwan-terminal "${pkgdir}/usr/bin/hel-terminal"

  # نسخ ملف gschema
  install -Dm644 helwan-terminal.gschema.xml \
    "${pkgdir}/usr/share/glib-2.0/schemas/helwan-terminal.gschema.xml"

  # Compile للـ schemas أثناء التثبيت
  glib-compile-schemas "${pkgdir}/usr/share/glib-2.0/schemas/"
}

# Maintainer: Saeed Badreldin <helwanlinux@gmail.com>

pkgname=hel-terminal
pkgver=0.1.5
pkgrel=8
pkgdesc="A powerful and customizable terminal emulator for Helwan Linux."
arch=('x86_64')
url="https://github.com/helwan-linux/helwan-terminal"
license=('GPL3')

depends=('gtk3' 'vte3' 'glib2' 'hicolor-icon-theme' 'desktop-file-utils')
makedepends=('meson' 'ninja')

source=()
sha256sums=()

prepare() {
    cp -a "${startdir}/helwan-terminal" "${srcdir}/helwan-terminal"
}

build() {
    cd "${srcdir}/helwan-terminal"

    meson setup build \
        --prefix=/usr \
        --buildtype=release

    ninja -C build
}

package() {
    cd "${srcdir}/helwan-terminal"

    DESTDIR="${pkgdir}" ninja -C build install

    rm -f "${pkgdir}/usr/share/glib-2.0/schemas/gschemas.compiled"
}

post_install() {
    update-desktop-database -q /usr/share/applications
    gtk-update-icon-cache -q -t -f /usr/share/icons/hicolor
    glib-compile-schemas /usr/share/glib-2.0/schemas
}

post_upgrade() {
    post_install
}

post_remove() {
    post_install
}

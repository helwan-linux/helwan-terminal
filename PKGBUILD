# Maintainer: Saeed <your_email@example.com>

pkgname=hel-terminal
pkgver=0.1.3
pkgrel=4
pkgdesc="A powerful and customizable terminal emulator for Helwan Linux."
arch=('x86_64')
url="https://github.com/helwan-linux/helwan-terminal"
license=('GPL3')
depends=('gtk3' 'vte3' 'glib2')
makedepends=('meson' 'ninja' 'git')

source=("git+${url}.git")
sha256sums=('SKIP')

build() {
  # الانتقال للمجلد الذي تم استنساخه
  cd "$srcdir/helwan-terminal"

  # البحث عن ملف meson.build وتحديد المجلد الذي يحتوي عليه
  # سنقوم بإنشاء مجلد build في المكان الذي يوجد به ملف meson.build
  if [ -f "meson.build" ]; then
    meson setup build --prefix=/usr
  else
    # إذا كان meson.build داخل مجلد فرعي
    find . -name meson.build -exec dirname {} \; | xargs -I {} meson setup build {} --prefix=/usr
  fi

  ninja -C build
}

package() {
  cd "$srcdir/helwan-terminal"
  
  # تثبيت الملفات عبر ninja
  DESTDIR="${pkgdir}" ninja -C build install

  # تثبيت الملف التنفيذي
  install -Dm755 build/helwan-terminal "${pkgdir}/usr/bin/hel-terminal"

  # نسخ ملف الإعدادات
  install -Dm644 helwan-terminal/data/helwan-terminal.gschema.xml \
    "${pkgdir}/usr/share/glib-2.0/schemas/helwan-terminal.gschema.xml"

  # حذف ملف الفهرس إذا وُجد في مجلد التثبيت لكي لا يتعارض مع النظام
  rm -f "${pkgdir}/usr/share/glib-2.0/schemas/gschemas.compiled"
}

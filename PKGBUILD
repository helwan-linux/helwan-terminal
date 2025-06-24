# Maintainer: Your Name <your_email@example.com>
# Replace "Your Name" and "your_email@example.com" with your actual name and email.

pkgname=hel-terminal
pkgver=0.1.0 # هذا الإصدار يجب أن يتطابق مع 'version' في ملف PKGBUILD الخاص بك و Releases على GitHub
pkgrel=1
pkgdesc="A powerful and customizable terminal emulator for Helwan Linux."
arch=('x86_64')
url="https://github.com/helwan-linux/helwan-terminal"
license=('GPL3') # تأكد من أن ترخيص مشروعك هو GPL3
depends=('gtk3' 'vte3' 'glib2') # التبعيات الأساسية لتشغيل hel-terminal
makedepends=('meson' 'ninja' 'git') # التبعيات اللازمة للبناء، أضفنا git لأننا نستخدم git clone

# المصدر: نستخدم git clone لسحب المستودع بالكامل
# ملاحظة: سنقوم بسحب المستودع ثم الانتقال إلى المجلد الفرعي 'helwan-terminal'
source=("git+${url}.git")
sha256sums=('SKIP') # استخدم 'SKIP' لأول مرة، ثم استبدلها بمجموع التحقق الفعلي بعد البناء الناجح

build() {
  # الانتقال إلى المجلد الذي تم استنساخه (عادةً يكون اسم المستودع)
  # ثم الانتقال إلى المجلد الفرعي الذي يحتوي على ملفات المشروع
  cd "${pkgname}" # أو يمكنك استخدام cd "helwan-terminal" إذا كنت تفضل الاسم الثابت للمجلد المستنسخ
  cd helwan-terminal # هذا هو المجلد الفرعي الذي ذكرته
  
  # تكوين Meson
  meson setup build --prefix=/usr
  
  # بناء المشروع باستخدام Ninja
  ninja -C build
}

package() {
  # الانتقال إلى المجلد الذي تم استنساخه والمجلد الفرعي
  cd "${pkgname}"
  cd helwan-terminal

  # تثبيت المشروع داخل pkgdir (وهو المجلد المؤقت الذي يتم فيه تجميع الحزمة)
  ninja -C build install DESTDIR="${pkgdir}"

  # بعد التثبيت، يجب التأكد من أن الـ gschema يتم تجميعه وتثبيته.
  # عادةً ما يقوم 'ninja install' بتشغيل 'glib-compile-schemas' بعد تثبيت الـ schema XML.
  # ولكن لضمان ذلك، يمكنك إضافة هذا السطر إذا واجهت مشاكل لاحقًا (لكنه ليس ضروريًا عادةً لـ PKGBUILD):
  # glib-compile-schemas "${pkgdir}/usr/share/glib-2.0/schemas/"
}

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

# المصدر: نستخدم git clone لسحب المستودع بالكامل.
# makepkg يقوم باستنساخ المستودع إلى مجلد باسم 'helwan-terminal' (اسم الريبو).
source=("git+${url}.git")
sha256sums=('SKIP') # استخدم 'SKIP' لأول مرة. بعد أول بناء ناجح، سيُعطيك makepkg مجموع التحقق الصحيح. قم بتحديث هذا السطر به لزيادة الأمان.

build() {
  # الدخول إلى المجلد الرئيسي للمستودع الذي تم استنساخه.
  cd "helwan-terminal"
  
  # ثم الدخول إلى المجلد الفرعي "helwan-terminal" حيث يوجد ملف 'meson.build' فعلياً.
  cd "helwan-terminal" 
  
  # تكوين Meson.
  meson setup build . --prefix=/usr --reconfigure
  
  # بناء المشروع باستخدام Ninja.
  ninja -C build
}

package() {
  # الدخول إلى المجلد الرئيسي للمستودع الذي تم استنساخه.
  cd "helwan-terminal"
  
  # ثم الدخول إلى المجلد الفرعي "helwan-terminal".
  cd "helwan-terminal"
  
  # تثبيت المشروع داخل pkgdir (وهو مجلد مؤقت يتم فيه تجميع ملفات الحزمة).
  # ****** التعديل هنا: DESTDIR قبل الأمر مباشرة ******
  DESTDIR="${pkgdir}" ninja -C build install
}

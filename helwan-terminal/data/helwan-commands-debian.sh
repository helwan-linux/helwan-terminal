```bash
#!/usr/bin/env bash

# ==========================================
# تنظيف المحارف الخفية تلقائياً
# ==========================================
sed -i 's/\xC2\xA0/ /g' "$0" 2>/dev/null
sed -i 's/\xE2\x80\x8B//g' "$0" 2>/dev/null
sed -i 's/\xE2\x80\x8C//g' "$0" 2>/dev/null
sed -i 's/\xE2\x80\x8D//g' "$0" 2>/dev/null
sed -i '1s/^\xEF\xBB\xBF//' "$0" 2>/dev/null

# ==========================================
# تحميل bashrc
# ==========================================
if [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi


# ==========================================
# 1. إصدار التوزيعة
# ==========================================
debian_version_print() {
    cat /etc/os-release
}

version() { debian_version_print; }
الإصدار() { debian_version_print; }
versión() { debian_version_print; }
版本() { debian_version_print; }


# ==========================================
# 2. مزامنة المستودعات
# ==========================================
debian_sync() {
    sudo apt update
}

sync() { debian_sync; }
مزامنة() { debian_sync; }
sincronizar() { debian_sync; }
同步() { debian_sync; }


# ==========================================
# 3. تثبيت الحزم
# ==========================================
debian_install() {
    sudo apt install "$@"
}

install() { debian_install "$@"; }
تثبيت() { debian_install "$@"; }
instalar() { debian_install "$@"; }
安装() { debian_install "$@"; }


# ==========================================
# 4. تحديث النظام
# ==========================================
debian_update() {
    sudo apt update && sudo apt upgrade -y
}

update() { debian_update; }
تحديث() { debian_update; }
actualizar() { debian_update; }
更新() { debian_update; }


# ==========================================
# 5. البحث عن الحزم
# ==========================================
debian_search() {
    apt search "$@"
}

search() { debian_search "$@"; }
بحث() { debian_search "$@"; }
buscar() { debian_search "$@"; }
搜索() { debian_search "$@"; }


# ==========================================
# 6. البحث عن الحزم المثبتة
# ==========================================
debian_search_local() {
    dpkg -l | grep -- "$@"
}

search_local() { debian_search_local "$@"; }
بحث_محلي() { debian_search_local "$@"; }
buscar_local() { debian_search_local "$@"; }
本地搜索() { debian_search_local "$@"; }


# ==========================================
# 7. معلومات الحزمة
# ==========================================
debian_pkg_info() {
    apt show "$@"
}

pkg_info() { debian_pkg_info "$@"; }
معلومات_الحزمة() { debian_pkg_info "$@"; }
info_paquete() { debian_pkg_info "$@"; }
软件包信息() { debian_pkg_info "$@"; }


# ==========================================
# 8. تثبيت حزمة محلية
# ==========================================
debian_local_install() {
    sudo apt install "$@"
}

local_install() { debian_local_install "$@"; }
تثبيت_محلي() { debian_local_install "$@"; }
instalar_local() { debian_local_install "$@"; }
本地安装() { debian_local_install "$@"; }


# ==========================================
# 9. تنظيف الكاش
# ==========================================
debian_clr_cache() {
    sudo apt clean
}

clr_cache() { debian_clr_cache; }
تفريغ_التخزين_المؤقت() { debian_clr_cache; }
limpiar_caché() { debian_clr_cache; }
清理缓存() { debian_clr_cache; }


# ==========================================
# 10. إزالة حزمة
# ==========================================
debian_remove() {
    sudo apt remove "$@"
}

remove() { debian_remove "$@"; }
حذف() { debian_remove "$@"; }
eliminar() { debian_remove "$@"; }
删除() { debian_remove "$@"; }


# ==========================================
# 11. إزالة المخلفات
# ==========================================
debian_autoremove() {
    sudo apt autoremove --purge -y
}

autoremove() { debian_autoremove; }
تنظيف_المخلفات() { debian_autoremove; }
limpiar_automatico() { debian_autoremove; }
自动清理() { debian_autoremove; }


# ==========================================
# 12. معلومات النظام
# ==========================================
debian_system() {
    echo "=== Debian / Ubuntu System ==="
    echo "User: $(builtin command whoami)"
    echo "Hostname: $(hostname)"
    echo "Kernel: $(uname -r)"
    echo "Architecture: $(uname -m)"
    echo "Uptime: $(uptime -p 2>/dev/null || uptime)"
}

system() { debian_system; }
نظام() { debian_system; }
sistema() { debian_system; }
系统() { debian_system; }


# ==========================================
# 13. المستخدم الحالي
# ==========================================
debian_whoami() {
    builtin command whoami
}

user() { debian_whoami; }
المستخدم() { debian_whoami; }
usuario() { debian_whoami; }
用户() { debian_whoami; }


# ==========================================
# 14. سجل الأوامر
# ==========================================
debian_history() {
    builtin history
}

history_list() { debian_history; }
سجل() { debian_history; }
historial() { debian_history; }
历史() { debian_history; }


# ==========================================
# 15. تنظيف الشاشة
# ==========================================
debian_clear() {
    builtin command clear
}

clear_screen() { debian_clear; }
مسح() { debian_clear; }
limpiar() { debian_clear; }
清屏() { debian_clear; }


# ==========================================
# 16. المساعدة
# ==========================================
debian_help() {
    cat << 'EOF'
==========================================
       Debian / Ubuntu Terminal Help
==========================================

VERSION / الإصدار / VERSIÓN / 版本
  version
  الإصدار
  versión
  版本

SYNC / مزامنة / SINCRONIZAR / 同步
  sync
  مزامنة
  sincronizar
  同步

UPDATE / تحديث / ACTUALIZAR / 更新
  update
  تحديث
  actualizar
  更新

INSTALL / تثبيت / INSTALAR / 安装
  install PACKAGE
  تثبيت PACKAGE
  instalar PACKAGE
  安装 PACKAGE

REMOVE / حذف / ELIMINAR / 删除
  remove PACKAGE
  حذف PACKAGE
  eliminar PACKAGE
  删除 PACKAGE

SEARCH / بحث / BUSCAR / 搜索
  search PACKAGE
  بحث PACKAGE
  buscar PACKAGE
  搜索 PACKAGE

SEARCH LOCAL / بحث محلي / BUSCAR LOCAL / 本地搜索
  search_local PACKAGE
  بحث_محلي PACKAGE
  buscar_local PACKAGE
  本地搜索 PACKAGE

PACKAGE INFO / معلومات الحزمة / INFO PAQUETE / 软件包信息
  pkg_info PACKAGE
  معلومات_الحزمة PACKAGE
  info_paquete PACKAGE
  软件包信息 PACKAGE

LOCAL INSTALL / تثبيت محلي / INSTALAR LOCAL / 本地安装
  local_install FILE
  تثبيت_محلي FILE
  instalar_local FILE
  本地安装 FILE

CLEAR CACHE / تنظيف الكاش / LIMPIAR CACHÉ / 清理缓存
  clr_cache
  تفريغ_التخزين_المؤقت
  limpiar_caché
  清理缓存

AUTOREMOVE / تنظيف المخلفات / LIMPIAR AUTOMÁTICO / 自动清理
  autoremove
  تنظيف_المخلفات
  limpiar_automatico
  自动清理

SYSTEM / نظام / SISTEMA / 系统
  system
  نظام
  sistema
  系统

USER / المستخدم / USUARIO / 用户
  user
  المستخدم
  usuario
  用户

HISTORY / السجل / HISTORIAL / 历史
  history_list
  سجل
  historial
  历史

CLEAR / مسح الشاشة / LIMPIAR / 清屏
  clear_screen
  مسح
  limpiar
  清屏

HELP / مساعدة / AYUDA / 帮助
  help
  مساعدة
  ayuda
  帮助
==========================================
EOF
}

help() { debian_help; }
مساعدة() { debian_help; }
ayuda() { debian_help; }
帮助() { debian_help; }
```

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
# تحميل bashrc الأصلي للمستخدم
# ==========================================
if [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi


# ==========================================
# 1. إصدار التوزيعة
# ==========================================
helwan_version_print() {
    cat /etc/os-release
}

version() { helwan_version_print; }
الإصدار() { helwan_version_print; }
versión() { helwan_version_print; }
版本() { helwan_version_print; }


# ==========================================
# 2. مزامنة المستودعات
# ==========================================
helwan_sync() {
    sudo pacman -Sy
}

sync() { helwan_sync; }
مزامنة() { helwan_sync; }
sincronizar() { helwan_sync; }
同步() { helwan_sync; }


# ==========================================
# 3. تثبيت الحزم
# ==========================================
helwan_install() {
    sudo pacman -S "$@"
}

install() { helwan_install "$@"; }
تثبيت() { helwan_install "$@"; }
instalar() { helwan_install "$@"; }
安装() { helwan_install "$@"; }


# ==========================================
# 4. تحديث النظام
# ==========================================
helwan_update() {
    sudo pacman -Syu
}

update() { helwan_update; }
تحديث() { helwan_update; }
actualizar() { helwan_update; }
更新() { helwan_update; }


# ==========================================
# 5. البحث عن الحزم
# ==========================================
helwan_search() {
    pacman -Ss "$@"
}

search() { helwan_search "$@"; }
بحث() { helwan_search "$@"; }
buscar() { helwan_search "$@"; }
搜索() { helwan_search "$@"; }


# ==========================================
# 6. البحث عن الحزم المثبتة محلياً
# ==========================================
helwan_search_local() {
    pacman -Qs "$@"
}

search_local() { helwan_search_local "$@"; }
بحث_محلي() { helwan_search_local "$@"; }
buscar_local() { helwan_search_local "$@"; }
本地搜索() { helwan_search_local "$@"; }


# ==========================================
# 7. معلومات الحزمة
# ==========================================
helwan_pkg_info() {
    pacman -Qi "$@"
}

pkg_info() { helwan_pkg_info "$@"; }
معلومات_الحزمة() { helwan_pkg_info "$@"; }
info_paquete() { helwan_pkg_info "$@"; }
软件包信息() { helwan_pkg_info "$@"; }


# ==========================================
# 8. تثبيت حزمة محلية
# ==========================================
helwan_local_install() {
    sudo pacman -U "$@"
}

local_install() { helwan_local_install "$@"; }
تثبيت_محلي() { helwan_local_install "$@"; }
instalar_local() { helwan_local_install "$@"; }
本地安装() { helwan_local_install "$@"; }


# ==========================================
# 9. تنظيف الكاش
# ==========================================
helwan_clr_cache() {
    sudo pacman -Scc
}

clr_cache() { helwan_clr_cache; }
تفريغ_التخزين_المؤقت() { helwan_clr_cache; }
limpiar_caché() { helwan_clr_cache; }
清理缓存() { helwan_clr_cache; }


# ==========================================
# 10. إلغاء قفل قاعدة بيانات Pacman
# ==========================================
helwan_unlock() {
    if [ -f /var/lib/pacman/db.lck ]; then
        sudo rm -f /var/lib/pacman/db.lck
        echo "Pacman database lock removed."
    else
        echo "No Pacman database lock found."
    fi
}

unlock() { helwan_unlock; }
فك_القفل() { helwan_unlock; }
desbloquear() { helwan_unlock; }
解锁() { helwan_unlock; }


# ==========================================
# 11. إزالة حزمة
# ==========================================
helwan_remove() {
    sudo pacman -R "$@"
}

remove() { helwan_remove "$@"; }
حذف() { helwan_remove "$@"; }
eliminar() { helwan_remove "$@"; }
删除() { helwan_remove "$@"; }


# ==========================================
# 12. إزالة الحزم والمخلفات
# ==========================================
helwan_autoremove() {
    sudo pacman -Rns "$@"
}

autoremove() { helwan_autoremove "$@"; }
تنظيف_المخلفات() { helwan_autoremove "$@"; }
limpiar_automatico() { helwan_autoremove "$@"; }
自动清理() { helwan_autoremove "$@"; }


# ==========================================
# 13. معلومات Helwan Linux
# ==========================================
helwan_info() {
    uname -a
}

helwan() { helwan_info; }
حلوان() { helwan_info; }
helwán() { helwan_info; }
赫尔万() { helwan_info; }


# ==========================================
# 14. معلومات النظام
# ==========================================
helwan_system() {
    echo "=== Helwan Linux System ==="
    echo "User: $(builtin command whoami)"
    echo "Hostname: $(hostname)"
    echo "Kernel: $(uname -r)"
    echo "Architecture: $(uname -m)"
    echo "Uptime: $(uptime -p 2>/dev/null || uptime)"
}

system() { helwan_system; }
نظام() { helwan_system; }
sistema() { helwan_system; }
系统() { helwan_system; }


# ==========================================
# 15. المستخدم الحالي
# ==========================================
helwan_whoami() {
    builtin command whoami
}

user() { helwan_whoami; }
المستخدم() { helwan_whoami; }
usuario() { helwan_whoami; }
用户() { helwan_whoami; }


# ==========================================
# 16. سجل الأوامر
# ==========================================
helwan_history() {
    builtin history
}

history_list() { helwan_history; }
سجل() { helwan_history; }
historial() { helwan_history; }
历史() { helwan_history; }


# ==========================================
# 17. تنظيف الشاشة
# ==========================================
helwan_clear() {
    builtin command clear
}

clear_screen() { helwan_clear; }
مسح() { helwan_clear; }
limpiar() { helwan_clear; }
清屏() { helwan_clear; }


# ==========================================
# 18. المساعدة
# ==========================================
helwan_help() {
    cat << 'EOF'
==========================================
        Helwan Linux Terminal Help
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

UNLOCK / فك القفل / DESBLOQUEAR / 解锁
  unlock
  فك_القفل
  desbloquear
  解锁

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

help() { helwan_help; }
مساعدة() { helwan_help; }
ayuda() { helwan_help; }
帮助() { helwan_help; }
```

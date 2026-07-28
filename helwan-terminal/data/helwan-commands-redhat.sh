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
fedora_version_print() {
    cat /etc/os-release
}

version() { fedora_version_print; }
الإصدار() { fedora_version_print; }
versión() { fedora_version_print; }
版本() { fedora_version_print; }


# ==========================================
# 2. مزامنة المستودعات
# ==========================================
fedora_sync() {
    sudo dnf check-update
    local status=$?

    if [ "$status" -eq 100 ]; then
        return 0
    fi

    return "$status"
}

sync() { fedora_sync; }
مزامنة() { fedora_sync; }
sincronizar() { fedora_sync; }
同步() { fedora_sync; }


# ==========================================
# 3. تثبيت الحزم
# ==========================================
fedora_install() {
    sudo dnf install "$@"
}

install() { fedora_install "$@"; }
تثبيت() { fedora_install "$@"; }
instalar() { fedora_install "$@"; }
安装() { fedora_install "$@"; }


# ==========================================
# 4. تحديث النظام
# ==========================================
fedora_update() {
    sudo dnf upgrade -y
}

update() { fedora_update; }
تحديث() { fedora_update; }
actualizar() { fedora_update; }
更新() { fedora_update; }


# ==========================================
# 5. البحث عن الحزم
# ==========================================
fedora_search() {
    dnf search "$@"
}

search() { fedora_search "$@"; }
بحث() { fedora_search "$@"; }
buscar() { fedora_search "$@"; }
搜索() { fedora_search "$@"; }


# ==========================================
# 6. البحث عن الحزم المثبتة
# ==========================================
fedora_search_local() {
    dnf list installed | grep -- "$@"
}

search_local() { fedora_search_local "$@"; }
بحث_محلي() { fedora_search_local "$@"; }
buscar_local() { fedora_search_local "$@"; }
本地搜索() { fedora_search_local "$@"; }


# ==========================================
# 7. معلومات الحزمة
# ==========================================
fedora_pkg_info() {
    dnf info "$@"
}

pkg_info() { fedora_pkg_info "$@"; }
معلومات_الحزمة() { fedora_pkg_info "$@"; }
info_paquete() { fedora_pkg_info "$@"; }
软件包信息() { fedora_pkg_info "$@"; }


# ==========================================
# 8. تثبيت حزمة محلية
# ==========================================
fedora_local_install() {
    sudo dnf install "$@"
}

local_install() { fedora_local_install "$@"; }
تثبيت_محلي() { fedora_local_install "$@"; }
instalar_local() { fedora_local_install "$@"; }
本地安装() { fedora_local_install "$@"; }


# ==========================================
# 9. تنظيف الكاش
# ==========================================
fedora_clr_cache() {
    sudo dnf clean all
}

clr_cache() { fedora_clr_cache; }
تفريغ_التخزين_المؤقت() { fedora_clr_cache; }
limpiar_caché() { fedora_clr_cache; }
清理缓存() { fedora_clr_cache; }


# ==========================================
# 10. إزالة حزمة
# ==========================================
fedora_remove() {
    sudo dnf remove "$@"
}

remove() { fedora_remove "$@"; }
حذف() { fedora_remove "$@"; }
eliminar() { fedora_remove "$@"; }
删除() { fedora_remove "$@"; }


# ==========================================
# 11. إزالة المخلفات
# ==========================================
fedora_autoremove() {
    sudo dnf autoremove -y
}

autoremove() { fedora_autoremove; }
تنظيف_المخلفات() { fedora_autoremove; }
limpiar_automatico() { fedora_autoremove; }
自动清理() { fedora_autoremove; }


# ==========================================
# 12. معلومات النظام
# ==========================================
fedora_system() {
    echo "=== Fedora System ==="
    echo "User: $(builtin command whoami)"
    echo "Hostname: $(hostname)"
    echo "Kernel: $(uname -r)"
    echo "Architecture: $(uname -m)"
    echo "Uptime: $(uptime -p 2>/dev/null || uptime)"
}

system() { fedora_system; }
نظام() { fedora_system; }
sistema() { fedora_system; }
系统() { fedora_system; }


# ==========================================
# 13. المستخدم الحالي
# ==========================================
fedora_whoami() {
    builtin command whoami
}

user() { fedora_whoami; }
المستخدم() { fedora_whoami; }
usuario() { fedora_whoami; }
用户() { fedora_whoami; }


# ==========================================
# 14. سجل الأوامر
# ==========================================
fedora_history() {
    builtin history
}

history_list() { fedora_history; }
سجل() { fedora_history; }
historial() { fedora_history; }
历史() { fedora_history; }


# ==========================================
# 15. تنظيف الشاشة
# ==========================================
fedora_clear() {
    builtin command clear
}

clear_screen() { fedora_clear; }
مسح() { fedora_clear; }
limpiar() { fedora_clear; }
清屏() { fedora_clear; }


# ==========================================
# 16. المساعدة
# ==========================================
fedora_help() {
    cat << 'EOF'
==========================================
          Fedora Terminal Help
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

help() { fedora_help; }
مساعدة() { fedora_help; }
ayuda() { fedora_help; }
帮助() { fedora_help; }
```

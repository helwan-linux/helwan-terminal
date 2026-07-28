```bash
#!/usr/bin/env bash

# ==========================================
# تنظيف المحارف الخفية تلقائياً عند التشغيل
# ==========================================

# إزالة Non-Breaking Space (C2 A0)
sed -i 's/\xC2\xA0/ /g' "$0" 2>/dev/null

# إزالة Zero Width Space (E2 80 8B)
sed -i 's/\xE2\x80\x8B//g' "$0" 2>/dev/null

# إزالة Zero Width Non-Joiner (E2 80 8C)
sed -i 's/\xE2\x80\x8C//g' "$0" 2>/dev/null

# إزالة Zero Width Joiner (E2 80 8D)
sed -i 's/\xE2\x80\x8D//g' "$0" 2>/dev/null

# إزالة BOM في بداية الملف إن وجد
sed -i '1s/^\xEF\xBB\xBF//' "$0" 2>/dev/null


# ==========================================
# تحميل ملف bashrc الأصلي للمستخدم أولاً
# ==========================================
# للحفاظ على إعداداته و aliases الأساسية
if [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi


# ==========================================
# 1. عرض إصدار التوزيعة (version)
# ==========================================
debian_version_print() {
    cat /etc/os-release
}

الإصدار() { debian_version_print; }
version() { debian_version_print; }
versión() { debian_version_print; }
版本() { debian_version_print; }


# ==========================================
# 2. مزامنة المستودعات وتحديثها (sync)
# ==========================================
debian_sync() {
    sudo apt update
}

مزامنة() { debian_sync; }
sync() { debian_sync; }
sincronizar() { debian_sync; }
同步() { debian_sync; }


# ==========================================
# 3. تثبيت الحزم (install)
# ==========================================
debian_install() {
    sudo apt install "$@"
}

تثبيت() { debian_install "$@"; }
install() { debian_install "$@"; }
instalar() { debian_install "$@"; }
安装() { debian_install "$@"; }


# ==========================================
# 4. تحديث النظام والحزم (update)
# ==========================================
debian_update() {
    sudo apt update && sudo apt upgrade -y
}

تحديث() { debian_update; }
update() { debian_update; }
actualizar() { debian_update; }
更新() { debian_update; }


# ==========================================
# 5. البحث عن الحزم (search)
# ==========================================
debian_search() {
    apt search "$@"
}

بحث() { debian_search "$@"; }
search() { debian_search "$@"; }
buscar() { debian_search "$@"; }
搜索() { debian_search "$@"; }


# ==========================================
# 6. البحث عن الحزم المثبتة محلياً (search-local)
# ==========================================
debian_search_local() {
    dpkg -l | grep -- "$@"
}

بحث_محلي() { debian_search_local "$@"; }
search_local() { debian_search_local "$@"; }
buscar_local() { debian_search_local "$@"; }
本地搜索() { debian_search_local "$@"; }


# ==========================================
# 7. معلومات حزمة معينة (pkg-info)
# ==========================================
debian_pkg_info() {
    apt show "$@"
}

معلومات_الحزمة() { debian_pkg_info "$@"; }
pkg_info() { debian_pkg_info "$@"; }
info_paquete() { debian_pkg_info "$@"; }
软件包信息() { debian_pkg_info "$@"; }


# ==========================================
# 8. تثبيت حزمة محلية بصيغة deb (local-install)
# ==========================================
debian_local_install() {
    sudo dpkg -i "$@" &&
    sudo apt install -f -y
}

تثبيت_محلي() { debian_local_install "$@"; }
local_install() { debian_local_install "$@"; }
instalar_local() { debian_local_install "$@"; }
本地安装() { debian_local_install "$@"; }


# ==========================================
# 9. تفريغ التخزين المؤقت للكاش (clr-cache)
# ==========================================
debian_clr_cache() {
    sudo apt clean
}

تفريغ_التخزين_المؤقت() { debian_clr_cache; }
clr_cache() { debian_clr_cache; }
limpiar_caché() { debian_clr_cache; }
清理缓存() { debian_clr_cache; }


# ==========================================
# 10. إزالة حزمة (remove)
# ==========================================
debian_remove() {
    sudo apt remove "$@"
}

حذف() { debian_remove "$@"; }
remove() { debian_remove "$@"; }
eliminar() { debian_remove "$@"; }
删除() { debian_remove "$@"; }


# ==========================================
# 11. إزالة الحزم اليتيمة والمخلفات (autoremove)
# ==========================================
debian_autoremove() {
    sudo apt autoremove --purge -y
}

تنظيف_المخلفات() { debian_autoremove; }
autoremove() { debian_autoremove; }
limpiar_automatico() { debian_autoremove; }
自动清理() { debian_autoremove; }
```

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
# تحميل ملف bashrc الأصلي للمستخدم
# ==========================================
# للحفاظ على إعداداته و aliases الأساسية
if [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi


# ==========================================
# 1. عرض إصدار التوزيعة (version)
# ==========================================
helwan_version_print() {
    sed -n '1p' /etc/os-release
    sed -n '12p' /etc/os-release
    sed -n '13p' /etc/os-release
}

الإصدار() { helwan_version_print; }
version() { helwan_version_print; }
versión() { helwan_version_print; }
版本() { helwan_version_print; }


# ==========================================
# 2. مزامنة المستودعات وتحديثها الكامل (sync)
# ==========================================
helwan_sync() {
    sudo pacman -Syyy
}

مزامنة() { helwan_sync; }
sync() { helwan_sync; }
sincronizar() { helwan_sync; }
同步() { helwan_sync; }


# ==========================================
# 3. تثبيت الحزم (install)
# ==========================================
helwan_install() {
    sudo pacman -S "$@"
}

تثبيت() { helwan_install "$@"; }
install() { helwan_install "$@"; }
instalar() { helwan_install "$@"; }
安装() { helwan_install "$@"; }


# ==========================================
# 4. تحديث النظام والحزم (update)
# ==========================================
helwan_update() {
    sudo pacman -Syu
}

تحديث() { helwan_update; }
update() { helwan_update; }
actualizar() { helwan_update; }
更新() { helwan_update; }


# ==========================================
# 5. البحث عن الحزم عبر الإنترنت (search)
# ==========================================
helwan_search() {
    sudo pacman -Ss "$@"
}

بحث() { helwan_search "$@"; }
search() { helwan_search "$@"; }
buscar() { helwan_search "$@"; }
搜索() { helwan_search "$@"; }


# ==========================================
# 6. البحث عن الحزم المثبتة محلياً (search-local)
# ==========================================
helwan_search_local() {
    sudo pacman -Qs "$@"
}

بحث_محلي() { helwan_search_local "$@"; }
search_local() { helwan_search_local "$@"; }
buscar_local() { helwan_search_local "$@"; }
本地搜索() { helwan_search_local "$@"; }


# ==========================================
# 7. معلومات حزمة معينة (pkg-info)
# ==========================================
helwan_pkg_info() {
    sudo pacman -Qi "$@"
}

معلومات_الحزمة() { helwan_pkg_info "$@"; }
pkg_info() { helwan_pkg_info "$@"; }
info_paquete() { helwan_pkg_info "$@"; }
软件包信息() { helwan_pkg_info "$@"; }


# ==========================================
# 8. تثبيت حزمة محلية بصيغة pkg.tar (local-install)
# ==========================================
helwan_local_install() {
    sudo pacman -U "$@"
}

تثبيت_محلي() { helwan_local_install "$@"; }
local_install() { helwan_local_install "$@"; }
instalar_local() { helwan_local_install "$@"; }
本地安装() { helwan_local_install "$@"; }


# ==========================================
# 9. تفريغ التخزين المؤقت للكاش (clr-cache)
# ==========================================
helwan_clr_cache() {
    sudo pacman -Scc
}

تفريغ_التخزين_المؤقت() { helwan_clr_cache; }
clr_cache() { helwan_clr_cache; }
limpiar_caché() { helwan_clr_cache; }
清理缓存() { helwan_clr_cache; }


# ==========================================
# 10. إلغاء قفل قاعدة بيانات باكمان (unlock)
# ==========================================
helwan_unlock() {
    if [ -f /var/lib/pacman/db.lck ]; then
        sudo rm -f /var/lib/pacman/db.lck
    else
        echo "لا يوجد قفل لقاعدة بيانات pacman."
    fi
}

فك_القفل() { helwan_unlock; }
unlock() { helwan_unlock; }
desbloquear() { helwan_unlock; }
解锁() { helwan_unlock; }


# ==========================================
# 11. إزالة حزمة (remove)
# ==========================================
helwan_remove() {
    sudo pacman -R "$@"
}

حذف() { helwan_remove "$@"; }
remove() { helwan_remove "$@"; }
eliminar() { helwan_remove "$@"; }
删除() { helwan_remove "$@"; }


# ==========================================
# 12. إزالة الحزم والمخلفات (autoremove)
# ==========================================
helwan_autoremove() {
    sudo pacman -Rns "$@"
}

تنظيف_المخلفات() { helwan_autoremove "$@"; }
autoremove() { helwan_autoremove "$@"; }
limpiar_automatico() { helwan_autoremove "$@"; }
自动清理() { helwan_autoremove "$@"; }


# ==========================================
# 13. معلومات النظام والتوزيعة العامة (helwan)
# ==========================================
helwan_info() {
    uname -a
}

حلوان() { helwan_info; }
helwan() { helwan_info; }
helwán() { helwan_info; }
赫尔万() { helwan_info; }
```

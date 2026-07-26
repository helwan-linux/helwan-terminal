# تحميل ملف الـ bashrc الأصلي للمستخدم أولاً لو موجود
if [ -f ~/.bashrc ]; then
    source ~/.bashrc
fi

# ==========================================
# 1. عرض إصدار التوزيعة (version)
# ==========================================
fedora_version_print() {
    cat /etc/os-release
}

الإصدار() { fedora_version_print; }
version() { fedora_version_print; }
versión() { fedora_version_print; }
版本() { fedora_version_print; }

# ==========================================
# 2. مزامنة المستودعات وتحديثها (sync)
# ==========================================
fedora_sync() {
    sudo dnf check-update
}

مزامنة() { fedora_sync; }
sync() { fedora_sync; }
sincronizar() { fedora_sync; }
同步() { fedora_sync; }

# ==========================================
# 3. تثبيت الحزم (install)
# ==========================================
fedora_install() {
    sudo dnf install "$@"
}

تثبيت() { fedora_install "$@"; }
install() { fedora_install "$@"; }
instalar() { fedora_install "$@"; }
安装() { fedora_install "$@"; }

# ==========================================
# 4. تحديث النظام والحزم (update)
# ==========================================
fedora_update() {
    sudo dnf upgrade -y
}

تحديث() { fedora_update; }
update() { fedora_update; }
actualizar() { fedora_update; }
更新() { fedora_update; }

# ==========================================
# 5. البحث عن الحزم (search)
# ==========================================
fedora_search() {
    dnf search "$@"
}

بحث() { fedora_search "$@"; }
search() { fedora_search "$@"; }
buscar() { fedora_search "$@"; }
搜索() { fedora_search "$@"; }

# ==========================================
# 6. البحث عن الحزم المثبتة محلياً (search-local)
# ==========================================
fedora_search_local() {
    dnf list installed | grep "$@"
}

بحث_محلي() { fedora_search_local "$@"; }
search-local() { fedora_search_local "$@"; }
buscar-local() { fedora_search_local "$@"; }
本地搜索() { fedora_search_local "$@"; }

# ==========================================
# 7. معلومات حزمة معينة (pkg-info)
# ==========================================
fedora_pkg_info() {
    dnf info "$@"
}

معلومات_الحزمة() { fedora_pkg_info "$@"; }
pkg-info() { fedora_pkg_info "$@"; }
info-paquete() { fedora_pkg_info "$@"; }
软件包信息() { fedora_pkg_info "$@"; }

# ==========================================
# 8. تثبيت حزمة محلية بصيغة rpm (local-install)
# ==========================================
fedora_local_install() {
    sudo dnf localinstall "$@"
}

تثبيت_محلي() { fedora_local_install "$@"; }
local-install() { fedora_local_install "$@"; }
instalar-local() { fedora_local_install "$@"; }
本地安装() { fedora_local_install "$@"; }

# ==========================================
# 9. تفريغ التخزين المؤقت للكاش (clr-cache)
# ==========================================
fedora_clr_cache() {
    sudo dnf clean all
}

تفريغ_التخزين_المؤقت() { fedora_clr_cache; }
clr-cache() { fedora_clr_cache; }
limpiar-caché() { fedora_clr_cache; }
清理缓存() { fedora_clr_cache; }

# ==========================================
# 10. إزالة حزمة (remove)
# ==========================================
fedora_remove() {
    sudo dnf remove "$@"
}

حذف() { fedora_remove "$@"; }
remove() { fedora_remove "$@"; }
eliminar() { fedora_remove "$@"; }
删除() { fedora_remove "$@"; }

# ==========================================
# 11. إزالة الحزم اليتيمة والمخلفات (autoremove)
# ==========================================
fedora_autoremove() {
    sudo dnf autoremove -y
}

تنظيف_المخلفات() { fedora_autoremove; }
autoremove() { fedora_autoremove; }
limpiar-automatico() { fedora_autoremove; }
自动清理() { fedora_autoremove; }
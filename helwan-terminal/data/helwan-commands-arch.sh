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

# ==========================================
# 2. مزامنة المستودعات
# ==========================================
helwan_sync() {
    sudo pacman -Sy
}

# ==========================================
# 3. تثبيت الحزم
# ==========================================
helwan_install() {
    sudo pacman -S "$@"
}

# ==========================================
# 4. تحديث النظام
# ==========================================
helwan_update() {
    sudo pacman -Syu
}

# ==========================================
# 5. البحث عن الحزم
# ==========================================
helwan_search() {
    pacman -Ss "$@"
}

# ==========================================
# 6. البحث عن الحزم المثبتة محلياً
# ==========================================
helwan_search_local() {
    pacman -Qs "$@"
}

# ==========================================
# 7. معلومات الحزمة
# ==========================================
helwan_pkg_info() {
    pacman -Qi "$@"
}

# ==========================================
# 8. تثبيت حزمة محلية
# ==========================================
helwan_local_install() {
    sudo pacman -U "$@"
}

# ==========================================
# 9. تنظيف الكاش
# ==========================================
helwan_clr_cache() {
    sudo pacman -Scc
}

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

# ==========================================
# 11. إزالة حزمة
# ==========================================
helwan_remove() {
    sudo pacman -R "$@"
}

# ==========================================
# 12. إزالة الحزم والمخلفات
# ==========================================
helwan_autoremove() {
    sudo pacman -Rns "$@"
}

# ==========================================
# 13. معلومات Helwan Linux
# ==========================================
helwan_info() {
    uname -a
}

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

# ==========================================
# 15. المستخدم الحالي
# ==========================================
helwan_whoami() {
    builtin command whoami
}

# ==========================================
# 16. سجل الأوامر
# ==========================================
helwan_history() {
    builtin history
}

# ==========================================
# 17. تنظيف الشاشة
# ==========================================
helwan_clear() {
    builtin command clear
}

# ==========================================
# الأوامر والوظائف الإضافية والمصححة
# ==========================================
helwan_upgrade() {
    sudo pacman -Syu
}

# تم إزالة --needed لضمان إعادة التثبيت الفعلي
helwan_reinstall() {
    sudo pacman -S --overwrite '*' --noconfirm "$@"
}

helwan_list_installed() {
    pacman -Qe "$@"
}

helwan_list_files() {
    pacman -Ql "$@"
}

helwan_clean_cache_all() {
    sudo pacman -Scc --noconfirm
}

helwan_processes() {
    ps aux "$@"
}

helwan_ports() {
    ss -tuln "$@"
}

helwan_ip() {
    ip a "$@"
}

helwan_ping_host() {
    ping -c 4 "$@"
}

helwan_shutdown() {
    sudo shutdown now
}

helwan_reboot() {
    sudo reboot
}

# ==========================================
# 18. المساعدة (محدثة لتشمل أوامر AUR)
# ==========================================
helwan_help() {
    cat << 'EOF'
┌────────────────────────────────────────────────────────────────────────┐
│                      HELWAN LINUX TERMINAL HELP                        │
├────────────────────┬────────────────────┬──────────────────┬───────────┤
│ English            │ العربية            │ Español          │ 中文      │
├────────────────────┼────────────────────┼──────────────────┼───────────┤
│ version            │ الإصدار            │ versión          │ 版本      │
│ sync               │ مزامنة             │ sincronizar      │ 同步      │
│ update             │ تحديث              │ actualizar       │ 更新      │
│ upgrade            │ ترقية              │ actualizar       │ 升级      │
│ install <pkg>      │ تثبيت <حزمة>       │ instalar         │ 安装      │
│ reinstall <pkg>    │ إعادة_تثبيت        │ reinstalar       │ 重新安装  │
│ remove <pkg>       │ حذف <حزمة>         │ eliminar         │ 删除      │
│ search <pkg>       │ بحث <حزمة>         │ buscar           │ 搜索      │
│ search_local       │ بحث_محلي           │ buscar_local     │ 本地搜索  │
│ list_installed     │ قائمة_المثبتات     │ lista_instalados │ 已安装    │
│ list_files <pkg>   │ ملفات_الحزمة       │ lista_archivos   │ 软件包文件│
│ pkg_info <pkg>     │ معلومات_الحزمة     │ info_paquete     │ 软件包信息│
│ local_install      │ تثبيت_محلي         │ instalar_local   │ 本地安装  │
│ clr_cache          │ تفريغ_التخزين_المؤقت│ limpiar_cache    │ 清理缓存  │
│ clean_cache_all    │ تفريغ_الكاش_كامل   │ limpiar_completo │ 完全清理缓存│
│ unlock             │ فك_القفل           │ desbloquear      │ 解锁      │
│ autoremove         │ تنظيف_المخلفات     │ limpiar_auto     │ 自动清理  │
│ helwan             │ حلوان              │ helwán           │ 赫尔万    │
│ system             │ نظام               │ sistema          │ 系统      │
│ user               │ المستخدم           │ usuario          │ 用户      │
│ history_list       │ سجل                │ historial        │ 历史      │
│ clear_screen       │ مسح                │ limpiar          │ 清屏      │
│ help               │ مساعدة             │ ayuda            │ 帮助      │
├────────────────────┴────────────────────┴──────────────────┴───────────┤
│                          AUR COMMANDS (YAY)                            │
├────────────────────┬────────────────────┬──────────────────┬───────────┤
│ aur_install <pkg>  │ تثبيت_اور          │ aur_instalar     │ AUR安装   │
│ aur_remove <pkg>   │ حذف_اور            │ aur_eliminar     │ AUR删除   │
│ aur_search <pkg>   │ بحث_اور            │ aur_buscar       │ AUR搜索   │
│ aur_update         │ تحديث_اور          │ aur_actualizar   │ AUR更新   │
│ aur_list           │ قائمة_اور          │ aur_lista        │ AUR列表   │
├────────────────────┴────────────────────┴──────────────────┴───────────┤
│                          ADVANCED COMMANDS                             │
├────────────────────┬────────────────────┬──────────────────┬───────────┤
│ orphans            │ يتيمة              │ huérfanos        │ 孤儿      │
│ remove_orphans     │ حذف_اليتيمة       │ eliminar_huerfano│ 删除孤儿  │
│ service            │ خدمة               │ servicio         │ 服务      │
│ logs               │ سجلات              │ registros        │ 日志      │
│ keys               │ مفاتيح             │ llaves           │ 密钥      │
│ net                │ شبكة               │ red              │ 网络      │
│ disk               │ أقراص              │ disco            │ 磁盘      │
│ disks_list         │ قائمة_الأقراص      │ lista_discos     │ 磁盘列表  │
│ memory             │ ذاكرة              │ memoria          │ 内存      │
│ modules            │ وحدات              │ módulos          │ 内核模块  │
│ processes          │ العمليات           │ procesos         │ 进程      │
│ ports              │ المنافذ            │ puertos          │ 端口      │
│ ip                 │ آي_بي              │ ip               │ IP地址    │
│ ping_host          │ بينج               │ ping_host        │ 持续ping  │
│ shutdown           │ إيقاف_التشغيل      │ apagar           │ 关机      │
│ reboot             │ إعادة_تشغيل        │ reiniciar        │ 重启      │
└────────────────────────────────────────────────────────────────────────┘
EOF
}

# ==========================================
# ربط الأسماء والأوامر الأساسية والإضافية
# ==========================================

# 1. الإصدار
version() { helwan_version_print; }
الاصدار() { helwan_version_print; }
versión() { helwan_version_print; }
版本() { helwan_version_print; }

# 2. مزامنة
sync() { helwan_sync; }
مزامنة() { helwan_sync; }
sincronizar() { helwan_sync; }
同步() { helwan_sync; }

# 3. تثبيت
install() { helwan_install "$@"; }
تثبيت() { helwan_install "$@"; }
instalar() { helwan_install "$@"; }
安装() { helwan_install "$@"; }

# 4. تحديث
update() { helwan_update; }
تحديث() { helwan_update; }
actualizar() { helwan_update; }
更新() { helwan_update; }

# 4.1 ترقية
upgrade() { helwan_upgrade; }
ترقية() { helwan_upgrade; }
actualizar_sistema() { helwan_upgrade; }
升级() { helwan_upgrade; }

# 4.2 إعادة تثبيت
reinstall() { helwan_reinstall "$@"; }
إعادة_تثبيت() { helwan_reinstall "$@"; }
reinstalar() { helwan_reinstall "$@"; }
重新安装() { helwan_reinstall "$@"; }

# 5. بحث
search() { helwan_search "$@"; }
بحث() { helwan_search "$@"; }
buscar() { helwan_search "$@"; }
搜索() { helwan_search "$@"; }

# 6. بحث محلي
search_local() { helwan_search_local "$@"; }
بحث_محلي() { helwan_search_local "$@"; }
buscar_local() { helwan_search_local "$@"; }
本地搜索() { helwan_search_local "$@"; }

# 6.1 قائمة المثبتات
list_installed() { helwan_list_installed "$@"; }
قائمة_المثبتات() { helwan_list_installed "$@"; }
lista_instalados() { helwan_list_installed "$@"; }
已安装() { helwan_list_installed "$@"; }

# 6.2 ملفات الحزمة
list_files() { helwan_list_files "$@"; }
ملفات_الحزمة() { helwan_list_files "$@"; }
lista_archivos() { helwan_list_files "$@"; }
软件包文件() { helwan_list_files "$@"; }

# 7. معلومات الحزمة
pkg_info() { helwan_pkg_info "$@"; }
معلومات_الحزمة() { helwan_pkg_info "$@"; }
info_paquete() { helwan_pkg_info "$@"; }
软件包信息() { helwan_pkg_info "$@"; }

# 8. تثبيت محلي
local_install() { helwan_local_install "$@"; }
تثبيت_محلي() { helwan_local_install "$@"; }
instalar_local() { helwan_local_install "$@"; }
本地安装() { helwan_local_install "$@"; }

# 9. تنظيف الكاش
clr_cache() { helwan_clr_cache; }
تفريغ_التخزين_المؤقت() { helwan_clr_cache; }
limpiar_cache() { helwan_clr_cache; }
清理缓存() { helwan_clr_cache; }

# 9.1 تنظيف الكاش بالكامل
clean_cache_all() { helwan_clean_cache_all; }
تفريغ_الكاش_كامل() { helwan_clean_cache_all; }
limpiar_completo() { helwan_clean_cache_all; }
完全清理缓存() { helwan_clean_cache_all; }

# 10. فك القفل
unlock() { helwan_unlock; }
فك_القفل() { helwan_unlock; }
desbloquear() { helwan_unlock; }
解锁() { helwan_unlock; }

# 11. حذف
remove() { helwan_remove "$@"; }
حذف() { helwan_remove "$@"; }
eliminar() { helwan_remove "$@"; }
删除() { helwan_remove "$@"; }

# 12. تنظيف المخلفات
autoremove() { helwan_autoremove "$@"; }
تنظيف_المخلفات() { helwan_autoremove "$@"; }
limpiar_auto() { helwan_autoremove "$@"; }
自动清理() { helwan_autoremove "$@"; }

# 13. helwan info
helwan() { helwan_info; }
حلوان() { helwan_info; }
helwán() { helwan_info; }
赫尔万() { helwan_info; }

# 14. نظام
system() { helwan_system; }
نظام() { helwan_system; }
sistema() { helwan_system; }
系统() { helwan_system; }

# 15. المستخدم
user() { helwan_whoami; }
المستخدم() { helwan_whoami; }
usuario() { helwan_whoami; }
用户() { helwan_whoami; }

# 16. السجل
history_list() { helwan_history; }
سجل() { helwan_history; }
historial() { helwan_history; }
历史() { helwan_history; }

# 17. مسح الشاشة
clear_screen() { helwan_clear; }
مسح() { helwan_clear; }
limpiar() { helwan_clear; }
清屏() { helwan_clear; }

# 18. مساعدة
help() { helwan_help; }
مساعدة() { helwan_help; }
ayuda() { helwan_help; }
帮助() { helwan_help; }

# ==========================================
# الدوال الإضافية المكملة وأوامر النظام والشبكة
# ==========================================

helwan_orphans() {
    pacman -Qdtq
}

helwan_remove_orphans() {
    local orphans
    orphans=$(pacman -Qdtq)
    if [ -z "$orphans" ]; then
        echo "No orphaned packages found."
    else
        sudo pacman -Rns $orphans
    fi
}

helwan_service() {
    sudo systemctl "$@"
}

helwan_logs() {
    journalctl -b "$@"
}

helwan_keys() {
    sudo pacman-key "$@"
}

helwan_net() {
    ip link show
}

helwan_disk() {
    df -h "$@"
}

helwan_disks_list() {
    lsblk "$@"
}

helwan_memory() {
    free -h "$@"
}

helwan_modules() {
    lsmod "$@"
}

# ==========================================
# ربط الأسماء والأوامر الإضافية باللغات الأربع
# ==========================================

# الحزم اليتيمة
orphans() { helwan_orphans; }
يتيمة() { helwan_orphans; }
huérfanos() { helwan_orphans; }
孤儿() { helwan_orphans; }

# إزالة الحزم اليتيمة
remove_orphans() { helwan_remove_orphans; }
حذف_اليتيمة() { helwan_remove_orphans; }
eliminar_huerfano() { helwan_remove_orphans; }
删除孤儿() { helwan_remove_orphans; }

# الخدمات (Systemd)
service() { helwan_service "$@"; }
خدمة() { helwan_service "$@"; }
servicio() { helwan_service "$@"; }
服务() { helwan_service "$@"; }

# السجلات
logs() { helwan_logs "$@"; }
سجلات() { helwan_logs "$@"; }
registros() { helwan_logs "$@"; }
日志() { helwan_logs "$@"; }

# مفاتيح الحزم
keys() { helwan_keys "$@"; }
مفاتيح() { helwan_keys "$@"; }
llaves() { helwan_keys "$@"; }
密钥() { helwan_keys "$@"; }

# شبكة العتاد
net() { helwan_net; }
شبكة() { helwan_net; }
red() { helwan_net; }
网络() { helwan_net; }

# مساحة الأقراص
disk() { helwan_disk "$@"; }
أقراص() { helwan_disk "$@"; }
disco() { helwan_disk "$@"; }
磁盘() { helwan_disk "$@"; }

# قائمة الأقراص
disks_list() { helwan_disks_list "$@"; }
قائمة_الأقراص() { helwan_disks_list "$@"; }
lista_discos() { helwan_disks_list "$@"; }
磁盘列表() { helwan_disks_list "$@"; }

# الذاكرة العشوائية
memory() { helwan_memory "$@"; }
ذاكرة() { helwan_memory "$@"; }
memoria() { helwan_memory "$@"; }
内存() { helwan_memory "$@"; }

# وحدات النواة
modules() { helwan_modules "$@"; }
وحدات() { helwan_modules "$@"; }
módulos() { helwan_modules "$@"; }
内核模块() { helwan_modules "$@"; }

# العمليات
processes() { helwan_processes "$@"; }
العمليات() { helwan_processes "$@"; }
procesos() { helwan_processes "$@"; }
进程() { helwan_processes "$@"; }

# المنافذ
ports() { helwan_ports "$@"; }
المنافذ() { helwan_ports "$@"; }
puertos() { helwan_ports "$@"; }
端口() { helwan_ports "$@"; }

# عناوين IP
ip() { helwan_ip "$@"; }
آي_بي() { helwan_ip "$@"; }
ip_red() { helwan_ip "$@"; }
IP地址() { helwan_ip "$@"; }

# اختبار الاتصال (محافظ على أمر النظام الأصلي ولا يعرّف ping)
ping_host() { helwan_ping_host "$@"; }
بينج() { helwan_ping_host "$@"; }
持续ping() { helwan_ping_host "$@"; }

# إيقاف التشغيل
shutdown() { helwan_shutdown; }
إيقاف_التشغيل() { helwan_shutdown; }
apagar() { helwan_shutdown; }
关机() { helwan_shutdown; }

# إعادة التشغيل
reboot() { helwan_reboot; }
إعادة_تشغيل() { helwan_reboot; }
reiniciar() { helwan_reboot; }
重启() { helwan_reboot; }

# ==========================================
# وظائف AUR (أداة yay)
# ==========================================
helwan_aur_install() {
    if command -v yay &> /dev/null; then
        yay -S "$@"
    else
        echo "Error: 'yay' is not installed on this system."
    fi
}

helwan_aur_search() {
    if command -v yay &> /dev/null; then
        yay -Ss "$@"
    else
        echo "Error: 'yay' is not installed on this system."
    fi
}

helwan_aur_update() {
    if command -v yay &> /dev/null; then
        yay
    else
        echo "Error: 'yay' is not installed on this system."
    fi
}

helwan_aur_list() {
    if command -v yay &> /dev/null; then
        yay -Qm "$@"
    else
        echo "Error: 'yay' is not installed on this system."
    fi
}

helwan_aur_remove() {
    if command -v yay &> /dev/null; then
        yay -Rns "$@"
    else
        echo "Error: 'yay' is not installed on this system."
    fi
}

# تثبيت من AUR
aur_install() { helwan_aur_install "$@"; }
تثبيت_اور() { helwan_aur_install "$@"; }
aur_instalar() { helwan_aur_install "$@"; }
AUR安装() { helwan_aur_install "$@"; }

# إزالة من AUR
aur_remove() { helwan_aur_remove "$@"; }
حذف_اور() { helwan_aur_remove "$@"; }
aur_eliminar() { helwan_aur_remove "$@"; }
AUR删除() { helwan_aur_remove "$@"; }

# بحث في AUR
aur_search() { helwan_aur_search "$@"; }
بحث_اور() { helwan_aur_search "$@"; }
aur_buscar() { helwan_aur_search "$@"; }
AUR搜索() { helwan_aur_search "$@"; }

# تحديث وترقية نظام AUR + الرسمي
aur_update() { helwan_aur_update; }
تحديث_اور() { helwan_aur_update; }
aur_actualizar() { helwan_aur_update; }
AUR更新() { helwan_aur_update; }

# قائمة حزم AUR المثبتة
aur_list() { helwan_aur_list "$@"; }
قائمة_اور() { helwan_aur_list "$@"; }
aur_lista() { helwan_aur_list "$@"; }
AUR列表() { helwan_aur_list "$@"; }


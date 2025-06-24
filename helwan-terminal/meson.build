project('helwan-terminal', 'c',
  version : '0.1.0',
  default_options : ['c_std=gnu11'])

# Find dependencies
gtk_dep = dependency('gtk+-3.0')
vte_dep = dependency('vte-2.91', version: '>=0.50.0', required: true)
gio_dep = dependency('gio-unix-2.0')

# Source files
source_files = [
  'src/main.c',
  'src/terminal_window.c',
]

# Create an executable
executable('helwan-terminal',
  source_files,
  dependencies : [gtk_dep, vte_dep, gio_dep],
  install : true)

# Install data files (e.g., GSettings schema)
install_data(
  'data/helwan-terminal.gschema.xml',
  install_dir : join_paths(get_option('datadir'), 'glib-2.0', 'schemas')
)

# تثبيت ملف .desktop
install_data(
  'data/helwan-terminal.desktop',
  install_dir : join_paths(get_option('datadir'), 'applications')
)

# تثبيت الأيقونة بحجم 64x64
install_data(
  'data/icons/hicolor/64x64/apps/helwan-terminal.png',
  install_dir : join_paths(get_option('datadir'), 'icons', 'hicolor', '64x64', 'apps'),
  rename : 'helwan-terminal.png'
)

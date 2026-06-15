project('helwan-terminal', 'c',
  version : '0.1.0',
  default_options : ['warning_level=2', 'c_std=c11']
)

cc = meson.get_compiler('c')

deps = [
  dependency('gtk+-3.0'),
  dependency('vte-2.91'),
]

inc = include_directories('include')

sources = files(
  'src/main.c',
  'src/config.c',
  'src/utils.c',
  'src/shortcuts.c',
  'src/terminal_tab.c',
  'src/preferences.c',
  'src/terminal_window.c',
)

executable('helwan-terminal',
  sources,
  dependencies : deps,
  include_directories : inc,
  install : true,
)

# GSchema
install_data('data/helwan-terminal.gschema.xml',
  install_dir : get_option('datadir') / 'glib-2.0' / 'schemas'
)

# Desktop file
install_data('data/helwan-terminal.desktop',
  install_dir : get_option('datadir') / 'applications'
)

# Icon
install_data('data/icons/hicolor/64x64/apps/helwan-terminal.png',
  install_dir : get_option('datadir') / 'icons' / 'hicolor' / '64x64' / 'apps'
)

# Tests
test_utils = executable('test_utils',
  ['tests/test_utils.c', 'src/utils.c'],
  include_directories : inc,
  dependencies : [dependency('glib-2.0')],
)
test('utils', test_utils)

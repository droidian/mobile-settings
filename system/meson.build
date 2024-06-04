miatoll_settings_sources = [
  'bus.c',
  'main.c',
  '../common/utils.c'
]

miatoll_settings_deps = [
  dependency('glib-2.0'),
  dependency('gio-2.0'),
  dependency('gio-unix-2.0'),
  dependency('libgbinder')
]

executable('miatoll-settings', miatoll_settings_sources,
  dependencies: miatoll_settings_deps,
  install_dir: sbin_dir,
  install: true,
)

tuscany_sources = [
  'main.c',
  'terminal.c'
]

tuscany_deps = [
  dependency('glib-2.0'),
  dependency('libinput'),
  dependency('gio-2.0')
]

executable('tuscany', tuscany_sources,
  dependencies: tuscany_deps,
  install: true,
)


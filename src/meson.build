tuscany_sources = [
  'main.c',
  'terminal.c'
]

tuscany_deps = [
  dependency('glib-2.0'),
  dependency('libinput')
]

executable('tuscany', tuscany_sources,
  dependencies: tuscany_deps,
  install: true,
)


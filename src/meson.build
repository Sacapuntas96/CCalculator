ccalculator_sources = [
  'main.c',
  'ccalculator-application.c',
  'ccalculator-window.c',
]

ccalculator_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1', version: '>= 1.4'),
]

ccalculator_sources += gnome.compile_resources('ccalculator-resources',
  'ccalculator.gresource.xml',
  c_name: 'ccalculator'
)

executable('ccalculator', ccalculator_sources,
  dependencies: ccalculator_deps,
       install: true,
)

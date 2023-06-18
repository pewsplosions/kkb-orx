project('game', 'c')
add_global_arguments('-Wno-missing-braces', language: 'c')

cc = meson.get_compiler('c')

orx_lib_type = 'orxp'
if get_option('buildtype') == 'debug'
  orx_lib_type = 'orxd'
elif get_option('buildtype') == 'release'
  orx_lib_type = 'orx'
endif

libs = [
  cc.find_library('m', required : false),
  cc.find_library(orx_lib_type, dirs: meson.current_source_dir() + '/ext/orx')
]

incs = include_directories([
  'src',
  'ext/orx/inc',
  'ext/uthash/inc'
])

srcs = [
  'src/game.c',
  'src/player/player.c',
  'src/water/water.c',
  'src/clouds/clouds.c',
  'src/bird/bird.c',
  'src/flock/flock.c'
]

executable(
  'game',
  'src/game.c',
  sources: srcs,
  dependencies: libs,
  include_directories: incs
)

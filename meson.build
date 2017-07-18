project('dlvm', ['c'])

add_global_arguments('-std=c11', language : 'c')

inc = include_directories('include')

subdir('src')

executable('dlvm', 'main.c', link_with : dlvm, include_directories : inc)

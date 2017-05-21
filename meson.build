project('dlvm', ['c'])

inc = include_directories('src')

subdir('src')

executable('dlvm', 'main.c', link_with : dlvmlib, include_directories : inc)

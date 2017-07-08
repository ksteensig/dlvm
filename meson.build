project('dlvm', ['c'])

inc = include_directories('include')

subdir('src')

args = ['-std=c11']

executable('dlvm', 'main.c', c_args:args, link_with : dlvmlib, include_directories : inc)

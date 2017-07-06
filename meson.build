project('dlvm', ['c'])

inc = include_directories('src')

subdir('src')

args = ['-std=c11', '-g']

executable('dlvm', 'main.c', c_args:args, link_with : dlvmlib, include_directories : inc)

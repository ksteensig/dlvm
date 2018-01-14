project('dlvm', ['cpp'])

add_global_arguments('-std=c++17', language : 'cpp')
#add_global_arguments('-g', language : 'cpp')

include = include_directories('src')
subdir('src')

executable('dlvm', 'src/main.cpp', link_with: dlvm, include_directories : include)
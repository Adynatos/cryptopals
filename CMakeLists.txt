cmake_minimum_required(VERSION 2.8)

project(cryptopals)

enable_testing()

add_compile_options(-std=c++11)

add_subdirectory(main)
add_subdirectory(test)
add_subdirectory(libcrypto)

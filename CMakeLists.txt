cmake_minimum_required(VERSION 2.8)

project(cryptopals)

enable_testing()

add_compile_options(-std=c++14)

add_subdirectory(solutions)
add_subdirectory(test)
add_subdirectory(libcrypto)

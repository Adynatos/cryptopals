add_executable(ex4
    ex4.cpp
)

add_executable(ex6
    ex6.cpp)

target_link_libraries(ex4
    libcrypto
)

target_link_libraries(ex6
    libcrypto
)

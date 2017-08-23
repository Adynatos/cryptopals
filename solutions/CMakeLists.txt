add_executable(ex4
    ex4.cpp
)

add_executable(ex6
    ex6.cpp)

add_executable(ex7
    ex7.cpp)

add_executable(ex8
    ex8.cpp)

add_executable(ex10
    ex10.cpp)

add_executable(ex11
    ex11.cpp)

add_executable(ex12
    ex12.cpp)

add_executable(ex13
    ex13.cpp)

find_package(OpenSSL REQUIRED)

target_link_libraries(ex4
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex6
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex7
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex8
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex10
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex11
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex12
    libcrypto
    ${OPENSSL_LIBRARIES}
)

target_link_libraries(ex13
    libcrypto
    ${OPENSSL_LIBRARIES}
)

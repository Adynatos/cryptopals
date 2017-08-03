set(Main_SRCS
    main.cpp
)

add_executable(myApp
    ${Main_SRCS}
)

add_executable(ex6
    ex6.cpp)

target_link_libraries(ex6
    libcrypto
)

# Specify here the libraries this program depends on
target_link_libraries(myApp
    libcrypto
)

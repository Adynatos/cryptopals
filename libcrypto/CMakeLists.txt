set(libcrypto_SRCS
    helpers.hpp
    helpers.cpp
)

# Declare the library
add_library(libcrypto SHARED
    ${libcrypto_SRCS}
)

# Specify here the include directories exported
# by this library
target_include_directories(libcrypto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(libcrypto PROPERTIES LINKER_LANGUAGE CXX)

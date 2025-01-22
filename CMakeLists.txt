cmake_minimum_required(VERSION 3.12)
project(SignScheme VERSION 1.0.0 LANGUAGES C)

# Set custom install directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

# Check if running on MacOS for OpenSSL path
if(APPLE)
    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
    set(OPENSSL_INCLUDE_DIR "/opt/homebrew/opt/openssl@3/include")
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define the library sources
set(SIGN_SCHEME_SOURCES
    src/sign_scheme.c

    src/helper/int2uc.c
    src/helper/uc2int.c

    src/eddsa/key_gen.c
    src/eddsa/read_key.c
    src/eddsa/sign.c
    src/eddsa/verify.c
    
    src/ecdsa/key_gen.c
    src/ecdsa/read_key.c
    src/ecdsa/sign.c
    src/ecdsa/verify.c
    
    src/rsa/key_gen.c
    src/rsa/read_key.c
    src/rsa/sign.c
    src/rsa/verify.c
    
    src/no_sign/key_gen.c
    src/no_sign/read_key.c
    src/no_sign/sign.c
    src/no_sign/verify.c
)

# Create the library
add_library(SignScheme ${SIGN_SCHEME_SOURCES})

# Set include directories for the library
target_include_directories(SignScheme
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/eddsa
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ecdsa
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rsa
        ${CMAKE_CURRENT_SOURCE_DIR}/src/no_sign
        ${CMAKE_CURRENT_SOURCE_DIR}/src/helper
        ${OPENSSL_INCLUDE_DIR}
)

# Link against OpenSSL
target_link_libraries(SignScheme PRIVATE OpenSSL::Crypto)

# Installation rules
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR lib/cmake/SignScheme)

# Install main library target
install(TARGETS SignScheme
    EXPORT SignSchemeTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install header files
# Main header
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# Module headers - create the subdirectories and install the headers
install(FILES src/rsa/rsa.h DESTINATION include/rsa)
install(FILES src/eddsa/eddsa.h DESTINATION include/eddsa)
install(FILES src/ecdsa/ecdsa.h DESTINATION include/ecdsa)
install(FILES src/no_sign/no_sign.h DESTINATION include/no_sign)
install(FILES src/helper/helper.h DESTINATION include/helper)

# Export targets
install(EXPORT SignSchemeTargets
    FILE SignSchemeTargets.cmake
    NAMESPACE SignScheme::
    DESTINATION ${INSTALL_CONFIGDIR}
)

# Create config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SignSchemeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/SignSchemeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/SignSchemeConfig.cmake"
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SignSchemeConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SignSchemeConfigVersion.cmake"
    DESTINATION ${INSTALL_CONFIGDIR}
)
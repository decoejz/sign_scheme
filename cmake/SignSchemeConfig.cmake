include(CMakeFindDependencyMacro)

# Find dependencies
find_dependency(OpenSSL REQUIRED)

# Import targets
include("${CMAKE_CURRENT_LIST_DIR}/SignSchemeTargets.cmake")
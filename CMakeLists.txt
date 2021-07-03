cmake_minimum_required(VERSION 3.2)

if (NOT DEFINED ENV{GITHUB_RUN_NUMBER})
    set(ENV{GITHUB_RUN_NUMBER} 1)
endif()
project(bulk VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(bulk ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/inc/Logger.h)

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;-pedantic"
)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

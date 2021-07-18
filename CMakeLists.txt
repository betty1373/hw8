cmake_minimum_required(VERSION 3.2)

if (NOT DEFINED ENV{GITHUB_RUN_NUMBER})
    set(ENV{GITHUB_RUN_NUMBER} 1)
endif()
project(bayan VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

file(GLOB TARGET_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h")
add_executable(bayan ${TARGET_SRC})

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;-pedantic;-Werror"
)
target_link_libraries(bayan PRIVATE ${Boost_LIBRARIES})

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

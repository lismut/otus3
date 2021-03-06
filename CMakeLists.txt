# INIT REQUIREMENTS
cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER}) 
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(allocator VERSION 0.0.3)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package (Threads REQUIRED)
# CONFIGURATIONS
configure_file(version.h.in "${CMAKE_CURRENT_SOURCE_DIR}/version.h")

add_subdirectory(
       "${CMAKE_CURRENT_SOURCE_DIR}/gtest"
       gtest
)

# ADD SOURCES
add_executable(allocator main.cpp)
add_library(lib.lib lib.cpp)
add_library(alloc.lib allocator.cpp)
add_library(container.lib container.cpp)
add_executable(testboost testboost.cpp)
add_executable(testgoogle testgoogle.cpp)

# CPP STANDARD REQUIREMENTS
set_target_properties(allocator lib.lib testboost testgoogle PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(lib.lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(testboost PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(allocator PRIVATE
    lib.lib alloc.lib container.lib
)
target_link_libraries(testboost
    ${Boost_LIBRARIES}
    lib.lib
)

target_link_libraries(testgoogle
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
    lib.lib
)

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
    target_compile_options(lib.lib PRIVATE
        /W4
    )
    target_compile_options(testboost PRIVATE
        /W4
    )
    target_compile_options(testgoogle PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(lib.lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(testboost PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(testgoogle PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT lismut@gmail.com)

include(CPack)

enable_testing()

add_test(testboost testboost)
add_test(GOOGLE_TEST1 testgoogle)
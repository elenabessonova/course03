cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

find_package(GTest REQUIRED)

configure_file(version.h.in version.h)

add_executable(custom_allocator_cli main.cpp)

add_executable(test_custom test_custom.cpp)

set_target_properties(custom_allocator_cli test_custom PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


target_link_libraries(test_custom
    GTest::gtest
    GTest::gtest_main
    
)

if (MSVC)
    target_compile_options(custom_allocator_cli PRIVATE
        /W4
    )
    target_compile_options(test_custom PRIVATE
        /W4
    )
else ()
    target_compile_options(custom_allocator_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_custom PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS custom_allocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_custom test_custom)

cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(matrix VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(matrix VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_executable(matrix_cli main.cpp)
add_library(matrix lib.cpp)
add_executable(autotests autotests.cpp)

set_target_properties(matrix_cli matrix autotests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(autotests PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(matrix_cli PRIVATE
        matrix
)
target_link_libraries(autotests
    ${Boost_LIBRARIES}
        matrix
)

if (MSVC)
    target_compile_options(matrix_cli PRIVATE
        /W4
    )
    target_compile_options(matrix PRIVATE
        /W4
    )
    target_compile_options(autotests PRIVATE
        /W4
    )
else ()
    target_compile_options(matrix_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(autotests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS matrix_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)

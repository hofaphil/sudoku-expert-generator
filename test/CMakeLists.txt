include(FetchContent)
FetchContent_Declare(
        gtest
        QUIET
        URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(
        generator-tests
        SudokuTests.cpp
)

target_link_libraries(
        generator-tests
        generator
        gtest_main
)

add_test(NAME AllTests COMMAND generator-tests)

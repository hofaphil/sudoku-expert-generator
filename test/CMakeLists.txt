include(FetchContent)
FetchContent_Declare(
        gtest
        QUIET
        URL https://github.com/google/googletest/archive/release-1.11.0.tar.gz
)
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(
        generator-tests
        SudokuTests.cpp
        NumberTests.cpp
        BlockTests.cpp
)

target_link_libraries(
        generator-tests
        generator
        gtest_main
)

add_test(NAME AllTests COMMAND generator-tests)

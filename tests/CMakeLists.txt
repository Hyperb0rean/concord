
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE TESTS_SOURCES "*.cpp")

add_executable(tests ${TESTS_SOURCES})

target_link_libraries(
  tests
  axis
  accord
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
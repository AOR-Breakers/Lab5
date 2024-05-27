enable_testing()

add_executable(hello_test local_search_test.cpp)
target_link_libraries(hello_test GTest::gtest_main LocalSearch)

include(GoogleTest)
gtest_discover_tests(hello_test)

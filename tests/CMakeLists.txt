enable_testing()

add_executable(SECSY_tests
    test_ecs_entity.cpp
    test_ecs_registry.cpp
)

target_link_libraries(SECSY_tests PRIVATE
    SECSY
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(SECSY_tests)

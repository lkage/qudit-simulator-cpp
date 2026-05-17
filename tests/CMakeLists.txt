find_package(GTest REQUIRED)

add_executable(test_qudit_state test_qudit_state.cpp)
target_link_libraries(test_qudit_state
    PRIVATE qudit_simulator_core GTest::gtest GTest::gtest_main
)

add_executable(test_pauli_operator test_pauli_operator.cpp)
target_link_libraries(test_pauli_operator
    PRIVATE qudit_simulator_core GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_qudit_state)
gtest_discover_tests(test_pauli_operator)
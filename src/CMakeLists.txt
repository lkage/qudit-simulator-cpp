add_library(qudit_simulator_core
    qudit_state.cpp
)
target_include_directories(qudit_simulator_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(qudit_simulator_core PUBLIC Eigen3::Eigen)
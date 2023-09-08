target_sources(${PROJECT_NAME} PUBLIC
    state_machine_system.cpp
    state_machine_builder.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    components
)
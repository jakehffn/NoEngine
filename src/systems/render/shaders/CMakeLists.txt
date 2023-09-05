target_sources(${PROJECT_NAME} PUBLIC
    shader_program.cpp
    shader_manager.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
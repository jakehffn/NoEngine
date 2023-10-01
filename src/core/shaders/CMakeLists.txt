target_sources(${PROJECT_NAME} PUBLIC
    shader_manager.cpp
    shader_loader.cpp
    shader_program.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
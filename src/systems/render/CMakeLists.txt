target_sources(${PROJECT_NAME} PUBLIC
    render_system.cpp
    renderer.cpp
    shader_loader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    components
)

add_subdirectory(shaders)
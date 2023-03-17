target_sources(${PROJECT_NAME} PUBLIC
    animation_system.cpp
    camera_system.cpp
    movement_system.cpp
    text_system.cpp
    collision_system.cpp
    gui_system.cpp
    input_system.cpp
    
    map_loader/map_loader_system.cpp
    
    render/render_system.cpp
    render/shader_loader.cpp
    render/renderer.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render/shaders)
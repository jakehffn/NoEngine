target_sources(${PROJECT_NAME} PUBLIC
    animation/animation_system.cpp
    
    camera/camera_system.cpp
    camera/camera.cpp
    
    map_loader/map_loader_system.cpp
    map_loader/texture_atlas.cpp

    movement/movement_system.cpp

    render/render_system.cpp
    render/shader_loader.cpp

    collision_system.cpp
    gui_system.cpp
    input_system.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render/shaders)
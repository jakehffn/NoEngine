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

    state_machine/state_machine_system.cpp
    state_machine/state_machine_builder.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    map_loader
    render
    state_machine
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/render/shaders)
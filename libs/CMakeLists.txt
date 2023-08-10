target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} #stb_image.h
    entt
    glm
    rapidjson
    retpack2d
    lightgrid/include
    imgui
    imgui/backends
)

target_sources(${PROJECT_NAME} PUBLIC
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
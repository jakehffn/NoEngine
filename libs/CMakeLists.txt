target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} #stb_image.h
    entt
    glm
    rapidjson
    retpack2d
    lightgrid/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
        imgui
        imgui/backends
    )
    add_library(imgui SHARED
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_sdl2.cpp
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
    )
    target_include_directories(imgui PUBLIC
        imgui
        imgui/backends
    )
endif()
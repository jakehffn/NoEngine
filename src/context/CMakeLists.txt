target_sources(${PROJECT_NAME} PUBLIC
    camera.cpp
    clock.cpp
    input.cpp
    texture_atlas.cpp
    sprite_sheet_atlas.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
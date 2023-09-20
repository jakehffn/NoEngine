target_sources(${PROJECT_NAME} PUBLIC
    clock.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(component_grid)
add_subdirectory(map_loader)
add_subdirectory(resource_loader)
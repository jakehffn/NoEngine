target_sources(${PROJECT_NAME} PUBLIC
    collision_system.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    components
)
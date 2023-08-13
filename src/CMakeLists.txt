target_sources(${PROJECT_NAME} PUBLIC
    game.cpp
    main.cpp
    debug.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    prefabs
)

add_subdirectory(context)
add_subdirectory(systems)
add_subdirectory(components)
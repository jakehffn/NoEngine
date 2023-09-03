target_sources(${PROJECT_NAME} PUBLIC
    game.cpp
    main.cpp
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${PROJECT_NAME} PUBLIC
        debug.cpp
        debug_timer.cpp
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    prefabs
)

add_subdirectory(context)
add_subdirectory(systems)
add_subdirectory(components)
target_sources(${PROJECT_NAME} PUBLIC
    game.cpp
    main.cpp
)

add_subdirectory(context)
add_subdirectory(systems)
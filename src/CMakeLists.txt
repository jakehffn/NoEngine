target_sources(${PROJECT_NAME} PUBLIC
    Game.cpp
    main.cpp
)

add_subdirectory(systems)
add_subdirectory(util)
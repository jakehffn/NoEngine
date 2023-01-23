target_sources(${PROJECT_NAME} PUBLIC
    renderSystem/RenderSystem.cpp
    AnimationSystem.cpp
    CameraSystem.cpp
    CollisionSystem.cpp
    GUISystem.cpp
    InputSystem.cpp
    MapLoaderSystem.cpp
    MovementSystem.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/renderSystem/shaderPrograms)
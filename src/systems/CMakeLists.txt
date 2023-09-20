target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(animation)
add_subdirectory(camera)
add_subdirectory(collision)
add_subdirectory(gui)
add_subdirectory(input)
add_subdirectory(movement)
add_subdirectory(render)
add_subdirectory(state_machine)
target_sources(${PROJECT_NAME} PUBLIC
    gui_system.cpp
    dialog_builder.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    components
)

add_subdirectory(context)
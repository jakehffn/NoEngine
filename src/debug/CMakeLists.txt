target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    components
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${PROJECT_NAME} PUBLIC
        debug.cpp
        debug_timer.cpp
    )
endif()
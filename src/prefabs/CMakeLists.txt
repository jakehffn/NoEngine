target_sources(${PROJECT_NAME} PUBLIC
    kid.cpp
    test_npc.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
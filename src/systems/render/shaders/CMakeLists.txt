target_sources(${PROJECT_NAME} PUBLIC
    instanced/instanced_shader.cpp
    screen/screen_shader.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    instanced
    screen
)

file(COPY "./instanced/src" DESTINATION "./instanced")
file(COPY "./screen/src" DESTINATION "./screen")
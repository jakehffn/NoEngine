target_sources(${PROJECT_NAME} PUBLIC
    instanced/instanced_shader.cpp
    screen/screen_shader.cpp
)

file(COPY "./instanced/src" DESTINATION "./instanced")
file(COPY "./screen/src" DESTINATION "./screen")
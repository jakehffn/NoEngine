cmake_minimum_required(VERSION 3.23)

project(NoEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(libs)
add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/)

find_package(OpenGL REQUIRED)

set(GLEW_VERBOSE true)
set(GLEW_ROOT "C:/msys64/mingw64/include/GL")
find_package(GLEW REQUIRED)

set(SDL2_PATH "C:/msys64/mingw64/include/SDL2")
find_package(SDL2 REQUIRED)

set(FREETYPE2_PATH "C:/msys64/mingw64/lib/libfreetype.dll.a")
set(FREETYPE2_INCLUDE_DIR "C:/msys64/mingw64/include/freetype2")

set(TMXLITE_PATH "C:/Program Files (x86)/tmxlite/lib/libtmxlite.dll.a")
set(TMXLITE_INCLUDE_DIR "C:/Program Files (x86)/tmxlite/include")

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR} ${TMXLITE_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC -mconsole ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${FREETYPE2_PATH} ${TMXLITE_PATH} GLEW::glew)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_include_directories(imgui PUBLIC ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR})
    target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARY} ${SDL2_LIBRARY})
    target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
    install(TARGETS imgui)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

file(COPY "./assets" DESTINATION ".")

# target_compile_options(${PROJECT_NAME} PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
# )
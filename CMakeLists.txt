cmake_minimum_required(VERSION 3.23)

project(UntitledRPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src)
add_subdirectory(headers)
add_subdirectory(libs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake/)

find_package(OpenGL REQUIRED)

set(GLEW_VERBOSE true)
set(GLEW_ROOT "C:/mingw_dev_lib/glew-2.1.0")
find_package(GLEW REQUIRED)

set(SDL2_PATH "C:/mingw_dev_lib/SDL2")
find_package(SDL2 REQUIRED)
message(STATUS GLEW_LIBRARIES=${GLEW_LIBRARIES})

set(TMXLITE_PATH "C:/mingw_dev_lib/tmxlite/lib/libtmxlite.a")
set(TMXLITE_INCLUDE_DIR "C:/mingw_dev_lib/tmxlite/include")

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${TMXLITE_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC -mconsole ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${TMXLITE_PATH} GLEW::glew)

# target_compile_options(${PROJECT_NAME} PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
#   $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
# )
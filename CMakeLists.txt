cmake_minimum_required (VERSION 3.2)
project (LambdaEngine)

file(GLOB engine_SRC
     "engine/*.c"
     "engine/*.h")
add_executable(lambda ${engine_SRC})
set_target_properties(lambda PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambda physfs dl)

file(GLOB lambda_video_SRC
     "lambda_video/*.c"
     "lambda_video/*.h")

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
add_library(lambda_video SHARED ${lambda_video_SRC})
find_library(OpenGL_LIBRARY OpenGL )
target_link_libraries(lambda_video ${SDL2_LIBRARIES} ${OpenGL_LIBRARY})
set_target_properties(lambda_video PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

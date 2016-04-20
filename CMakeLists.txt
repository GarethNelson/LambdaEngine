cmake_minimum_required (VERSION 3.2)
project (LambdaEngine)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

find_library(OpenGL_LIBRARY OpenGL )

file(GLOB engine_SRC
     "engine/*.c"
     "engine/*.h")
add_executable(lambda ${engine_SRC})
set_target_properties(lambda PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambda physfs dl)

file(GLOB lambda_video_SRC
     "lambda_video/*.c"
     "lambda_video/*.h")
add_library(lambda_video SHARED ${lambda_video_SRC})
target_link_libraries(lambda_video ${SDL2_LIBRARIES} ${OpenGL_LIBRARY})
set_target_properties(lambda_video PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

file(GLOB lambda_render_SRC
     "soil/*.c"
     "lambda_render/*.c"
     "lambda_render/*.h")
add_library(lambda_render SHARED ${lambda_render_SRC})
target_link_libraries(lambda_render ${OpenGL_LIBRARY})
set_target_properties(lambda_render PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

add_custom_target(core01_md5sums ALL
                 DEPENDS lambda_video lambda_render
                 COMMAND  "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_md5sums.sh"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)


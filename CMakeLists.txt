cmake_minimum_required (VERSION 3.2)
project (LambdaEngine)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/soil ${CMAKE_CURRENT_SOURCE_DIR}/include)


find_library(OpenGL_LIBRARY OpenGL )

file(GLOB engine_SRC
     "engine/*.c"
     "engine/*.h")
add_executable(lambda ${engine_SRC})
set_target_properties(lambda PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambda physfs dl ${OpenGL_LIBRARY})

file(GLOB lambda_video_SRC
     "lambda_video/*.c"
     "lambda_video/*.h")
add_library(lambda_video SHARED ${lambda_video_SRC})
target_link_libraries(lambda_video ${SDL2_LIBRARIES} ${OpenGL_LIBRARY})
set_target_properties(lambda_video PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   set(EXTRA_GL GL)
endif()

file(GLOB lambda_render_SRC
     "soil/*.c"
     "lambda_render/*.c"
     "lambda_render/*.h")
add_library(lambda_render SHARED ${lambda_render_SRC})
target_link_libraries(lambda_render ${OpenGL_LIBRARY} ${EXTRA_GL} m)
set_target_properties(lambda_render PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

add_custom_target(core01_md5sums ALL
                 DEPENDS lambda_video lambda_render
                 COMMAND  "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_md5sums.sh"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

add_custom_target(core01_textures ALL
                  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/textures
                  )
                 
add_custom_target(core02_gameinfo ALL
                 ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample/gameinfo.def ${CMAKE_CURRENT_SOURCE_DIR}/build/core02/gameinfo.def)

add_custom_target(core02_md5sums ALL
                 DEPENDS lambda_video lambda_render
                 COMMAND  "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_md5sums.sh"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core02/libs)

add_custom_target(core01.pak ALL
                  DEPENDS core01_textures lambda_video lambda_render core01_md5sums
                  COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${CMAKE_CURRENT_SOURCE_DIR}/dist/data/core01.pak --format=zip
                  *
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01)


add_custom_target(core02.pak ALL
                  DEPENDS core02_gameinfo core02_md5sums
                  COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${CMAKE_CURRENT_SOURCE_DIR}/dist/data/core02.pak --format=zip
                  *
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core02)

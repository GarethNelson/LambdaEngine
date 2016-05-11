cmake_minimum_required (VERSION 3.2)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMakeFiles")
project (LambdaEngine)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(FREETYPE2 REQUIRED freetype2)
find_package(ImageMagick REQUIRED COMPONENTS MagickWand)
include_directories(${SDL2_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/soil
                    ${FREETYPE2_INCLUDE_DIRS}
                    ${ImageMagick_MagickWand_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(SDL2TTF REQUIRED)
#find_library(OPENGL_LIBRARIES OpenGL )

find_package(OpenGL REQUIRED)
file(GLOB engine_SRC
     "engine/*.c"
     "engine/*.h")
add_executable(lambda ${engine_SRC})
set_target_properties(lambda PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambda pthread physfs dl ${OPENGL_LIBRARIES})

file(GLOB lambdamc_SRC
     "tools/lambdamc/*.c"
     "tools/lambdamc/*.h")
add_executable(lambdamc ${lambdamc_SRC})
set_target_properties(lambdamc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
target_link_libraries(lambdamc )

file(GLOB lambdacut_SRC
     "tools/lambdacut/*.c"
     "tools/lambdacut/*.h")
add_executable(lambdacut ${lambdacut_SRC})
set_target_properties(lambdacut PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist/bin)
set_target_properties(lambdacut PROPERTIES COMPILE_FLAGS "-DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16")
target_link_libraries(lambdacut ${ImageMagick_MagickWand_LIBRARIES})


file(GLOB lambda_video_SRC
     "lambda_video/*.c"
     "lambda_video/*.h")
add_library(lambda_video SHARED ${lambda_video_SRC})
target_link_libraries(lambda_video ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
set_target_properties(lambda_video PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)


file(GLOB lambda_input_SRC
     "lambda_input/*.c"
     "lambda_input/*.h")
add_library(lambda_input SHARED ${lambda_input_SRC})
target_link_libraries(lambda_input ${SDL2_LIBRARIES} )
set_target_properties(lambda_input PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   set(EXTRA_GL GL)
endif()

file(GLOB lambda_render_SRC
     "soil/*.c"
     "lambda_render/*.c"
     "lambda_render/*.h")
add_library(lambda_render SHARED ${lambda_render_SRC})
target_link_libraries(lambda_render ${OPENGL_LIBRARIES} ${EXTRA_GL} ${FREETYPE2_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARY} m)
set_target_properties(lambda_render PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

add_custom_target(core01_md5sums ALL
                 DEPENDS lambda_video lambda_render
                 COMMAND  "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_md5sums.sh"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/libs)

add_custom_target(core01_fonts ALL
                  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/fonts ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/fonts
                  )

add_custom_target(core01_textures ALL
                  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures ${CMAKE_CURRENT_SOURCE_DIR}/build/core01/textures
                  )
                 
add_custom_target(core02_textures ALL
                  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/sample/textures ${CMAKE_CURRENT_SOURCE_DIR}/build/core02/textures
                  )

add_custom_target(core02_gameinfo ALL
                 ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample/gameinfo.def ${CMAKE_CURRENT_SOURCE_DIR}/build/core02/gameinfo.def)

add_custom_target(core02_md5sums ALL
                 DEPENDS lambda_video lambda_render
                 COMMAND  "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_md5sums.sh"
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core02/libs)

add_custom_target(core01.pak ALL
                  DEPENDS core01_fonts lambda_input core01_textures lambda_video lambda_render core01_md5sums
                  COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${CMAKE_CURRENT_SOURCE_DIR}/dist/data/core01.pak --format=zip
                  *
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core01)


add_custom_target(core02.pak ALL
                  DEPENDS core02_textures core02_gameinfo core02_md5sums
                  COMMAND ${CMAKE_COMMAND} -E tar "cfv" ${CMAKE_CURRENT_SOURCE_DIR}/dist/data/core02.pak --format=zip
                  *
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/core02)

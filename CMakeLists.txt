cmake_minimum_required (VERSION 2.6)
project (LambdaEngine)

file(GLOB engine_SRC
     "engine/*.c"
     "engine/*.h")
add_executable(lambda ${engine_SRC})
target_link_libraries(lambda dl)
target_link_libraries(lambda physfs)

cmake_minimum_required(VERSION 3.16)
project(LibSWBF2_Tests)

add_executable(parse_level ${PROJECT_SOURCE_DIR}/parse_level.c)
target_link_libraries(parse_level PRIVATE LibSWBF2)

if (UNIX)
  target_compile_options(parse_level PUBLIC -Wall -Wextra -Wno-attributes -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare)

  # Optionally build with GCC/Clang sanitizers
  option(BUILD_SAN "Build with sanitizers" OFF)
  if(BUILD_SAN)
    target_compile_options(parse_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3 -O0)
    target_link_options   (parse_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3 -O0)
  endif()
endif(UNIX)

if(MSVC)
  target_compile_options(parse_level PRIVATE "/MP /W4 /WX")
endif()

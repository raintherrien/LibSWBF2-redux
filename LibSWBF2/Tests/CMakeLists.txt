cmake_minimum_required(VERSION 3.16)
project(LibSWBF2_Tests)

add_executable(standard_level ${PROJECT_SOURCE_DIR}/standard_level.c)
add_executable(addon_level ${PROJECT_SOURCE_DIR}/addon_level.c)
target_link_libraries(standard_level PRIVATE LibSWBF2)
target_link_libraries(addon_level PRIVATE LibSWBF2)

if (UNIX)
  target_compile_options(standard_level PUBLIC -Wall -Wextra -Wno-attributes -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare)
  target_compile_options(addon_level PUBLIC -Wall -Wextra -Wno-attributes -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare)

  # Optionally build with GCC/Clang sanitizers
  option(BUILD_SAN "Build with sanitizers" OFF)
  if(BUILD_SAN)
    target_compile_options(standard_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3)
    target_link_options   (standard_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3)
    target_compile_options(addon_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3)
    target_link_options   (addon_level PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3)
  endif()
endif(UNIX)

if(MSVC)
  target_compile_options(standard_level PRIVATE "/MP /W4 /WX")
  target_compile_options(addon_level PRIVATE "/MP /W4 /WX")
endif()

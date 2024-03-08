set(PROJECT_NAME 2048)

cmake_minimum_required(VERSION 3.15.0)

project(${PROJECT_NAME})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found. Please install SDL2 and its development libraries.")
endif()

if (NOT SDL2_image_FOUND)
    message(FATAL_ERROR "SDL2_image not found. Please install SDL2 and its development libraries.")
endif()

if (NOT SDL2_ttf_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found. Please install SDL2 and its development libraries.")
endif()

add_library(board src/board.c)
add_library(unity include/Unity/src/unity.c include/Unity/src/unity.h include/Unity/src/unity_internals.h)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(board SDL2::Main SDL2::Image SDL2::TTF)
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF board)

file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(board_test tests/board.c)
target_link_libraries(board_test board unity)

enable_testing()

add_test(
  NAME board_test
  COMMAND $<TARGET_FILE:board_test>
  )

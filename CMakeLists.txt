set(PROJECT_NAME 2048)

cmake_minimum_required(VERSION 3.15.0)

project(${PROJECT_NAME})

file(GLOB SOURCES "src/*.c")

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

# include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::TTF)

file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

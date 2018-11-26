cmake_minimum_required(VERSION 3.12)
project(visualsort C)

set(CMAKE_C_STANDARD 11)

add_executable(visualsort src/main.c src/rendering.c src/rendering.h)

include_directories(/usr/local/lib)
include_directories(/usr/local/include)
include_directories(/usr/local/Cellar)
include_directories(/Library/Frameworks)

find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    target_include_directories(visualsort PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(visualsort PRIVATE ${SDL2_LIBRARIES})
endif ()

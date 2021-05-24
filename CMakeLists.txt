# cmake_minimum_required(VERSION <specify CMake version here>)
project(Lab2 C)

set(CMAKE_C_STANDARD 11)

set(allocator_SOURCES main.c include/allocator.h include/driver.h include/macros.h include/test.h allocator.c test.c include/slab.h include/object.h slab.c include/cache.h include/mask.h mask.c cache.c)

include_directories("include")

if(WIN32)
    set(allocator_SOURCES ${allocator_SOURCES} Drivers/windriver.c)
elseif(UNIX)
    set(allocator_SOURCES ${allocator_SOURCES} Drivers/linuxdriver.c Utils/align_utils.c)
endif()

add_executable(Lab1 ${allocator_SOURCES})
# cmake_minimum_required(VERSION 3.16)
cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0...3.18.4)
project(Particles)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(glfw/include)

IF(APPLE) # Mac
    IF (CMAKE_OSX_ARCHITECTURES==x86_64)
        link_directories(glfw/mac/lib-x86_64)
    ELSEIF(CMAKE_OSX_ARCHITECTURES==arm64)
        link_directories(glfw/mac/lib-arm64)
    ELSE()
       link_directories(glfw/mac/lib-universal)
    ENDIF()
ELSEIF(UNIX) # Linux
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    link_directories(glfw/unix)
ELSEIF(WIN32) # Windows
    link_directories(glfw/win) # Windows
ENDIF()

find_package(OpenGL REQUIRED)
# find_package(OpenCLHeaders REQUIRED)
find_package(OpenCL REQUIRED)
# find_package(OpenCLICDLoader REQUIRED)
# find_package(OpenCLHeadersCpp REQUIRED)

add_executable(Particles
        glad/glad.c
        src/main.cpp
        src/scene.cpp
        src/object.cpp
        src/shader.cpp
        src/cl_particales.cpp
        include/scene.h
        include/object.h
        include/shader.h
        include/cl_particales.h
        )

IF(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    target_link_libraries(Particles "libglfw3.a" ${OPENGL_LIBRARIES} ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
ELSEIF(UNIX) # Linux
    target_link_libraries(Particles PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS})
ELSEIF(WIN32) # Windows
    target_link_libraries(Particles "libglfw3.a" OpenGL::GL OpenCL::OpenCL)
ENDIF ()
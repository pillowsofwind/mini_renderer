CMAKE_MINIMUM_REQUIRED(VERSION 3.9)
PROJECT(ray_tracer CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(ray_tracer_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/scene_parser.cpp)

SET(ray_tracer_INCLUDES
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/ray.hpp
        include/scene_parser.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/render.hpp include/bitmap_image.hpp include/texture.hpp include/noise.hpp include/cube_map.hpp include/octree.hpp src/octree.cpp include/perlin_noise.hpp src/perlin_noise.cpp include/kdtree.hpp include/simple_method.hpp src/render.cpp src/kdtree.cpp include/bezier.hpp src/bezier.cpp)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_EXECUTABLE(${PROJECT_NAME} ${ray_tracer_SOURCES} ${ray_tracer_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE vecmath)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)

#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j
cd ..

# Run all testcases. 
# You can comment some lines to disable the run of specific examples.
mkdir -p output
#bin/ray_tracer testcases/Final_scene0.txt output/Final_scene0.bmp
#bin/ray_tracer testcases/Final_scene1.txt output/Final_scene1.bmp
#bin/ray_tracer testcases/Final_scene1_copy.txt output/Final_scene1_copy.bmp
bin/ray_tracer testcases/Final_scene2.txt output/Final_scene2.bmp


cmake_minimum_required(VERSION 3.5)

project(calcscript LANGUAGES CXX)

add_executable(calcscript
    main.cpp
)

target_compile_features(calcscript PRIVATE cxx_std_11)
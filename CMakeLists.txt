cmake_minimum_required(VERSION 3.10)

project(calcc LANGUAGES CXX)

add_executable(calcc
    main.cpp
    
    src/lexer.cpp
    src/parser.cpp
)

target_compile_features(calcc PRIVATE cxx_std_11)

target_include_directories(calcc PRIVATE include)
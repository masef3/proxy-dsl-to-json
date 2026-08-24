cmake_minimum_required(VERSION 3.20)
project(dslproxy)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dsl/examples
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/dsl)

add_executable(dslproxy
    dsl/lexer.cpp
    main.cpp
)



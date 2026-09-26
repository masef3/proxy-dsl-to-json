cmake_minimum_required(VERSION 3.20)
project(tj)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(nlohmann_json 3.2.0 REQUIRED)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dsl/examples
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/dsl)

add_executable(tj
    dsl/lexer.cpp
    dsl/parser.cpp
    dsl/jsongen.cpp
    main.cpp
)

target_link_libraries(tj PRIVATE nlohmann_json::nlohmann_json)



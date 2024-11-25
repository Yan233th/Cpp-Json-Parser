cmake_minimum_required (VERSION 3.11)
project (Cpp-Json-Parser)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories (./includes)

add_executable(
    Cpp-Json-Parser
    "./src/main.cpp"
    "./src/Resolver_impl.cpp"
)
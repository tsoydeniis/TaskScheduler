cmake_minimum_required(VERSION 3.12)

project(
        TaskSchedulerProgect
        VERSION 1.0
        DESCRIPTION "lab_10"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(src)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)
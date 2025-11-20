cmake_minimum_required(VERSION 3.5.0)
project(EasyCutter
    VERSION 0.0.1
    DESCRIPTION "This is a free video processor program"
    HOMEPAGE_URL "https://github.com/Ren-Pen/EasyCutter"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(tests)

cmake_minimum_required(VERSION 3.10)

project(RecipeProcessor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_path(GUMBO_INCLUDE_DIR gumbo.h /usr/include/gumbo.h)
find_library(GUMBO_LIBRARY NAMES gumbo /usr/lib/x86_64-linux-gnu/libgumbo.so)

find_package(TBB REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(gumbo-parser/src)

add_executable(RecipeProcessor 
    main.cpp
    impl/DBManager.cpp
    impl/Logger.cpp
    impl/Parser.cpp
    impl/TaskManager.cpp
)

target_include_directories(RecipeProcessor PRIVATE ./inc)

target_link_libraries(RecipeProcessor 
    PRIVATE 
        TBB::tbb 
        SQLite::SQLite3 
        gumbo
)

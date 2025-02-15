cmake_minimum_required(VERSION 3.10)

project(RecipeExtractor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)

add_executable(RecipeExtractor main.cpp curlwrap.cpp threadpool.cpp)

target_link_libraries(RecipeExtractor PRIVATE ${CURL_LIBRARIES})

target_include_directories(RecipeExtractor PRIVATE ${CURL_INCLUDE_DIRS})

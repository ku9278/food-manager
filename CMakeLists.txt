cmake_minimum_required(VERSION 3.31)

project(food-manager)

set(program food_manager)
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${program} 
    food_manager.cpp 
    ${SOURCE_FILES}
)

target_include_directories(${program} PUBLIC 
    ${CMAKE_SOURCE_DIR}/include 
    ${CMAKE_SOURCE_DIR}/openai 
    ${CMAKE_SOURCE_DIR}/curl-8.11.0_1-win64-mingw/include
)

target_link_libraries(${program} PUBLIC 
    ${CMAKE_SOURCE_DIR}/curl-8.11.0_1-win64-mingw/lib/libcurl.dll.a
)

set_target_properties(${program} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
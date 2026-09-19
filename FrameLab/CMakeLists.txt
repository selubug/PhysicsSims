cmake_minimum_required(VERSION 3.20)
project(FrameLab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(framelab_physics
    PhysicsCore/src/ClassicalEtherModel.cpp
    PhysicsCore/src/SpecialRelativityModel.cpp
    PhysicsCore/src/MichelsonMorleyExperiment.cpp
)

target_include_directories(framelab_physics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsCore/include
)

if(MSVC)
    target_compile_options(framelab_physics PRIVATE /W4 /permissive-)
else()
    target_compile_options(framelab_physics PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(framelab_cli apps/main.cpp)
target_link_libraries(framelab_cli PRIVATE framelab_physics)

add_executable(framelab_tests tests/MichelsonMorleyTests.cpp)
target_link_libraries(framelab_tests PRIVATE framelab_physics)
add_test(NAME MichelsonMorleyTests COMMAND framelab_tests)

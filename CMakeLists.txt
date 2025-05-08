cmake_minimum_required(VERSION 3.10)
project(App)

find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ColourfulTriangles.cpp src/glad.c)

# Tell the compiler where to find GLAD headers
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw dl)
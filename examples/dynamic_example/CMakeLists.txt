cmake_minimum_required(VERSION 3.0.0)

# Set the Project name
set(PROJECT_NAME
        dynamic_example
        )

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} gemmi)

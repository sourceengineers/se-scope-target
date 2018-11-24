#Wrapper for the jsm library
cmake_minimum_required(VERSION 3.0.0)

# Set the Project name
set(PROJECT_NAME
    jsmn
)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} jsmn/jsmn.h jsmn/jsmn.c)

install( FILES jsmn/jsmn.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" COMPONENT library )

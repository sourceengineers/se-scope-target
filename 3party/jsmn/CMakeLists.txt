#Wrapper for the jsm library
cmake_minimum_required(VERSION 3.0.0)

# Set the Project name
set(PROJECT_NAME
    jsmn
)

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} jsmn/jsmn.h jsmn/jsmn.c)

TARGET_INCLUDE_DIRECTORIES (jsmn
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

install( FILES jsmn/jsmn.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include" )
install( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" COMPONENT library )

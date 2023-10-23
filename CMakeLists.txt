# Generate Solution -> 'cmake -B ./Solution'
cmake_minimum_required(VERSION 3.18.2)

#  ============== PROJECT ================
project(KittyPongProj)

# ============== CONFIG ================

set(SOURCE_PROJECT_DIR Source) 
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
set(BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)

macro(ADD_DEFINE IDENT)
    add_definitions("-D${IDENT}")
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CmakeGenerated")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Add the source files with proper folder structure
file(GLOB_RECURSE SOURCES "Source/*")
source_group(TREE ${PROJECT_SOURCE_DIR}/Source PREFIX "Source Files/" FILES ${SOURCES})

# create .exe 
add_executable(KittyPong ${SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KittyPong)

# ============== ENGINE ==============
set(MON_DEV_BUILD_GAME OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/Lib/MonEngine)

target_link_libraries(
    KittyPong 
    PRIVATE
    MonEngine
)

target_include_directories(
    KittyPong
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

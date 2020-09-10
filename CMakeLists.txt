# Generate Solution -> 'cmake -B ./Solution -A Win32'

# set cmake version
cmake_minimum_required(VERSION 3.18)

# set project name
project(KittyPong)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set property to organize targets into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# set property to name CMake's automatically generated folder
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "PredefinedTargets")

# set cmake to search for library file types
set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
# initialize property for all targets
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# specifies build configurations for project
set(CMAKE_CONFIGURATION_TYPES 
    Release 
    Debug 
    CACHE TYPE INTERNAL FORCE )

# define vars for Folder paths
set(SOURCE_PROJECT_DIR Source) 
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)
set(BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)

# set all source files to var
set(PROJECT_SRC_FILES
    "${SOURCE_PROJECT_DIR}/AIPlayer.cpp"
    "${SOURCE_PROJECT_DIR}/AIPlayer.h"
    "${SOURCE_PROJECT_DIR}/COGBounce.cpp"
    "${SOURCE_PROJECT_DIR}/COGBounce.h"
    "${SOURCE_PROJECT_DIR}/COGBoxShape.cpp"
    "${SOURCE_PROJECT_DIR}/COGBoxShape.h"
    "${SOURCE_PROJECT_DIR}/COGCircleShape.cpp"
    "${SOURCE_PROJECT_DIR}/COGCircleShape.h"
    "${SOURCE_PROJECT_DIR}/COGPhysics.cpp"
    "${SOURCE_PROJECT_DIR}/COGPhysics.h"
    "${SOURCE_PROJECT_DIR}/COGShape.cpp"
    "${SOURCE_PROJECT_DIR}/COGShape.h"
    "${SOURCE_PROJECT_DIR}/COGTransform.h"
    "${SOURCE_PROJECT_DIR}/Component.h"
    "${SOURCE_PROJECT_DIR}/Game.cpp"
    "${SOURCE_PROJECT_DIR}/Game.h"
    "${SOURCE_PROJECT_DIR}/GameInterface.h"
    "${SOURCE_PROJECT_DIR}/GameObject.h"
    "${SOURCE_PROJECT_DIR}/Input.h"
    "${SOURCE_PROJECT_DIR}/IPhysicsCollisionEvent.h"
    "${SOURCE_PROJECT_DIR}/Main.cpp"
    "${SOURCE_PROJECT_DIR}/Score.cpp"
    "${SOURCE_PROJECT_DIR}/Score.h"
    "${SOURCE_PROJECT_DIR}/ScoreManager.cpp"
    "${SOURCE_PROJECT_DIR}/ScoreManager.h"
    "${SOURCE_PROJECT_DIR}/UserInput.cpp"
    "${SOURCE_PROJECT_DIR}/UserInput.h"
    "${SOURCE_PROJECT_DIR}/World.cpp"
    "${SOURCE_PROJECT_DIR}/World.h"
)
# file( GLOB PROJECT_SRC_FILES ${SOURCE_PROJECT_DIR}/*.cpp ${SOURCE_PROJECT_DIR}/*.h )

# Organize filters based on directory structure
source_group( TREE ${CMAKE_SOURCE_DIR} FILES PROJECT_SRC_FILES)
# Set Debug binary files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BINARY_DIR}/Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BINARY_DIR}/Debug)

# create .exe 
add_executable(KittyPong WIN32 ${PROJECT_SRC_FILES})

# link library files (keywords: optimized, debug, general)
# winmm specifies windows libraries
target_link_libraries(
KittyPong 
    optimized
    "${THIRD_PARTY_DIR}/EngineX/lib/EngineX.lib"
    debug
    "${THIRD_PARTY_DIR}/EngineX/lib/EngineXd.lib"
    general
    "${THIRD_PARTY_DIR}/SDL/SDL2-2.0.3/lib/SDL2.lib"
    "${THIRD_PARTY_DIR}/SDL/SDL2_gfx-1.0.1/lib/SDL2-gfx.lib"
    "${THIRD_PARTY_DIR}/SDL/SDL2_ttf-2.0.12/lib/SDL2-ttf.lib"
    winmm.lib
    
)

target_include_directories(
KittyPong
    PUBLIC
        ${SOURCE_PROJECT_DIR}
        "${THIRD_PARTY_DIR}/EngineX/include"
        "${THIRD_PARTY_DIR}/SDL/SDL2-2.0.3/include"
        "${THIRD_PARTY_DIR}/SDL/SDL2_gfx-1.0.1/include"
        "${THIRD_PARTY_DIR}/SDL/SDL2_ttf-2.0.12/include"
)

set_property(TARGET KittyPong PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:KittyPong>)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KittyPong)

add_custom_command(
TARGET KittyPong 
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${THIRD_PARTY_DIR}/SDL/SDL2-2.0.3/lib/SDL2.dll"
        "${THIRD_PARTY_DIR}/SDL/SDL2_gfx-1.0.1/lib/SDL2-gfx.dll"
        "${THIRD_PARTY_DIR}/SDL/SDL2_ttf-2.0.12/lib/SDL2-ttf.dll"
        "${THIRD_PARTY_DIR}/FreeType/lib/libfreetype-6.dll"
        "${THIRD_PARTY_DIR}/ZLib/lib/zlib1.dll"
    $<TARGET_FILE_DIR:KittyPong>)

    
add_custom_command( TARGET KittyPong  PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets/Fonts/
    $<TARGET_FILE_DIR:KittyPong>)
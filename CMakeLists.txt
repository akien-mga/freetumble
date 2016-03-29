cmake_minimum_required(VERSION 2.8)

######################
# Project definition #
######################

project(freetumble)

set(FT_MAJOR_VERSION 1)
set(FT_MINOR_VERSION 0)
set(FT_PATCH_VERSION 0)
set(FT_VERSION "${FT_MAJOR_VERSION}.${FT_MINOR_VERSION}")
if(FT_PATCH_VERSION GREATER 0)
    set(FT_VERSION "${FT_VERSION}.${FT_PATCH_VERSION}")
endif()

add_definitions(-DVERSION="${FT_VERSION}")

option(FOR_INSTALL "Build the binary to look for its data in system directories." OFF)

if(FOR_INSTALL)
    # Set binary and data install locations if we want to use the installer
    set(FT_BIN_PATH ${CMAKE_INSTALL_PREFIX}/games CACHE PATH "Absolute path to the game binary directory")
    set(FT_DATA_PATH ${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME} CACHE PATH "Absolute path to the game data directory")
    set(FT_SHARE_PATH ${CMAKE_INSTALL_PREFIX}/share CACHE PATH "Absolute path to the shared data directory (desktop file, icons, etc.)")

    # Set the data paths
    add_definitions(-DDATADIR="${FT_DATA_PATH}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "CMake build type is not set, defaulting to 'RelWithDebInfo'")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

################
# Source files #
################

set(SRC ${CMAKE_SOURCE_DIR}/src)

set(FT_SOURCEFILES
    ${SRC}/gameEngine/GameInput.cpp
    ${SRC}/gameEngine/GameMain.cpp
    ${SRC}/gameEngine/GameModel.cpp
    ${SRC}/gameEngine/GameScores.cpp
    ${SRC}/gameEngine/Menu.cpp
    ${SRC}/gameEngine/MenuEntry.cpp

    ${SRC}/gfxEngine/EntityManager.cpp
    ${SRC}/gfxEngine/GameEntity.cpp
    ${SRC}/gfxEngine/GfxEngine.cpp

    ${SRC}/main.cpp
)

# Create the binary file
add_executable(${PROJECT_NAME} ${FT_SOURCEFILES})

#####################
# Find dependencies #
#####################

find_package(SFML 2 REQUIRED COMPONENTS Audio Graphics System Window)

#######################
# Headers and linking #
#######################

include_directories(
    # FreeTumble includes
    ${SRC}

    # External libraries
    SYSTEM ${SFML_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(
    # target
    ${PROJECT_NAME}

    # libraries
    ${SFML_LIBRARIES}
)

################
# Installation #
################

if(FOR_INSTALL)
    set(FT_DATADIRS  ${CMAKE_SOURCE_DIR}/gfx
                     ${CMAKE_SOURCE_DIR}/music
                     ${CMAKE_SOURCE_DIR}/sfx
                     ${CMAKE_SOURCE_DIR}/skins
                     ${CMAKE_SOURCE_DIR}/tilesets)

    set(FT_DATAFILES ${CMAKE_SOURCE_DIR}/FreeSansBold.ttf
                     ${CMAKE_SOURCE_DIR}/reset_scores.dat)

    set(FT_DOC       ${CMAKE_SOURCE_DIR}/LICENSE.txt
                     ${CMAKE_SOURCE_DIR}/README.md)

    install(TARGETS ${PROJECT_NAME}
            DESTINATION ${FT_BIN_PATH}
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(DIRECTORY ${FT_DATADIRS}
            DESTINATION ${FT_DATA_PATH})
    install(FILES ${FT_DATAFILES}
            DESTINATION ${FT_DATA_PATH})

    # Additional distribution content (desktop file, icon)
    install(FILES ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.desktop
            DESTINATION ${FT_SHARE_PATH}/applications)
    install(FILES ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.png
            DESTINATION ${FT_SHARE_PATH}/icons/hicolor/64x64/apps)
    install(FILES ${FT_DOC}
            DESTINATION ${FT_SHARE_PATH}/doc/${PROJECT_NAME})
endif()

cmake_minimum_required(VERSION 3.16)

project(Samosbor   VERSION 0.1
                    DESCRIPTION ""
                    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Samosbor
    src/main.cpp
    src/game.cpp                                    includes/game.hpp
    src/entity/entity.cpp                           includes/entity/entity.hpp
    src/player/player.cpp                           includes/player/player.hpp
    src/monster/monster.cpp                         includes/monster/monster.hpp

    core/scene/scene.cpp                            core/scene/scene.hpp
    core/managers/scenemanager/scenemanager.hpp     core/managers/scenemanager/scenemanager.cpp
    core/managers/resourcemanager.hpp               core/managers/resourcemanager.tpp
    core/managers/input/inputmanager.hpp            core/managers/input/inputmanager.cpp
    core/managers/event/eventmanager.hpp            core/managers/event/eventmanager.tpp

    gamescene/gamescene.hpp                         gamescene/gamescene.cpp

    #panelscene/panelscene.hpp                       panelscene/panelscene.cpp
)

target_include_directories(Samosbor PRIVATE
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/entity
    ${CMAKE_SOURCE_DIR}/includes/player
    ${CMAKE_SOURCE_DIR}/includes/monster

    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/entity
    ${CMAKE_SOURCE_DIR}/src/player
    ${CMAKE_SOURCE_DIR}/src/monster

    ${CMAKE_SOURCE_DIR}/core/scene
    ${CMAKE_SOURCE_DIR}/core/managers
    ${CMAKE_SOURCE_DIR}/core/managers/scenemanager
    ${CMAKE_SOURCE_DIR}/core/managers/input
    ${CMAKE_SOURCE_DIR}/core/managers/event

    ${CMAKE_SOURCE_DIR}/gamescene

    #${CMAKE_SOURCE_DIR}/panelscene  
)


# Лінкування з notcurses
find_library(NOTCURSES_LIB notcurses)
find_library(NOTCURSES_CORE_LIB notcurses-core)

target_link_libraries(Samosbor ${NOTCURSES_CORE_LIB})
target_link_libraries(Samosbor ${NOTCURSES_LIB})

include(GNUInstallDirs)
install(TARGETS Samosbor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
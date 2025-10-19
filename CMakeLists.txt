cmake_minimum_required(VERSION 3.16)

project(Samosbor   VERSION 0.2
                    DESCRIPTION ""
                    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(Samosbor
    src/main.cpp
    src/game.cpp                                includes/game.hpp 
    src/settings.cpp                            includes/settings.hpp
    src/entity/entity.cpp                       includes/entity/entity.hpp 
    src/player/player.cpp                       includes/player/player.hpp 
    src/monster/monster.cpp                     includes/monster/monster.hpp 
    src/item/item.cpp                           includes/item/item.hpp
    src/item/itemregistry.cpp                   includes/item/itemregistry.hpp
    src/inventory/inventory.cpp                 includes/inventory/inventory.hpp

    core/managers/resourcemanager.hpp           core/managers/resourcemanager.tpp 
    core/managers/input/inputmanager.hpp        core/managers/input/inputmanager.cpp 
    core/managers/scenemanager/scenemanager.hpp core/managers/scenemanager/scenemanager.cpp
    core/managers/panel/panelmanager.cpp        core/managers/panel/panelmanager.hpp 
     
    core/scene/scene.hpp                        core/scene/scene.cpp 
    core/scene/gamescene/gamescene.hpp          core/scene/gamescene/gamescene.cpp
    core/scene/menu/menu.hpp                    core/scene/menu/menu.cpp
)

target_include_directories(Samosbor PRIVATE
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/entity
    ${CMAKE_SOURCE_DIR}/includes/player
    ${CMAKE_SOURCE_DIR}/includes/monster
    ${CMAKE_SOURCE_DIR}/includes/item
    ${CMAKE_SOURCE_DIR}/includes/inventory

    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/entity
    ${CMAKE_SOURCE_DIR}/src/player
    ${CMAKE_SOURCE_DIR}/src/monster
    ${CMAKE_SOURCE_DIR}/src/item

    ${CMAKE_SOURCE_DIR}/core/scene
    ${CMAKE_SOURCE_DIR}/core/scene/gamescene
    ${CMAKE_SOURCE_DIR}/core/scene/menu

    ${CMAKE_SOURCE_DIR}/core/managers
    ${CMAKE_SOURCE_DIR}/core/managers/input
    ${CMAKE_SOURCE_DIR}/core/managers/scenemanager
    ${CMAKE_SOURCE_DIR}/core/managers/panel
)


find_library(NOTCURSES_LIB libnotcurses)
find_library(NOTCURSES_CORE_LIB libnotcurses-core)
find_package(nlohmann_json REQUIRED)

target_link_libraries(Samosbor ${NOTCURSES_CORE_LIB})
target_link_libraries(Samosbor ${NOTCURSES_LIB})
target_link_libraries(Samosbor nlohmann_json::nlohmann_json)

include(GNUInstallDirs)
install(TARGETS Samosbor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

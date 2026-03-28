cmake_minimum_required(VERSION 3.14)
project(game)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
find_package(raylib_cpp QUIET)
if (NOT raylib_cpp_FOUND)
    # if (NOT DEFINED RAYLIB_CPP_VERSION)
    #     set(RAYLIB_CPP_VERSION next)
    # endif()
    include(FetchContent)
    FetchContent_Declare(
        raylib_cpp
        GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
        GIT_TAG v5.5.0
    )
    FetchContent_MakeAvailable(raylib_cpp)
endif()

include_directories(
    include
    external

    include/text_adventure
    include/text_adventure/databases
    include/text_adventure/dialogue
    include/text_adventure/states
    include/text_adventure/ui
)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This is the main part:
set(SOURCES 
    src/main.cpp

    src/text_adventure/dialogue/choice.cpp
    src/text_adventure/dialogue/dialogue_manager.cpp
    src/text_adventure/dialogue/dialogue.cpp
    src/text_adventure/dialogue/action_factory.cpp

    src/text_adventure/state/condition_factory.cpp
    src/text_adventure/state/condition.cpp
    src/text_adventure/state/game_state.cpp

    src/text_adventure/ui/rpg_text.cpp
    src/text_adventure/ui/ui_manager.cpp

    src/text_adventure/databases/config_loader.cpp
    src/text_adventure/databases/item_loader.cpp
    src/text_adventure/databases/flag_registry.cpp
    src/text_adventure/databases/variable_registry.cpp
    src/text_adventure/databases/effect_loader.cpp

    src/text_adventure/helper.cpp
    src/text_adventure/ke_engine.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raylib_cpp)

# That's it! You should have an example executable that you can run. Have fun!

#CMakeLists.txt
cmake_minimum_required (VERSION 3.6)
project(server)

add_executable(server server.cpp clienthandler.cpp clienthandler.h dbmanager.cpp dbmanager.h discordbot.cpp discordbot.h includes.h)

add_subdirectory(deps/sleepy-discord)
target_link_libraries(server sleepy-discord)
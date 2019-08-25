cmake_minimum_required(VERSION 3.0)

project(app)
find_package(clog REQUIRED)
find_package(crosssocket REQUIRED)
find_package(crypt)

add_executable(app rat.c)
target_link_libraries(app clog::clog)
target_link_libraries(app crosssocket::crosssocket)
target_link_libraries(app crypt)

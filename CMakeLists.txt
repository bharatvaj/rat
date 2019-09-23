cmake_minimum_required(VERSION 3.0)

project(rat)
file(GLOB SRCS "src/*.c")
find_package(clog REQUIRED)
find_package(crosssocket REQUIRED)

add_executable(rat ${SRCS} rat_server.c)

target_include_directories(rat PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

set_target_properties(rat PROPERTIES
	LINK_FLAGS "-Wall -Os"
)

target_link_libraries(rat clog::clog)
target_link_libraries(rat crosssocket::crosssocket)

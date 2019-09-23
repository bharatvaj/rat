cmake_minimum_required(VERSION 3.0)

project(rat)
find_package(clog REQUIRED)
find_package(crosssocket REQUIRED)
# find_package(crypt)

add_executable(rat rat.c)

target_include_directories(rat PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

target_link_libraries(rat clog::clog)
target_link_libraries(rat crosssocket::crosssocket)
# target_link_libraries(app crypt)

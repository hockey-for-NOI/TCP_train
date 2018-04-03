CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(include)

set(CMAKE_CXX_FLAGS "-std=c++11 -O2")

add_library(server SHARED
		src/server_child.cpp
		src/server.cpp
		src/server_scheduler.cpp
		src/server_storage.cpp)

target_link_libraries(server pthread)

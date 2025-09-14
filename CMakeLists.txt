cmake_minimum_required(VERSION 3.10)
project(autosub CXX)

find_package(CURL REQUIRED)

file(GLOB_RECURSE SRC *.cc)

add_executable(autosub ${SRC})
target_link_libraries(autosub PRIVATE CURL::libcurl)

include(GNUInstallDirs)
install(TARGETS autosub DESTINATION ${CMAKE_INSTALL_BINDIR})


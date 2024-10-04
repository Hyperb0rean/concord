cmake_minimum_required(VERSION 3.20)

project(concord)

include(cmake/Logging.cmake)
include(cmake/Ccache.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Platform.cmake)
include(cmake/Development.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(axis)
add_subdirectory(concord)

add_subdirectory(tests)

add_subdirectory(playground)


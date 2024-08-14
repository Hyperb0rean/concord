file(GLOB_RECURSE PLAYGROUND_SOURCES "*.cpp")

add_executable(playground ${PLAYGROUND_SOURCES})

target_link_libraries(playground fmt accord axis)

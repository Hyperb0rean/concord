set(LIB_TARGET accord)

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

add_library(${LIB_TARGET} STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

# set(LIB_DEPENDENCIES fmt sure twist wheels)
# target_link_libraries(${LIB_TARGET} PUBLIC ${LIB_DEPENDENCIES})

set_target_properties(accord PROPERTIES LINKER_LANGUAGE CXX)

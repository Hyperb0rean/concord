set(LIB_TARGET concord)

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH} *.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH} *.hpp)

set(CONTEXT "${LIB_PATH}/cord/context/context.S")

enable_language(ASM)
set_property(SOURCE ${CONTEXT} PROPERTY LANGUAGE ASM)

add_library(${LIB_TARGET} STATIC ${CONTEXT} ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(${LIB_TARGET} PUBLIC ${LIB_INCLUDE_PATH})

set(LIB_DEPENDENCIES fmt axis)
target_link_libraries(${LIB_TARGET} PUBLIC ${LIB_DEPENDENCIES})

set_target_properties(concord PROPERTIES LINKER_LANGUAGE CXX)

# Common compile options for C++

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# https://clang.llvm.org/docs/DiagnosticsReference.html
add_compile_options(-Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer)

# Turn warnings into errors
add_compile_options(-Wno-language-extension-token)

add_compile_options(-Wno-unused-command-line-argument -Wno-unused-function)

add_compile_options(-gdwarf-4)

# libc++
add_compile_options(-stdlib=libstdc++)
add_link_options(-stdlib=libstdc++)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Common compile options for C++

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# https://clang.llvm.org/docs/DiagnosticsReference.html
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g -fno-omit-frame-pointer")

# Turn warnings into errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-language-extension-token")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -Wno-unused-function")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4")

# libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++")

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

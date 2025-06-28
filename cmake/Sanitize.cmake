# Sanitizers

# --------------------------------------------------------------------

# Address Sanitizer
# https://clang.llvm.org/docs/AddressSanitizer.html

set(ASAN_COMPILE_FLAGS -fsanitize=address,undefined,leak -fno-sanitize-recover=all)
set(ASAN_LINK_FLAGS -fsanitize=address,undefined,leak)

if(CMAKE_BUILD_TYPE MATCHES "Asan")
    message(STATUS "Sanitize with Address Sanitizer (ASAN)")
    add_compile_options(${ASAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ASAN_LINK_FLAGS}")
endif()

# --------------------------------------------------------------------

# Thread Sanitizer
# https://clang.llvm.org/docs/ThreadSanitizer.html

set(TSAN_COMPILE_FLAGS -fsanitize=thread -fno-sanitize-recover=all)
set(TSAN_LINK_FLAGS -fsanitize=thread)

if(CMAKE_BUILD_TYPE MATCHES "Tsan")
    message(STATUS "Sanitize with Thread Sanitizer (TSAN)")
    add_compile_options(${TSAN_COMPILE_FLAGS})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${TSAN_LINK_FLAGS}")
endif()

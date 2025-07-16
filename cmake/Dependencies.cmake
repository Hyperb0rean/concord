include(FetchContent)

project_log("FetchContent: fmt")

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)

set(LIBURING_LIB_PATH ${CMAKE_CURRENT_LIST_DIR}/../third_party/liburing/src/liburing.a)
if (NOT EXISTS ${LIBURING_LIB_PATH})
    message(STATUS "${CMAKE_CURRENT_LIST_DIR}/../third_party/liburing")
    execute_process(
            COMMAND ./configure
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../third_party/liburing
    )

    execute_process(
            COMMAND make
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../third_party/liburing
    )
    message(STATUS "Built liburing")
endif ()

set(LIB_URING "${CMAKE_CURRENT_LIST_DIR}/../third_party/liburing/src/include")
message(STATUS "${LIB_URING}")

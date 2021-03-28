cmake_minimum_required(VERSION 3.17)
project(hashtable C)

set(CMAKE_C_STANDARD 99)

set(HASHTABLE_SRC_DIR "src")
set(HASHTABLE_HEADERS
        "${HASHTABLE_SRC_DIR}/command.h"
        "${HASHTABLE_SRC_DIR}/hash.h"
        "${HASHTABLE_SRC_DIR}/log.h"
        "${HASHTABLE_SRC_DIR}/table.h")
set(HASHTABLE_SRC
        "${HASHTABLE_SRC_DIR}/command.c"
        "${HASHTABLE_SRC_DIR}/hash.c"
        "${HASHTABLE_SRC_DIR}/log.c"
        "${HASHTABLE_SRC_DIR}/table.c"
        "${HASHTABLE_SRC_DIR}/main.c")

add_executable(hashtable ${HASHTABLE_HEADERS} ${HASHTABLE_SRC})

add_custom_target(run-hashtable
        COMMAND ./hashtable
        DEPENDS hashtable
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

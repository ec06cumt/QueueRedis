#Author:Gliu
#Created:2015/12/20

file(GLOB SRC_LISTS *.cpp *.hpp)

set (CUR_INC_BASE ${CMAKE_CURRENT_SOURCE_DIR}/BaseLib)

add_executable(${REDISWORLD_EXE} ${SRC_LISTS})
link_directories(${CUR_INC_BASE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(${REDISWORLD_EXE} ${ALL_LAS} ${MYSQLCLIENT_LIB})


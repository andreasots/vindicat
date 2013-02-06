cmake_minimum_required(VERSION 2.8.2)
project(vindicat CXX C)

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-std=c++11 -Wall -Wextra -g -pg)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(NACL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(LEMON REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/lib/libev)
add_definitions(-DEV_STANDALONE)
include_directories(${PROJECT_SOURCE_DIR}/lib/libsocket)
include_directories(${PROJECT_SOURCE_DIR}/lib/ed25519-donna)
include_directories(${PROJECT_SOURCE_DIR}/lib/keccak)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${LEMON_INCLUDE_DIRS})
include_directories(${NACL_INCLUDE_DIRS})
# The following are needed for compiling against internal nacl
# Internal nacl is broken
#include_directories(${CMAKE_BINARY_DIR}/nacl_build/include_internal)
#include_directories(${CMAKE_BINARY_DIR}/nacl_build/crypto_box/curve25519xsalsa20poly1305)

FILE(GLOB_RECURSE srcs src/*.c src/*.cc src/*.cpp)
FILE(GLOB_RECURSE libs lib/libsocket/*.c lib/ed25519-donna/*.c lib/keccak/*.c lib/libev/ev.c lib/libev/event.c)
FILE(GLOB_RECURSE clibs lib/*.cc lib/*.cpp lib/randombytes.c)

ADD_EXECUTABLE(vindicat ${clibs} ${libs} ${srcs})
set_source_files_properties(${libs} PROPERTIES LANGUAGE CXX)

target_link_libraries(vindicat ${PROTOBUF_LITE_LIBRARIES} ${NACL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES} ${LEMON_LIBRARIES})
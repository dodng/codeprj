cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")

project(NetworkFrame)
set(EXECUTABLE_OUTPUT_PATH ${NetworkFrame_SOURCE_DIR}/bin/)
add_definitions(-g -D _LINUX64)
include_directories(
./
./frame/
../
../helper
../lib-mine/libevent/
../lib-mine/libevent/include/
./main/
)

link_directories(
${NetworkFrame_SOURCE_DIR}/../lib-mine/libevent/lib
)

file(GLOB ALL_HEADER_SRCS
./*.h
./frame/*.h
../lib-mine/libevent/*.h
../lib-mine/libevent/include/*.h
./main/*.h
../type.h
../helper/strUtil.h
../helper/assert.h
../helper/thread.h
)

file(GLOB ALL_CXX_SRCS
./*.cpp
./frame/*.cpp
../lib-mine/libevent/*.cpp
../lib-mine/libevent/include/*.cpp
./main/*.cpp
../helper/strUtil.cpp
../helper/assert.cpp
../helper/thread.cpp
)

set(ALL_LIB dl pthread libevent.a libevent_core.a libevent_extra.a libevent_openssl.a libevent_pthreads.a)
add_executable(NetworkFrame ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(NetworkFrame ${ALL_LIB})

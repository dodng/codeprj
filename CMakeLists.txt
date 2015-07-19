cmake_minimum_required(VERSION 2.8)
set(CMAKE_BUILID_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g gdb3 -Wno-invalid-offsetof")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g gdb3 -O2 -Wno-invalid-offsetof")


project(CodePrj)
set(EXECUTABLE_OUTPUT_PATH ${CodePrj_SOURCE_DIR}/./bin/)
add_definitions(-g -D _LINUX64)
include_directories(
./
./algorithm
./datastruct
./designPattern
./fileParser
./fileParser/xml
./helper
./lib-mine/libConhash/include
./lib-mine/libCurl/include
./lib-mine/libJsonc/include
./lib-mine/libMySql-linux/include
./lib-mine/libNet/include
./lib-mine/openssl/include
./main
./network
./reflect
./seh
./tcpSocketIO
./test
)

link_directories(
${CodePrj_SOURCE_DIR}/lib-mine/lib/
)

file(GLOB ALL_HEADER_SRCS
./*.h
./algorithm/*.h
./datastruct/*.h
./designPattern/*.h
./fileParser/*.h
./fileParser/xml/*.h
./helper/*.h
./lib-mine/libConhash/include/*.h
./lib-mine/libCurl/include/*.h
./lib-mine/libJsonc/include/*.h
./lib-mine/libMySql-linux/include/*.h
./lib-mine/libNet/include/*.h
./main/*.h
./network/*.h
./reflect/*.h
./seh/*.h
./tcpSocketIO/*.h
./test/*.h
)

file(GLOB ALL_CXX_SRCS
./*.cpp
./algorithm/*.cpp
./datastruct/*.cpp
./designPattern/*.cpp
./fileParser/*.cpp
./fileParser/xml/*.cpp
./helper/*.cpp
./libNetTest/*.cpp
./main/*.cpp
./reflect/*.cpp
./seh/*.cpp
./tcpSocketIO/*.cpp
./test/*.cpp
)

set(ALL_LIB dl pthread curl crypto Conhash Jsonc MySql mysqlclient Net rt)
add_executable(CodePrj ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(CodePrj ${ALL_LIB})

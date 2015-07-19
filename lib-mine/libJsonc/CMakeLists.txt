project(Jsonc)
set(LIBRARY_OUTPUT_PATH ${Jsonc_SOURCE_DIR}/../lib/)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./
./include
../
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
../type.h
../helper/assert.h
)
file(GLOB ALL_CXX_SRCS
./src/*.cpp
../helper/assert.cpp
)

set(ALL_LIB pthread dl)
add_library(Jsonc STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(Jsonc ${ALL_LIB})

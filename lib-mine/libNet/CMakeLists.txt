project(Net)
set(LIBRARY_OUTPUT_PATH ${Net_SOURCE_DIR}/../lib/)
add_definitions(-g -D _LIB -D _LINUX64)
include_directories(
./
./include
../../
../../helper
../../network
)

file(GLOB ALL_HEADER_SRCS
./include/*.h
../../type.h
../../helper/strUtil.h
../../helper/assert.h
../../network/packet.h
)
file(GLOB ALL_CXX_SRCS
./src/*.cpp
../../helper/strUtil.cpp
../../helper/assert.cpp
)

set(ALL_LIB pthread dl)
add_library(Net STATIC ${ALL_HEADER_SRCS} ${ALL_CXX_SRCS})
target_link_libraries(Net ${ALL_LIB})

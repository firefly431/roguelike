INCLUDE_DIRECTORIES(${Roguelike_SOURCE_DIR}/ext/include)
ADD_LIBRARY(external_libs src/lodepng.cpp)
SET_PROPERTY(TARGET external_libs PROPERTY CXX_STANDARD 14)

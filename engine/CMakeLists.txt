cmake_minimum_required(VERSION 4.3.2)

set(CMAKE_CXX_COMPILER g++)

project(perp CXX)

file(GLOB PROTO_SRCS "${CMAKE_SOURCE_DIR}/gen/cpp/*.pb.cc")

find_package(Protobuf CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp ${PROTO_SRCS})

target_include_directories(perp PRIVATE ${CMAKE_SOURCE_DIR}/gen/cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotobuf)
target_link_libraries(${PROJECT_NAME} PRIVATE redis++::redis++_static)
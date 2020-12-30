# enable x.y version format
cmake_policy(SET CMP0048 NEW)
project(CommonLib VERSION 0.1)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

option(BUILD_UNO_LIB OFF)
option(BUILD_TEXAS_LIB OFF)

# if this file is in top directory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BUILD_UNO_LIB ON)
    set(BUILD_TEXAS_LIB ON)
endif()

# Proto file
set(proto_dir "${CMAKE_CURRENT_SOURCE_DIR}/proto")
file(GLOB proto_files ${proto_dir}/core/*.proto)
if(BUILD_UNO_LIB)
    file(GLOB uno_proto_files ${proto_dir}/uno/*.proto)
    list(APPEND proto_files ${uno_proto_files})
endif()
if(BUILD_TEXAS_LIB)
    file(GLOB texas_proto_files ${proto_dir}/texas/*.proto)
    list(APPEND proto_files ${texas_proto_files})
endif()

# Generated pb files
set(pb_dir "${CMAKE_BINARY_DIR}")
file(GLOB pb_hdrs ${pb_dir}/core/*.pb.h)
file(GLOB pb_srcs ${pb_dir}/core/*.pb.cc)
if(BUILD_UNO_LIB)
    file(GLOB uno_pb_hdrs ${pb_dir}/uno/*.pb.h)
    file(GLOB uno_pb_srcs ${pb_dir}/uno/*.pb.cc)
    list(APPEND pb_hdrs ${uno_pb_hdrs})
    list(APPEND pb_srcs ${uno_pb_srcs})
endif()
if(BUILD_UNO_LIB)
    file(GLOB texas_pb_hdrs ${pb_dir}/texas/*.pb.h)
    file(GLOB texas_pb_srcs ${pb_dir}/texas/*.pb.cc)
    list(APPEND pb_hdrs ${texas_pb_hdrs})
    list(APPEND pb_srcs ${texas_pb_srcs})
endif()

add_custom_command(
    OUTPUT ${pb_srcs} ${pb_hdrs}
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out=generate_mock_code=true:"${CMAKE_BINARY_DIR}"
    --cpp_out "${CMAKE_BINARY_DIR}"
    -I "${proto_dir}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    # here should be absolute path
    ${proto_files}
    DEPENDS "${proto_files}"# "${_PROTOBUF_PROTOC}" "${_GRPC_CPP_PLUGIN_EXECUTABLE}"
)
set_source_files_properties(${pb_srcs} PROPERTIES GENERATED TRUE)

add_library(uno_pb_lib ${pb_srcs})
target_include_directories(uno_pb_lib PUBLIC ${pb_dir})
target_link_libraries(uno_pb_lib ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

add_subdirectory(src)
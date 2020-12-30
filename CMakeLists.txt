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

# if this file is in top directory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BUILD_UNO_LIB ON)
endif()

# Proto file
set(core_proto_dir "${CMAKE_CURRENT_SOURCE_DIR}/proto/core")
set(uno_proto_dir "${CMAKE_CURRENT_SOURCE_DIR}/proto/uno")
set(proto_dir "${CMAKE_CURRENT_SOURCE_DIR}/proto")

# XXX: use file(glob *.pb)
set(proto_files 
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/core/core.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/core/error.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/core/provider.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/core/types.proto"
    "${CMAKE_CURRENT_SOURCE_DIR}/proto/uno/uno.proto"
)

# Generated sources
# TODO: conditional build
list(APPEND proto_srcs 
    "${CMAKE_BINARY_DIR}/core/core.pb.cc"
    "${CMAKE_BINARY_DIR}/core/error.pb.cc"
    "${CMAKE_BINARY_DIR}/core/provider.pb.cc"
    "${CMAKE_BINARY_DIR}/core/types.pb.cc"
    "${CMAKE_BINARY_DIR}/uno/uno.pb.cc"
)
list(APPEND proto_hdrs 
    "${CMAKE_BINARY_DIR}/core/core.pb.h"
    "${CMAKE_BINARY_DIR}/core/error.pb.h"
    "${CMAKE_BINARY_DIR}/core/provider.pb.h"
    "${CMAKE_BINARY_DIR}/core/types.pb.h"
    "${CMAKE_BINARY_DIR}/uno/uno.pb.h"
)
list(APPEND grpc_srcs 
    "${CMAKE_BINARY_DIR}/core/core.grpc.pb.cc"
    "${CMAKE_BINARY_DIR}/core/error.grpc.pb.cc"
    "${CMAKE_BINARY_DIR}/core/provider.grpc.pb.cc"
    "${CMAKE_BINARY_DIR}/core/types.grpc.pb.cc"
    "${CMAKE_BINARY_DIR}/uno/uno.grpc.pb.cc"
)
list(APPEND grpc_hdrs 
    "${CMAKE_BINARY_DIR}/core/core.grpc.pb.h"
    "${CMAKE_BINARY_DIR}/core/error.grpc.pb.h"
    "${CMAKE_BINARY_DIR}/core/provider.grpc.pb.h"
    "${CMAKE_BINARY_DIR}/core/types.grpc.pb.h"
    "${CMAKE_BINARY_DIR}/uno/uno.grpc.pb.h"
)

add_custom_command(
    OUTPUT ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs}
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out=generate_mock_code=true:"${CMAKE_BINARY_DIR}"
    --cpp_out "${CMAKE_BINARY_DIR}"
    -I "${proto_dir}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    # here should be absolute path
    ${proto_files}
    DEPENDS "${proto_files}"# "${_PROTOBUF_PROTOC}" "${_GRPC_CPP_PLUGIN_EXECUTABLE}"
)
set_source_files_properties(${proto_srcs} ${grpc_srcs} PROPERTIES GENERATED TRUE)

add_library(uno_proto_lib ${proto_srcs} ${grpc_srcs})
target_link_libraries(uno_proto_lib ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_include_directories(uno_proto_lib PUBLIC "${CMAKE_BINARY_DIR}")

add_subdirectory(src)
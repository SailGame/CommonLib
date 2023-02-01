# enable x.y version format
cmake_policy(SET CMP0048 NEW)
project(CommonLib VERSION 0.1)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(cmake/utils.cmake)
include(cmake/grpc.cmake)

option(IMPORT_GRPC_BY_SUBMODULE OFF)
option(IMPORT_GRPC_BY_FETCHCONTENT OFF)

# Proto files
set(proto_dir "${CMAKE_CURRENT_SOURCE_DIR}/proto")
file(GLOB proto_files ${proto_dir}/**/*.proto)

# Generated pb files
set(pb_dir "${CMAKE_BINARY_DIR}/sailgame_pb")
file(MAKE_DIRECTORY "${pb_dir}")
add_pb_files("${proto_files}")

import_grpc()
add_custom_command(
    OUTPUT ${pb_srcs} ${pb_hdrs}
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out=generate_mock_code=true:"${pb_dir}"
    --cpp_out "${pb_dir}"
    -I "${proto_dir}"
    -I "${_PROTOBUF_INCLUDE}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    # here should be absolute path
    ${proto_files}
    DEPENDS "${proto_files}"# "${_PROTOBUF_PROTOC}" "${_GRPC_CPP_PLUGIN_EXECUTABLE}"
)
set_source_files_properties(${pb_srcs} PROPERTIES GENERATED TRUE)

add_library(pb_lib ${pb_srcs})
# use "#include <sailgame_pb/core/core.pb.h>"
target_include_directories(pb_lib PUBLIC ${CMAKE_BINARY_DIR})
target_include_directories(pb_lib PUBLIC ${pb_dir})
target_link_libraries(pb_lib PUBLIC 
    ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

add_subdirectory(src)
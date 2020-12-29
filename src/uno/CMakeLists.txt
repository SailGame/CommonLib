aux_source_directory(. UNO_SRCS)

add_library(uno_lib ${UNO_SRCS})
target_link_libraries(uno_lib INTERFACE uno_proto_lib)

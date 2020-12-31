aux_source_directory(. UNO_SRCS)

add_library(uno_lib ${UNO_SRCS})
target_include_directories(uno_lib PUBLIC ../../include)
target_link_libraries(uno_lib PUBLIC pb_lib)

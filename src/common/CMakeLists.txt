aux_source_directory(. COMMON_SRCS)

add_library(common_lib ${COMMON_SRCS})
target_link_libraries(common_lib PUBLIC pb_lib)
target_include_directories(common_lib PUBLIC ../../include)

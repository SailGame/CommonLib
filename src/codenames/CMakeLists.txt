aux_source_directory(. CODENAMES_SRCS)

add_library(codenames_lib ${CODENAMES_SRCS})
target_include_directories(codenames_lib PUBLIC ../../include)
target_link_libraries(codenames_lib PUBLIC pb_lib)

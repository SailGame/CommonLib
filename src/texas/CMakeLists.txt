aux_source_directory(. TEXAS_SRCS)

add_library(texas_lib ${TEXAS_SRCS})
target_link_libraries(texas_lib PUBLIC common_lib)

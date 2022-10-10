aux_source_directory(. DIR_LIB_SRCS)
FILE(GLOB DIR_LIB_SRCS "*.cpp" "*.h")
add_library(materials INTERFACE)

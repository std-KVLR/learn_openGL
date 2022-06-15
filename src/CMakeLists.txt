set(HEADER_LIST "${OpenGLtest_SOURCE_DIR}/include/glad.h")

add_library(glad glad.c ${HEADER_LIST})

target_include_directories(glad PUBLIC ../include)

target_compile_features(glad PUBLIC cxx_std_17)


source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})
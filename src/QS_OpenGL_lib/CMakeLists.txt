set(HEADER_LIST "${OpenGLtest_SOURCE_DIR}/include/QS_OpenGL_lib/qs_ogl_lib.h")


add_library(qsOGL qs_ogl_lib.cpp ${HEADER_LIST})

target_link_libraries(qsOGL glad glfw OpenGL::GL ${CMAKE_DL_LIBS})

target_include_directories(qsOGL PUBLIC ../../include)

target_compile_features(qsOGL PUBLIC cxx_std_17)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}) 

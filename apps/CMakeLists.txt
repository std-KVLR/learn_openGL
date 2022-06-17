add_executable(app app.cpp)
target_link_libraries(app PRIVATE glad qsOGL glfw OpenGL::GL ${CMAKE_DL_LIBS})
target_compile_features(app PRIVATE cxx_std_17)

 

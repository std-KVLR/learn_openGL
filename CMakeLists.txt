cmake_minimum_required(VERSION 3.8)

project(
    OpenGLtest
    VERSION 0.1
    LANGUAGES C CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx

  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app

  # Note this needs to be done in the main CMakeLists

  # since it calls enable_testing, which must be in the

  # main CMakeLists.

  include(CTest)

  # Docs only available if this is the main app

  find_package(Doxygen)

  if(Doxygen_FOUND)

    add_subdirectory(docs)

  else()

    message(STATUS "Doxygen not found, not building docs")

  endif()

endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_subdirectory(src)
add_subdirectory(apps)










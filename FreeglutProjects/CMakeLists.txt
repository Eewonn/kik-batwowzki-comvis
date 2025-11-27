cmake_minimum_required(VERSION 3.10)
project(FreeglutExample)

set(CMAKE_CXX_STANDARD 11)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLUT (FreeGLUT)
find_package(GLUT REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find OpenAL
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENAL REQUIRED openal)
pkg_check_modules(SNDFILE REQUIRED sndfile)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS} ${GLUT_LIBRARY_DIRS} ${OPENAL_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})
add_definitions(${OPENGL_DEFINITIONS} ${GLUT_DEFINITIONS})

# Multi-scene modular version
add_executable(FreeglutApp 
    src/main.cpp
    src/AudioManager.cpp
    src/DrawingUtils.cpp
    src/Scene1_BikeInGarage.cpp
    src/Scene2_GarageTakeoff.cpp
    src/Scene3_MeetingFriends.cpp
    src/Scene4_GroupBiking.cpp
    src/Scene5_FinalPose.cpp
)

target_link_libraries(FreeglutApp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENAL_LIBRARIES} ${SNDFILE_LIBRARIES})

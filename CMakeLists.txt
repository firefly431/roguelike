CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
PROJECT(Roguelike)
SUBDIRS(ext)
if (WIN32)
    set(OGL_LIBS glew32 glfw3 opengl32)
else()
    set(OGL_LIBS GLEW glfw GL)
endif()
INCLUDE_DIRECTORIES(${Roguelike_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Roguelike_SOURCE_DIR}/ext/include)
ADD_EXECUTABLE(test_ex src/main.cpp src/graphics/glfwwindow.cpp src/graphics/gltexture.cpp src/graphics/glsprite.cpp src/graphics/glrenderctx.cpp)
ADD_EXECUTABLE(test_maze src/generation/generation.cpp)
SET_PROPERTY(TARGET test_ex PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET test_maze PROPERTY CXX_STANDARD 14)
TARGET_LINK_LIBRARIES(test_ex ${OGL_LIBS})
TARGET_LINK_LIBRARIES(test_ex external_libs)
TARGET_COMPILE_OPTIONS(test_ex PRIVATE -Wall -Wold-style-cast)
TARGET_COMPILE_OPTIONS(test_maze PRIVATE -Wall -Wold-style-cast)

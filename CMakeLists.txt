cmake_minimum_required(VERSION 3.23)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(src libs/glad/include libs/glfw/include libs/stb/include libs/glm/include)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp libs/glad/src/glad.c libs/stb/src/stb_image.c main.cpp)
add_executable(LearnOpenGL ${SOURCE_FILES})


message("CMAKE_SYSTEM_NAME = " "${CMAKE_SYSTEM_NAME}")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	find_library(GLFW_LIB glfw3 "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-mingw-w64")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(GLFW_LIB glfw3 "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/lib-macos-universal")
endif()

if (NOT GLFW_LIB)
	message(FATAL_ERROR "glfw3 lib hasn't been found!")
endif()

target_link_libraries(LearnOpenGL ${GLFW_LIB})


if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	find_library(COCOA_FRAMEWORK Cocoa)
	if (NOT COCOA_FRAMEWORK)
		message(FATAL_ERROR "Cocoa framework hasn't been found!")
	endif ()

	find_library(OPENGL_FRAMEWORK OpenGL)
	if (NOT OPENGL_FRAMEWORK)
		message(FATAL_ERROR "OpenGL framework hasn't been found!")
	endif ()

	find_library(IOKIT_FRAMEWORK IOKit)
	if (NOT IOKIT_FRAMEWORK)
		message(FATAL_ERROR "IOKit framework hasn't been found!")
	endif ()

	target_link_libraries(LearnOpenGL ${COCOA_FRAMEWORK} ${OPENGL_FRAMEWORK} ${IOKIT_FRAMEWORK})
endif ()


if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(OS_WINDOWS=1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	add_compile_definitions(OS_MACOS=1)
endif()

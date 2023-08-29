#include "Utilities.hpp"

#include <glad/glad.h>


void debugHandleGLErrors()
{
	for (GLenum errCode = glGetError(); errCode != GL_NO_ERROR; errCode = glGetError()) {
		std::string errStr;
		switch (errCode) {
			case GL_INVALID_ENUM:
				errStr = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				errStr = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				errStr = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				errStr = "GL_OUT_OF_MEMORY";
				break;
			default:
				break;
		}
		std::cerr << "OpenGL error: 0x" << std::hex << errCode << " (" << errStr << ")" << std::endl;
	}
}

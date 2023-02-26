#include "Shader.hpp"

#include "Utilities.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// glad.h must be included before any header files that require OpenGL (like GLFW).
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>


static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGHT = 728;
static constexpr auto WINDOW_TITLE = "LearnOpenGL";

static GLuint _vertexArrayBufferId = 0;
static GLuint _wallTextureId = 0;
static GLuint _faceTextureId = 0;
static size_t _verticesAmount = 0;
static size_t _indicesAmount = 0;
static std::unique_ptr<Shader> _shader;


void onGlfwError(int error, const char* description)
{
	std::cerr
			<< "GLFW Error"
			<< "\tCode: " << std::to_string(error) << "\n"
			<< "\tDescription: " << description << "\n";
}


void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}


void loadShaderProgram()
{
	_shader = std::make_unique<Shader>("../assets/shaders/default.vsh", "../assets/shaders/default.fsh");
	if (!_shader->isValid()) {
		_shader.reset();
	}
}


/**
 * @brief Uploads a texture with a given file name to the GL internal memory.
 * @param textureFilename
 * @param internalFormat A format of pixels inside GL after they are loaded to GPU.
 * @param pixelDataFormat A format of pixels in the texture file.
 * @return GL texture id, which is more than 0, if it's a success; and which equals 0, if it's a failure.
 */
GLuint loadTexture(const std::string_view textureFilename, const GLint internalFormat, const GLenum pixelDataFormat)
{
	GLuint textureId = 0;

	// Load texture data from file.
	stbi_set_flip_vertically_on_load(true);
	int textureWidth = 0;
	int textureHeight = 0;
	int textureChannelsNum = 0;
	unsigned char* textureData = stbi_load(textureFilename.data(), &textureWidth, &textureHeight, &textureChannelsNum, 0);
	if (textureData == nullptr) {
		std::cerr << "stbi error! " << stbi_failure_reason() << ": " << textureFilename << std::endl;
		return textureId;
	}

	// Generate and bind texture.
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set wrapping.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Set filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture data to GPU.
	const GLint lodLevel = 0; 						// lod - level of detail. Also, a mipmap level.
	const GLenum pixelDataType = GL_UNSIGNED_BYTE;	// type of textureData elements.
	glTexImage2D(GL_TEXTURE_2D, lodLevel, internalFormat, textureWidth, textureHeight, 0,
				 pixelDataFormat, pixelDataType, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Free texture data.
	stbi_image_free(textureData);

	// Reset bound texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	std::cout << "Texture is loaded: " << textureFilename << std::endl;

	return textureId;
}


void doOnce()
{
	loadShaderProgram();

	_wallTextureId = loadTexture("../assets/textures/wall.jpg", GL_RGB, GL_RGB);
	_faceTextureId = loadTexture("../assets/textures/awesomeface.png", GL_RGBA, GL_RGBA);

	// Create and bind vertex array object (VAO).
	// All the next calls of glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray will be bound to this vao.
	glGenVertexArrays(1, &_vertexArrayBufferId);
	glBindVertexArray(_vertexArrayBufferId);

	// Create and bind vertex buffer object (VBO).
	GLuint vboId = 0;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	// Fill vbo.
	struct VertexFormat {
		struct Pos {
			float x = 0.f;
			float y = 0.f;
			float z = 0.f;
		};
		struct Color {
			float r = 0.f;
			float g = 0.f;
			float b = 0.f;
			float a = 0.f;
		};
		struct TextureCoords {
			float u = 0.f;
			float v = 0.f;
		};
		Pos pos;
		Color color;
		TextureCoords texCoords;
	};
	const auto vertices = std::vector<VertexFormat> {
			VertexFormat{{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f, 1.f}, {0.f, 0.f}},	// Left bottom
			VertexFormat{{0.5f, -0.5f, 0.f}, {0.f, 1.f, 0.f, 1.f}, {1.f, 0.f}},		// Right bottom
			VertexFormat{{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 1.f, 1.f}, {0.f, 1.f}},		// Left top
			VertexFormat{{0.5f, 0.5f, 0.f}, {0.f, 1.f, 1.f, 1.f}, {1.f, 1.f}},		// Right top
	};
	_verticesAmount = vertices.size();

	const size_t bufferBytesSize = sizeof(vertices[0]) * _verticesAmount;
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) bufferBytesSize, vertices.data(), GL_STATIC_DRAW);

	// Create and fill Element Buffer Object (EBO).
	// It contains vertex indices (from VBO) which need to draw.
	GLuint elementsBuffer = 0;
	glGenBuffers(1, &elementsBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer);
	const auto indices = std::vector<unsigned int> {
			0, 1, 2,
			1, 2, 3,
	};
	_indicesAmount = indices.size();
	const size_t elementsBufferSize = _indicesAmount * sizeof(indices[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) elementsBufferSize, indices.data(), GL_STATIC_DRAW);

	// Configure vertex attributes.
	{
		// 0. in vec3 aPos
		const int attrLocation = 0;                     // The position of the attribute in the vertex shader.
		const int attrSize = 3;                         // The number of components per the vertex attribute. Must be 1, 2, 3, 4.
		const int attrStride = sizeof(VertexFormat);    // The next vertex attribute can be found after that amount of bytes.
		const int attrDataOffset = 0;					// The offset of the attribute in bytes in the buffer data array.
		glVertexAttribPointer(attrLocation, attrSize, GL_FLOAT, GL_FALSE, attrStride, (void*) attrDataOffset);
		glEnableVertexAttribArray(attrLocation);
	}
	{
		// 1. in vec4 aColor
		const int attrLocation = 1;
		const int attrSize = 4;
		const int attrStride = sizeof(VertexFormat);
		const int attrDataOffset = sizeof(VertexFormat::Pos);
		glVertexAttribPointer(attrLocation, attrSize, GL_FLOAT, GL_FALSE, attrStride, (void*) attrDataOffset);
		glEnableVertexAttribArray(attrLocation);
	}
	{
		// 2. in vec2 aTexCoords
		const int attrLocation = 2;
		const int attrSize = 2;
		const int attrStride = sizeof(VertexFormat);
		const int attrDataOffset = sizeof(VertexFormat::Pos) + sizeof(VertexFormat::Color);
		glVertexAttribPointer(attrLocation, attrSize, GL_FLOAT, GL_FALSE, attrStride, (void*) attrDataOffset);
		glEnableVertexAttribArray(attrLocation);
	}

	// Reset bound VAO.
	glBindVertexArray(0);
}


void doMainUpdate()
{
	// Clear the frame buffer.
	glClearColor(0.1f, 0.05f, 0.05f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	const GLint firstSamplerIndex = 0;
	glActiveTexture(GL_TEXTURE0 + firstSamplerIndex);
	glBindTexture(GL_TEXTURE_2D, _wallTextureId);

	const GLint secondSamplerIndex = 1;
	glActiveTexture(GL_TEXTURE0 + secondSamplerIndex);
	glBindTexture(GL_TEXTURE_2D, _faceTextureId);

	if (_shader) {
		_shader->bind();

		// Set uniform values after the shader binding.
		const double timeSec = glfwGetTime();
		const auto v = static_cast<float>(std::sin(timeSec));
		const float progress = v * 0.5f + 0.5f; // [0, 1]
		_shader->setUniform1f("uProgress", progress);

		_shader->setUniform1i("sampler0", firstSamplerIndex);
		_shader->setUniform1i("sampler1", secondSamplerIndex);
	}

	glBindVertexArray(_vertexArrayBufferId);

	// Set wireframe mode drawing.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Draw vertices directly from VBO connected to _vertexArrayBufferId.
	// glDrawArrays(GL_TRIANGLES, 0, (GLsizei) _verticesAmount);

	// Draw vertices from VBO whom indices appear in EBO connected to _vertexArrayBufferId.
	glDrawElements(GL_TRIANGLES, (GLsizei) _indicesAmount, GL_UNSIGNED_INT, nullptr);
}


int main()
{
	glfwSetErrorCallback(onGlfwError);

	if (glfwInit() != GLFW_TRUE) {
		std::cerr << "GLWF initialization failed." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if OS_MACOS
	// The initialisation doesn't work on the MacOS without such an instruction.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (!window) {
		std::cerr << "Window or OpenGL context creation failed." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	const bool loaded = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	if (!loaded) {
		std::cerr << "GLAD GLLoader initialization failed." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	const auto version = glGetString(GL_VERSION);
	std::cout << "OpenGL version: " << version << "\n";

	GLint maxVertexAttributes = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
	std::cout << "\tMax vertex attributes: " << maxVertexAttributes << std::endl;

	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	onFrameBufferSizeChanged(window, width, height);
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);

	doOnce();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		doMainUpdate();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

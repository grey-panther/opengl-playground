#include <vector>
#include <iostream>
#include <cmath>

// glad.h must be included before any header files that require OpenGL (like GLFW).
#include <glad/glad.h>
#include <GLFW/glfw3.h>


static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGHT = 728;
static constexpr auto WINDOW_TITLE = "LearnOpenGL";

static GLuint _shaderProgramId = 0;
static GLuint _vertexArrayBufferId = 0;
static size_t _verticesAmount = 0;
static size_t _indicesAmount = 0;


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
	// Load and compile vertex shader.
	const char* vertexShaderText =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec4 aColor;\n"
			"out vec4 vColor;\n"
			"void main() {\n"
			"    vColor = aColor;\n"
			"    gl_Position = vec4(aPos, 1.0);\n"
			"}";

	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
	glCompileShader(vertexShader);

	int vshStatus = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vshStatus);
	if (vshStatus == GL_FALSE) {
		std::string infoLog;
		infoLog.resize(512);
		glGetShaderInfoLog(vertexShader, (GLint) infoLog.size(), nullptr, infoLog.data());
		std::cerr << "Vertex shader compilation failed:\n\t" << infoLog << std::endl;
		return;
	}


	// Load and compile fragment shader.
	const char* fragmentShaderText =
			"#version 330 core\n"
			"in vec4 vColor;\n"
			"out vec4 FragColor;\n"
			"void main() {\n"
			"    FragColor = vColor;\n"
			"}";

	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
	glCompileShader(fragmentShader);

	int fshStatus = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fshStatus);
	if (fshStatus == GL_FALSE) {
		std::string infoLog;
		infoLog.resize(512);
		glGetShaderInfoLog(fragmentShader, (GLint) infoLog.size(), nullptr, infoLog.data());
		std::cerr << "Fragment shader compilation failed:\n\t" << infoLog << std::endl;
		return;
	}


	// Create and link shader program.
	const GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int programStatus = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programStatus);
	if (programStatus == GL_FALSE) {
		std::string infoLog;
		infoLog.resize(512);
		glGetProgramInfoLog(shaderProgram, (GLint) infoLog.size(), nullptr, infoLog.data());
		std::cerr << "Shader program link failed:\n\t" << infoLog << std::endl;
		return;
	}

	// When the shader program is linked, shaders are no longer needed.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	std::cout << "Shaders are successfully loaded." << std::endl;

	_shaderProgramId = shaderProgram;
}


void doOnce()
{
	loadShaderProgram();

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
		Pos pos;
		Color color;
	};
	const auto vertices = std::vector<VertexFormat> {
			VertexFormat{{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f, 1.f}},	// Left bottom
			VertexFormat{{0.5f, -0.5f, 0.f}, {0.f, 1.f, 0.f, 1.f}},		// Right bottom
			VertexFormat{{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 1.f, 1.f}},		// Left top
			VertexFormat{{0.5f, 0.5f, 0.f}, {0.f, 1.f, 1.f, 1.f}},		// Right top
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

	// Reset bound VAO.
	glBindVertexArray(0);
}


void doMainUpdate()
{
	// Clear the frame buffer.
	glClearColor(0.1f, 0.05f, 0.05f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_shaderProgramId);
	glBindVertexArray(_vertexArrayBufferId);

	// Set uniform values after the program binding.
	const GLint uColorLocation = glGetUniformLocation(_shaderProgramId, "uColor");
	if (uColorLocation >= 0) {
		const double timeSec = glfwGetTime();
		const auto v = static_cast<float>(std::sin(timeSec));
		const float blue = v * 0.5f + 0.5f;
		glUniform4f(uColorLocation, 0.99f, 0.43f, blue, 1.f);
	}

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

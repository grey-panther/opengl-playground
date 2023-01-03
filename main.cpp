#include <array>
#include <iostream>

// glad.h must be included before any header files that require OpenGL (like GLFW).
#include <glad/glad.h>
#include <GLFW/glfw3.h>


static constexpr int WINDOW_WIDTH = 1024;
static constexpr int WINDOW_HEIGHT = 728;
static constexpr auto WINDOW_TITLE = "LearnOpenGL";


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


void doMainUpdate()
{
	// Clear the frame buffer.
	glClearColor(0.1f, 0.05f, 0.05f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create vertex buffer object (vbo).
	GLuint vboId = 0;
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	// Fill vbo.
	const auto coords = std::array<float, 9> {
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.f, 0.5f, 0.f
	};
	const size_t bufferBytesSize = sizeof(coords[0]) * coords.size();
	glBufferData(GL_ARRAY_BUFFER, bufferBytesSize, coords.data(), GL_STATIC_DRAW);
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

	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	onFrameBufferSizeChanged(window, width, height);
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);

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

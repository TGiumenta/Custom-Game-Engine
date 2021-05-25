#include "pch.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

int WINAPI WinMain(_In_ HINSTANCE /*instance*/, _In_opt_ HINSTANCE /*previousInstance*/, _In_ LPSTR /*commandLine*/, _In_ int /*showCommand*/)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	const int width = 800;
	const int height = 600;
	const char* title = "OpenGL Essentials";
	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() != 0)
	{
		return EXIT_FAILURE;
	}

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
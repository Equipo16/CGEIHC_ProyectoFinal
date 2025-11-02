//Práctica 2
//Sandra Laparra Miranda
//Fecha de entrega: 24 de agosto de 2025
//Número de cuenta: 311243563

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 Sandra Laparra", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci n de errores de inicializaci n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");
	
	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
			
			//y = 0.4f 
			-0.45f, 0.4f, 0.0f,     0.0f, 0.0f, 0.0f, //negro

			-0.42f, 0.4f, 0.0f,     0.545f, 0.271f, 0.075f, //cafe
	
			-0.39f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,//azul claro
			-0.36f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.33f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.30f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.27f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.24f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.21f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			-0.18f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,

			-0.15f, 0.4f, 0.0f,     0.545f, 0.271f, 0.075f,
			-0.12f, 0.4f, 0.0f,     0.545f, 0.271f, 0.075f,
			-0.09f, 0.4f, 0.0f,     0.545f, 0.271f, 0.075f,

			-0.06f, 0.4f, 0.0f,     0.0f, 0.0f, 0.0f,

			-0.03f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			 0.00f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			 0.03f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,

			 0.06f, 0.4f, 0.0f,     0.0f, 0.0f, 0.0f,

			 0.09f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,
			 0.12f, 0.4f, 0.0f,     0.2f, 0.6f, 1.0f,

			 0.15f, 0.4f, 0.0f,     0.0f, 0.0f, 0.0f,

			 //y = 0.36f

			-0.45f, 0.36f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.42f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.39f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.36f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.33f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.30f, 0.36f, 0.0f,    1.0f, 1.0f, 1.0f, //blanco

			-0.27f, 0.36f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.24f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.21f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.18f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.15f, 0.36f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.12f, 0.36f, 0.0f,    0.545f, 0.271f, 0.075f,
			-0.09f, 0.36f, 0.0f,    0.545f, 0.271f, 0.075f,
			-0.06f, 0.36f, 0.0f,    0.545f, 0.271f, 0.075f,

			-0.03f, 0.36f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.00f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,
			 0.03f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,

			 0.06f, 0.36f, 0.0f,    0.0f, 0.0f, 0.0f,

			 0.09f, 0.36f, 0.0f,    0.2f, 0.6f, 1.0f,

			 0.12f, 0.36f, 0.0f,    0.0f, 0.0f, 0.0f,

			 //y = 0.32f

			 -0.45f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,

			 -0.42f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f, 
			 -0.39f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.36f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.33f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,

			 -0.30f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,

			 -0.27f, 0.32f, 0.0f,   0.545f, 0.271f, 0.075f,

			 -0.24f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.21f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.18f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,

			 -0.15f, 0.32f, 0.0f,   1.0f, 1.0f, 1.0f,

			 -0.12f, 0.32f, 0.0f,   0.545f, 0.271f, 0.075f,
			 -0.09f, 0.32f, 0.0f,   0.545f, 0.271f, 0.075f,
			 -0.06f, 0.32f, 0.0f,   0.545f, 0.271f, 0.075f,

			 -0.03f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,

			  0.00f, 0.32f, 0.0f,   0.2f, 0.6f, 1.0f,

			  0.03f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,
			  0.06f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,
			  0.09f, 0.32f, 0.0f,   0.0f, 0.0f, 0.0f,

			  //y = 0.28f

			 -0.42f, 0.28f, 0.0f,   0.0f, 0.0f, 0.0f,

			 -0.39f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.36f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.33f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,

			 -0.30f, 0.28f, 0.0f,   0.0f, 0.0f, 0.0f,

			 -0.27f, 0.28f, 0.0f,   0.545f, 0.271f, 0.075f,

			 -0.24f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.21f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,
			 -0.18f, 0.28f, 0.0f,   0.2f, 0.6f, 1.0f,

			 -0.15f, 0.28f, 0.0f,   0.0f, 0.0f, 0.0f,

			 -0.12f, 0.28f, 0.0f,   1.0f, 1.0f, 1.0f,

			 -0.09f, 0.28f, 0.0f,   0.545f, 0.271f, 0.075f,
			 -0.06f, 0.28f, 0.0f,   0.545f, 0.271f, 0.075f,
			 -0.03f, 0.28f, 0.0f,   0.545f, 0.271f, 0.075f,

			 0.00f, 0.28f, 0.0f,    0.0f, 0.0f, 0.0f,
			 0.03f, 0.28f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.24f

			-0.39f, 0.24f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.36f, 0.24f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.33f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.30f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.27f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.24f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,

		    -0.21f, 0.24f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.18f, 0.24f, 0.0f,    0.0f, 0.0f, 0.0f,

		    -0.15f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,
		    -0.12f, 0.24f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.09f, 0.24f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.06f, 0.24f, 0.0f,    0.545f, 0.271f, 0.075f,
			-0.03f, 0.24f, 0.0f,    0.545f, 0.271f, 0.075f,

			-0.00f, 0.24f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.20f

			-0.39f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.36f, 0.20f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.33f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.30f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.27f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.24f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.21f, 0.20f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.18f, 0.20f, 0.0f,    0.2f, 0.6f, 1.0f,
        	-0.15f, 0.20f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.12f, 0.20f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.09f, 0.20f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.06f, 0.20f, 0.0f,    0.545f, 0.271f, 0.075f,
			-0.03f, 0.20f, 0.0f,    0.545f, 0.271f, 0.075f,

			-0.00f, 0.20f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.16f

			-0.36f, 0.16f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.33f, 0.16f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.30f, 0.16f, 0.0f,    1.0f, 1.0f, 0.0f, //amarillo
			-0.27f, 0.16f, 0.0f,    1.0f, 1.0f, 0.0f,

			-0.24f, 0.16f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.21f, 0.16f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.18f, 0.16f, 0.0f,    0.2f, 0.6f, 1.0f,
		    -0.15f, 0.16f, 0.0f,    0.2f, 0.6f, 1.0f,

		    -0.12f, 0.16f, 0.0f,    0.0f, 0.0f, 0.0f,

		    -0.09f, 0.16f, 0.0f,    1.0f, 1.0f, 1.0f,

		    -0.06f, 0.16f, 0.0f,    0.545f, 0.271f, 0.075f,
		    -0.03f, 0.16f, 0.0f,    0.545f, 0.271f, 0.075f,

			 0.00f, 0.16f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.12f

			-0.30f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.27f, 0.12f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.24f, 0.12f, 0.0f,    1.0f, 1.0f, 0.0f,

			-0.21f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.18f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.15f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.12f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.09f, 0.12f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.06f, 0.12f, 0.0f,    0.545f, 0.271f, 0.075f,
			-0.03f, 0.12f, 0.0f,    0.545f, 0.271f, 0.075f,

			 0.00f, 0.12f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.08f

			-0.33f, 0.08f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.30f, 0.08f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.27f, 0.08f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.24f, 0.08f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.21f, 0.08f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.18f, 0.08f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.15f, 0.08f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.12f, 0.08f, 0.0f,    1.0f, 1.0f, 0.0f,

			-0.09f, 0.08f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.06f, 0.08f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.03f, 0.08f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.04f

			-0.30f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.27f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.24f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.21f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.18f, 0.04f, 0.0f,    1.0f, 1.0f, 0.0f,
			-0.15f, 0.04f, 0.0f,    1.0f, 1.0f, 0.0f,

			-0.12f, 0.04f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.09f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.06f, 0.04f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.03f, 0.04f, 0.0f,    0.0f, 0.0f, 0.0f,

			//y = 0.0f

			-0.21f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
			-0.18f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
			-0.15f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,

			-0.12f, 0.0f, 0.0f,     0.2f, 0.6f, 1.0f,

			-0.09f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
			-0.06f, 0.0f, 0.0f,     0.0f, 0.0f, 0.0f,

			//y = -0.04f

			-0.18f, -0.04f, 0.0f,   0.0f, 0.0f, 0.0f,

			-0.15f, -0.04f, 0.0f,   0.2f, 0.6f, 1.0f,
			-0.12f, -0.04f, 0.0f,   0.2f, 0.6f, 1.0f,
			-0.09f, -0.04f, 0.0f,   0.2f, 0.6f, 1.0f,

			-0.06f, -0.04f, 0.0f,   0.0f, 0.0f, 0.0f,

			//y = -0.08

			-0.15f, -0.08f, 0.0f,   0.0f, 0.0f, 0.0f,
			-0.12f, -0.08f, 0.0f,   0.0f, 0.0f, 0.0f,
			-0.09f, -0.08f, 0.0f,   0.0f, 0.0f, 0.0f,

			//y = 0.44

			-0.42f, 0.44f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.39f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.36f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.33f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.30f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.27f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.24f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.21f, 0.44f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.18f, 0.44f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.15f, 0.44f, 0.0f,    0.545f, 0.271f, 0.075f,

			-0.12f, 0.44f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.09f, 0.44f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.06f, 0.44f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.03f, 0.44f, 0.0f,    0.0f, 0.0f, 0.0f,

			0.00f, 0.44f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.03f, 0.44f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.06f, 0.44f, 0.0f,     0.2f, 0.6f, 1.0f,

			0.09f, 0.44f, 0.0f,     0.0f, 0.0f, 0.0f,

			0.12f, 0.44f, 0.0f,     0.2f, 0.6f, 1.0f,

			0.15f, 0.44f, 0.0f,     0.0f, 0.0f, 0.0f,

			//y = 0.48

			-0.42f, 0.48f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.39f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.36f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.33f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.30f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.27f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.24f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.21f, 0.48f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.18f, 0.48f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.15f, 0.48f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.12f, 0.48f, 0.0f,    1.0f, 1.0f, 1.0f,
			-0.09f, 0.48f, 0.0f,    1.0f, 1.0f, 1.0f,
			-0.06f, 0.48f, 0.0f,    1.0f, 1.0f, 1.0f,

			-0.03f, 0.48f, 0.0f,    0.0f, 0.0f, 0.0f,

			0.00f, 0.48f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.03f, 0.48f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.06f, 0.48f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.09f, 0.48f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.12f, 0.48f, 0.0f,     0.2f, 0.6f, 1.0f,

			0.15f, 0.48f, 0.0f,     0.0f, 0.0f, 0.0f,

			//y = 0.52

			-0.39f, 0.52f, 0.0f,    0.0f, 0.0f, 0.0f,

			-0.36f, 0.52f, 0.0f,    0.2f, 0.6f, 1.0f, 
			-0.33f, 0.52f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.30f, 0.52f, 0.0f,    0.2f, 0.6f, 1.0f,
			-0.27f, 0.52f, 0.0f,    0.2f, 0.6f, 1.0f,

			-0.24f, 0.52f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.21f, 0.52f, 0.0f,    0.0f, 0.0f, 0.0f,

			0.00f, 0.52f, 0.0f,     0.0f, 0.0f, 0.0f,

			0.03f, 0.52f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.06f, 0.52f, 0.0f,     0.2f, 0.6f, 1.0f,
			0.09f, 0.52f, 0.0f,     0.2f, 0.6f, 1.0f,

			0.12f, 0.52f, 0.0f,     0.0f, 0.0f, 0.0f,

			//y = 0.56

			-0.36f, 0.56f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.33f, 0.56f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.30f, 0.56f, 0.0f,    0.0f, 0.0f, 0.0f,
			-0.27f, 0.56f, 0.0f,    0.0f, 0.0f, 0.0f,

			0.03f, 0.56f, 0.0f,     0.0f, 0.0f, 0.0f,
			0.06f, 0.56f, 0.0f,     0.0f, 0.0f, 0.0f,
			0.09f, 0.56f, 0.0f,     0.0f, 0.0f, 0.0f,
			
			 
	};
	int totalVertices = sizeof(vertices) / (6 * sizeof(float)); //Calcula automáticamente cuántos vértices hay en el arreglo
	
	unsigned int indices[] = {  // note that we start from 0!
		3,2,1,// second Triangle
		0,1,3,
		
	};



	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //Para cambiar el fondo de la ventana a color blanco a negro usar (0.0f, 0.0f, 0.0f, 1.0f)
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


		glPointSize(12); // tamaño del punto 12 pixeles igual a 0.03 unidades en "X" y 0.04 unidades en "Y"
		glDrawArrays(GL_POINTS, 0, totalVertices);
        
        //glDrawArrays(GL_LINES,0,2);
        //glDrawArrays(GL_LINE_LOOP,0,4);
        
        //glDrawArrays(GL_TRIANGLES,0,3);
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}
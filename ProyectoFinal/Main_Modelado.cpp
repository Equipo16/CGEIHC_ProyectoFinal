//Práctica 4
//Sandra Laparra Miranda
//Fecha de entrega : 7 de septiembre de 2025
//Número de cuenta : 311243563
#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Sandra Laparra", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

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
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers



	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,

		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		-0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		// Para ponerle color al gato
		GLint indiceColor = glGetUniformLocation(ourShader.Program, "objetoColor");

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);


		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(VAO);

		// Transformación para centrar al gato más bajo
		glm::mat4 base = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// ============================
		// Cabeza
		// ============================

		// Cabeza principal
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.6f, 0.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Orejas
		model = base;
		model = glm::translate(model, glm::vec3(-0.4f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.6f, 0.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = base;
		model = glm::translate(model, glm::vec3(0.4f, 0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.6f, 0.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojos y pupilas
		model = base;
		model = glm::translate(model, glm::vec3(-0.2f, 0.1f, 0.51f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = base;
		model = glm::translate(model, glm::vec3(-0.2f, 0.1f, 0.56f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = base;
		model = glm::translate(model, glm::vec3(0.2f, 0.1f, 0.51f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = base;
		model = glm::translate(model, glm::vec3(0.2f, 0.1f, 0.56f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mejillas
		model = base;
		model = glm::translate(model, glm::vec3(-0.4f, -0.1f, 0.51f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = base;
		model = glm::translate(model, glm::vec3(0.4f, -0.1f, 0.51f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Nariz
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.55f));
		model = glm::scale(model, glm::vec3(0.15f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.5f, 0.5f, 0.5f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Boca
		for (int i = 0; i < 3; i++) {
			model = base;
			model = glm::translate(model, glm::vec3(-0.1f - 0.05f * i, -0.25f + 0.05f * i, 0.55f));
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(indiceColor, 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (int i = 0; i < 3; i++) {
			model = base;
			model = glm::translate(model, glm::vec3(0.1f + 0.05f * i, -0.25f + 0.05f * i, 0.55f));
			model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3f(indiceColor, 0.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ============================
		// Cuello 
		// ============================

		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, 0.0f));  // más abajo
		model = glm::scale(model, glm::vec3(0.4f, 0.8f, 0.4f));       // delgado y alargado
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.7f, 0.4f);  // un poco más claro
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============================
		// Cuerpo
		// ============================

		// Parte inferior (cuerpo principal)
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));  // antes -1.6f
		model = glm::scale(model, glm::vec3(1.0f, 1.6f, 0.8f));       // altura moderada
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.6f, 0.2f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pecho claro
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -1.3f, 0.41f));
		model = glm::scale(model, glm::vec3(0.4f, 0.9f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Franja clara horizontal
		model = base;
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 0.41f));
		model = glm::scale(model, glm::vec3(1.0f, 0.25f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============================
		// Patas delanteras
		// ============================

		// Pata izquierda
		model = base;
		model = glm::translate(model, glm::vec3(-0.3f, -2.12f, 0.25f));
		model = glm::scale(model, glm::vec3(0.45f, 0.35f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.82f, 0.71f, 0.55f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pata derecha
		model = base;
		model = glm::translate(model, glm::vec3(0.3f, -2.12f, 0.25f));
		model = glm::scale(model, glm::vec3(0.45f, 0.35f, 0.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.82f, 0.71f, 0.55f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============================
		// Cubos laterales que forman la pata trasera
		// ============================
		
		// base
		model = base;
		model = glm::translate(model, glm::vec3(-0.725f, -2.13, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.35f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// primer cubo
		model = base;
		model = glm::translate(model, glm::vec3(-0.725f, -1.65, -0.15f));
		model = glm::scale(model, glm::vec3(0.45f, 0.59f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.82f, 0.71f, 0.55f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// segundo cubo
		model = base;
		model = glm::translate(model, glm::vec3(-0.63f, -1.24f, -0.15f));
		model = glm::scale(model, glm::vec3(0.25f, 0.23f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 1.0f, 0.9f, 0.7f);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// tercer cubo
		model = base;
		model = glm::translate(model, glm::vec3(-0.61f, -1.01f, -0.15f));
		model = glm::scale(model, glm::vec3(0.15f, 0.23f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(indiceColor, 0.82f, 0.71f, 0.55f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ============================
		// Cola
		// ============================

		{
			const glm::vec3 colorNaranja = glm::vec3(1.0f, 0.6f, 0.2f);
			const glm::vec3 colorCrema = glm::vec3(1.0f, 0.9f, 0.7f);

			const int   tramosAtras = 2;   // cubos hacia atrás
			const int   tramosArriba = 5;   // cubos hacia arriba 
			const float tam = 0.35f;	// tamaño de cada cubo
			const float pasoY = 0.30f; // cuánto sube cada cubo en Y
			const float pasoZ = 0.15f; // desplazamiento hacia atrás en Z
			const float desviacionX = -0.45f; // inclinación hacia la izquierda

			glm::mat4 acumulado = base; // a partir de esta matriz se irán acumulando los movimientos de cada cubo que forman la cola
			acumulado = glm::translate(acumulado, glm::vec3(0.0f, -1.8f, -0.6f));

			int index = 0;

			// Parte hacia atrás 
			for (int i = 0; i < tramosAtras; ++i) {
				model = acumulado;
				model = glm::scale(model, glm::vec3(tam, tam, tam));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				if (index % 2 == 0) 
					glUniform3f(indiceColor, colorNaranja.r, colorNaranja.g, colorNaranja.b);
				else                
					glUniform3f(indiceColor, colorCrema.r, colorCrema.g, colorCrema.b);

				glDrawArrays(GL_TRIANGLES, 0, 36);

				acumulado = glm::translate(acumulado, glm::vec3(0.0f, 0.0f, -pasoZ)); // hacia atrás
				index++;
			}

			// Parte hacia arriba con giro a la izquierda
			for (int i = 0; i < tramosArriba; ++i) {
				model = acumulado;
				model = glm::scale(model, glm::vec3(tam, tam, tam));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				if (index % 2 == 0) 
					glUniform3f(indiceColor, colorNaranja.r, colorNaranja.g, colorNaranja.b);
				else                
					glUniform3f(indiceColor, colorCrema.r, colorCrema.g, colorCrema.b);

				glDrawArrays(GL_TRIANGLES, 0, 36);

				// Subir + giro a la izquierda
				acumulado = glm::translate(acumulado, glm::vec3(desviacionX, pasoY, 0.0f));
				index++;
			}
		}


		glBindVertexArray(0);


		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
}



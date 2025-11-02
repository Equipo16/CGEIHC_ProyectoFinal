//Practica 5
//Sandra Laparra Miranda
//Fecha de entrega: 14 de septiembre de 2025
//Número de cuenta: 311243563

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f, //nos permite generar la articulación
codo = 0.0f,
muneca = 0.0f,
dedo1 = 0.0f,
dedo2 = 0.0f,
dedo3 = 0.0f,
dedo4 = 0.0f,
dedo5 = 0.0f,
dedo6 = 0.0f,
dedo7 = 0.0f,
dedo8 = 0.0f,
dedo9 = 0.0f,
dedo10 = 0.0f;


int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 5 Sandra Laparra", nullptr, nullptr);

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
	// use with Orthographic Projection


	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
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
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f)); //matriz auxiliar
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //RGB
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model Antebrazo
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model= glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); //RGB
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B

		//Model Palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f); //RGB
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C

		//Dedos de la parte de arriba de la palma

		//Model Dedo 1 - Falange A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo1), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //RGB
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//D

		//Model Dedo 1 - Falange B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); //RGB
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//E

		//Model Dedo 2 - Falange A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.4f));
		model = glm::rotate(model, glm::radians(dedo3), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 2 - Falange B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo4), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.5f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 3 - Falange A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.4f));
		model = glm::rotate(model, glm::radians(dedo5), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 3 - Falange B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo6), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.5f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Dedos de la parte de abajo de la palma

		//Model Dedo 4 - Falange A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.35f, -0.2f));
		model = glm::rotate(model, glm::radians(dedo7), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.8f, 0.4f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 4 - Falange B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo8), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.9f, 0.6f, 0.2f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 5 - Falange A
		model = glm::translate(modelTemp2, glm::vec3(0.25f, -0.35f, 0.2f));
		model = glm::rotate(model, glm::radians(dedo9), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.2f, 0.8f, 0.4f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Model Dedo 5 - Falange B
		model = glm::translate(modelTemp, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo10), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.3f, 0.25f));
		color = glm::vec3(0.4f, 0.9f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 //Brazo robotico 
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.01f;
	 if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
		 movY += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		 movY -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && rot < 90.0f)
		 rot += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && rot > 0.0f)
		 rot -= 0.01f;
	 //Hombro
	 if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && hombro < 180.0f)
		 hombro += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && hombro > 0.0f)
		 hombro -= 0.01f;
	 //Codo
	 if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && codo < 135.0f)
		 codo += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && codo > 0.0f)
		 codo -= 0.01f;
	 //Palma
	 if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && muneca < 90.0f)
		 muneca += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && muneca > -90.0f)
		 muneca -= 0.01f;
	//Dedo 1
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo1 < 90.0f)
		 dedo1 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo1 > 0.0f)
		 dedo1 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && dedo2 > -10.0f)
		 dedo2 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && dedo2 < 0.0f)
		 dedo2 += 0.01f;
	 //Dedo 2
	 if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && dedo3 < 90.0f)
		 dedo3 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && dedo3 > 0.0f)
		 dedo3 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && dedo4 > -10.0f)
		 dedo4 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && dedo4 < 0.0f)
		 dedo4 += 0.01f;
	 // Dedo 3
	 if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && dedo5 < 90.0f)
		 dedo5 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && dedo5 > 0.0f)
		 dedo5 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && dedo6 > -10.0f)
		 dedo6 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && dedo6 < 0.0f)
		 dedo6 += 0.01f;
	 // Dedo 4
	 if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && dedo7 < 90.0f)
		 dedo7 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && dedo7 > 0.0f)
		 dedo7 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && dedo8 < 10.0f)
		 dedo8 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && dedo8 > 0.0f)
		 dedo8 -= 0.01f;
	 // Dedo 5
	 if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && dedo9 < 90.0f)
		 dedo9 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && dedo9 > 0.0f)
		 dedo9 -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && dedo10 < 10.0f)
		 dedo10 += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && dedo10 > 0.0f)
		 dedo10 -= 0.01f;


	//Cerrar la palma 
	 if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		 //Dedos de arriba cierran hacia abajo
		 if (dedo1 > -5.0f)  dedo1 -= 0.01f;   
		 if (dedo2 > -10.0f)  dedo2 -= 0.01f;   

		 if (dedo3 > -5.0f)  dedo3 -= 0.01f;
		 if (dedo4 > -10.0f)  dedo4 -= 0.01f;

		 if (dedo5 > -5.0f)  dedo5 -= 0.01f;
		 if (dedo6 > -10.0f)  dedo6 -= 0.01f;

		 //Dedos de abajo cierran hacia arriba
		 if (dedo7 < 5.0f)   dedo7 += 0.01f;
		 if (dedo8 < 10.0f)   dedo8 += 0.01f;

		 if (dedo9 < 5.0f)   dedo9 += 0.01f;
		 if (dedo10 < 10.0f)  dedo10 += 0.01f;
	 }

	//Abrir la palma
	 if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		 //Dedos de arriba abren hacia arriba
		 if (dedo1 < 0.0f)  dedo1 += 0.01f;   
		 if (dedo2 < 0.0f)  dedo2 += 0.01f;   

		 if (dedo3 < 0.0f)  dedo3 += 0.01f;
		 if (dedo4 < 0.0f)  dedo4 += 0.01f;

		 if (dedo5 < 0.0f)  dedo5 += 0.01f;
		 if (dedo6 < 0.0f)  dedo6 += 0.01f;

		 //Dedos de abajo abren hacia abajo
		 if (dedo7 > 0.0f)   dedo7 -= 0.01f;
		 if (dedo8 > 0.0f)   dedo8 -= 0.01f;

		 if (dedo9 > 0.0f)   dedo9 -= 0.01f;
		 if (dedo10 > 0.0f)  dedo10 -= 0.01f;
	 }



 }



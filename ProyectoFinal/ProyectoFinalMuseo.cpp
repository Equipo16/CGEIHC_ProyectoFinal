#include <iostream>
#include <cmath> 

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation(GLfloat deltaTime); 

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	// ... (Vértices del cubo, sin cambios) ...
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


bool animMummy = false;      // Controla si la animación está activa
float mummyLegRot = 0.0f;    // Rotación actual de las piernas/brazos
bool mummyStep = false;      // Controla la dirección del paso
glm::vec3 mummyPos = glm::vec3(37.0f, 2.8f, 0.0f); // Posición inicial de la momia (ANIMADA)
float mummyRot = 270.0f;      // Rotación inicial de la momia

const float MUMMY_SCALE_MIN = 1.0f; // Escala pequeña (dentro del sarcófago)
const float MUMMY_SCALE_MAX = 1.4f; // Escala normal (fuera)
float mummyScale = MUMMY_SCALE_MIN; // Escala actual (empieza pequeña)



bool animSarcofago = false;   // Controla si la animación *general* está activa
glm::vec3 sarcofagoPos = glm::vec3(37.0f, 2.8f, 0.0f); // Posición FIJA del sarcófago
float tapaPosCerrado = 0.0f;  // Posición X CERRADA (relativa al punto de inicio)
float tapaPosAbierto = 0.5f;  // Posición X ABIERTA (0.5 unidades a la DERECHA)
float tapaPosX = tapaPosCerrado; // Posición X actual (empieza CERRADA)
float tapaAnimSpeed = 0.0002f; 
float tapaPosBaseZ = 0.2f;      // Posición Z base (la que tenías en Draw)
float tapaLevitateAmount = 0.3f; // Cuánto se eleva/adelanta (ej. 0.3 unidades)
float tapaPosZ = tapaPosBaseZ;  // Posición Z actual (empieza en la base)
float tapaShakeAmount = 0.008f; 


int animState = 0;             // 0=idle, 1=opening, 2=walk_fwd, 3=wave, 4=walk_back, 5=closing
float mummyMoveSpeed = 0.001f; // Velocidad de caminata
float mummyWalkTargetDist = 1.0f; // Cuánto camina (ej. 1.0 unidad "3 pasos")
float mummyWalkDistance = 0.0f;   // Cuánto ha caminado
float stateTimer = 0.0f;       // Timer para el estado de saludo
float mummyArmRot = 0.0f;      // Rotación del brazo para saludar (AHORA NO SE USA)
float mummyArmSpeed = 1.0f;    // Velocidad del saludo 
float mummyArmMaxAngle = 90.0f; // Ángulo del saludo 
bool mummyArmWavingUp = true;  


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 11 Sandra Laparra", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	// --- AÑADIR MODELOS DE LA MOMIA ---
	Model MummyBody((char*)"Models/cuerpo.obj");
	Model MummyArmL((char*)"Models/brazo_izq.obj");
	Model MummyArmR((char*)"Models/brazo_der.obj");
	Model MummyLegR((char*)"Models/pierna_der.obj");
	Model MummyLegL((char*)"Models/pierna_izq.obj");
	// ------------------------------------

	// --- AÑADIR MODELO DEL SARCÓFAGO ---
	Model Sarcofago((char*)"Models/sarcofago.obj");
	Model TapaSarcofago((char*)"Models/tapa_sarcofago.obj");
	// -----------------------------------


	Model SalaEgipcia((char*)"Models/museo.obj"); 
	// Model BancoModel((char*)"Models/banco.obj");     

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation(deltaTime); // <-- Pasa deltaTime a la animación

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 sarcofagoModelTemp = glm::mat4(1.0f); // Matriz TEMPORAL para el sarcófago

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// ... (Toda la configuración de luces queda igual) ...
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);


		
		
		model = glm::mat4(1); // Matriz identidad para la sala
		// Puedes descomentar y ajustar la escala o posición si es necesario:
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -2.0f)); // Ejemplo de traslación
		// model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Ejemplo de escala
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0); // Asegurarse que no es transparente
		SalaEgipcia.Draw(lightingShader); 
		
		


		// --- DIBUJAR BANCO (BLOQUE COMENTADO) ---
		/*
		// 1. Empezar con la matriz de la sala
		glm::mat4 bancoModel = glm::mat4(1);

		// 2. Aplicar transformaciones LOCALES (DENTRO de la sala)

		//    a) Moverlo a la derecha (X=3.0) y al fondo (Z=-3.0)
		bancoModel = glm::translate(bancoModel, glm::vec3(33.0f, 1.0f, -3.0f));
		//    b) Escalarlo para que se vea (¡Importante!)
		bancoModel = glm::scale(bancoModel, glm::vec3(3.5f, 3.5f, 3.5f));

		// 3. Dibujar el banco con su matriz final
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(bancoModel));
		BancoModel.Draw(lightingShader);
		*/
		// --- FIN DIBUJO BANCO ---


		
		sarcofagoModelTemp = glm::mat4(1);
		sarcofagoModelTemp = glm::translate(sarcofagoModelTemp, sarcofagoPos);
		sarcofagoModelTemp = glm::rotate(sarcofagoModelTemp, glm::radians(mummyRot), glm::vec3(0.0f, 1.0f, 0.0f));


		
		model = glm::mat4(1); // Reiniciar matriz

		// 1. Aplicar transformaciones base (Posición y Rotación)
		//    ¡Ahora se usa la variable animada mummyPos!
		model = glm::translate(model, mummyPos);
		model = glm::rotate(model, glm::radians(mummyRot), glm::vec3(0.0f, 1.0f, 0.0f));

		
		// Se aplica aquí para que afecte a todas las partes del cuerpo por igual.
		model = glm::scale(model, glm::vec3(mummyScale, mummyScale, mummyScale));
		// -------------------------------------

		// Guardar esta matriz base (la del cuerpo)
		modelTemp = model;

		// 2. Dibujar el Cuerpo
		// Este es tu centro.
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyBody.Draw(lightingShader);

		// 3. Dibujar Pierna Izquierda (POSICIONADA)
		model = modelTemp; // Volver a la pos del cuerpo
		// (Valores para moverla abajo y a la izquierda)
		model = glm::translate(model, glm::vec3(0.08f, -0.9f, 0.09f));
		model = glm::rotate(model, glm::radians(mummyLegRot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyLegL.Draw(lightingShader);

		// 4. Dibujar Pierna Derecha (POSICIONADA)
		model = modelTemp; // Volver a la pos del cuerpo
		// (Valores para moverla abajo y a la derecha)
		model = glm::translate(model, glm::vec3(-0.1f, -0.9f, 0.0f));
		model = glm::rotate(model, glm::radians(-mummyLegRot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyLegR.Draw(lightingShader);


		// 5. Dibujar Brazo Izquierdo (POSICIONADO)
		model = modelTemp; // Volver a la pos del cuerpo
		// ** Ajusta este '0.0f' a '0.1f' o '0.2f' si quieres subir los brazos **
		model = glm::translate(model, glm::vec3(0.24f, 0.15f, 0.0f)); // Mover al hombro izquierdo

		float leftArmXRot = 0.0f;
		float leftArmYRot = 0.0f; 

		if (animState == 2 || animState == 3) // CAMINANDO AFUERA o SALUDANDO
		{
			leftArmXRot = -90.0f; // Brazo estirado al frente
			leftArmYRot = 0.0f;
		}
		
		// Ahora el estado 4 (caminando de regreso) usa la pose cruzada
		else // QUIETO (0, 1, 5) O CAMINANDO DE REGRESO (4) - ¡POSE CRUZADA!
		{
			leftArmXRot = -75.0f; // Rotar hacia adelante (casi 90)
			leftArmYRot = 35.0f;  // Rotar hacia adentro (cruzado)
		}

		// APLICAR ROTACIONES
		model = glm::rotate(model, glm::radians(leftArmXRot), glm::vec3(1.0f, 0.0f, 0.0f)); // Adelante/Atrás
		model = glm::rotate(model, glm::radians(leftArmYRot), glm::vec3(0.0f, 1.0f, 0.0f)); // Cruzar

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Escala (se mantiene 0.6f)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyArmL.Draw(lightingShader);

		// 6. Dibujar Brazo Derecho (POSICIONADO)
		
		model = modelTemp; // Volver a la pos del cuerpo
		// ** Ajusta este '0.0f' a '0.1f' o '0.2f' si quieres subir los brazos **
		model = glm::translate(model, glm::vec3(-0.24f, 0.15f, 0.0f)); // Mover al hombro derecho

		float rightArmXRot = 0.0f;
		float rightArmYRot = 0.0f; 

		if (animState == 2 || animState == 3) // CAMINANDO AFUERA o SALUDANDO
		{
			rightArmXRot = -90.0f; // Brazo estirado al frente
			rightArmYRot = 0.0f;
		}
		
		// Ahora el estado 4 (caminando de regreso) usa la pose cruzada
		else // QUIETO (0, 1, 5) O CAMINANDO DE REGRESO (4) - ¡POSE CRUZADA!
		{
			rightArmXRot = -75.0f; // Rotar hacia adelante (casi 90)
			rightArmYRot = -35.0f; // Rotar hacia adentro (cruzado) - opuesto al izquierdo
		}

		// APLICAR ROTACIONES
		model = glm::rotate(model, glm::radians(rightArmXRot), glm::vec3(1.0f, 0.0f, 0.0f)); // Adelante/Atrás
		model = glm::rotate(model, glm::radians(rightArmYRot), glm::vec3(0.0f, 1.0f, 0.0f)); // Cruzar

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); // Escala (se mantiene 0.6f)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyArmR.Draw(lightingShader);
		

		model = sarcofagoModelTemp; 

		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sarcofago.Draw(lightingShader);
		

		float shakeOffsetY = 0.0f; // Por defecto no tiembla
		// Solo tiembla si está abriendo (1) o cerrando (5)
		if (animState == 1 || animState == 5)
		{
			
			shakeOffsetY = sinf(currentFrame * 10.0f) * tapaShakeAmount;
		}
		


		
		model = sarcofagoModelTemp; 

		// Aplicamos LA MISMA escala
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));

		
		// Aplicamos la traslación Y (vertical) fija + el TEMBLOR (shakeOffsetY)
		// la traslación X animada (tapaPosX)
		// y la traslación Z animada de levitación (tapaPosZ)
		model = glm::translate(model, glm::vec3(tapaPosX, -0.1f + shakeOffsetY, tapaPosZ));
		// -------------------------------------

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TapaSarcofago.Draw(lightingShader);
		

		
		/*
		// ... (código de la lámpara) ...
		*/

		//Draw SkyBox
		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);
		}
	}

	// --- AÑADIR TECLA M PARA LA MOMIA ---
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		animMummy = !animMummy; // Esto sigue funcionando para la animación de caminar simple
	}
	// -------------------------------------

	// --- AÑADIR TECLA N PARA EL SARCÓFAGO ---
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		// Solo inicia la secuencia si está en estado IDLE (0)
		if (animState == 0)
		{
			animSarcofago = true; // Activa la animación general
			animState = 1;        // Pasa al primer estado (Abrir tapa)
		}
	}
	// ----------------------------------------
}


void Animation(GLfloat deltaTime) {

	
	// Esta lógica se activa solo cuando animMummy es true
	if (animMummy)
	{
		
		float animSpeed = 0.02f; // Velocidad de la pierna (antes 0.05f)
		float maxAngle = 40.0f;  // Ángulo del paso 

		if (!mummyStep) {
			mummyLegRot += animSpeed;
			if (mummyLegRot > maxAngle) {
				mummyLegRot = maxAngle; // Corregido para que no se pase
				mummyStep = true;
			}
		}
		else {
			mummyLegRot -= animSpeed;
			if (mummyLegRot < -maxAngle) { // Esta es la forma correcta
				mummyLegRot = -maxAngle; // Corregido para que no se pase
				mummyStep = false;
			}
		}
	}
	else
	{
		// Si no está animada, volver a la posición de reposo
		// (Los brazos se controlan en main)
		mummyLegRot = 0.0f;
	}
	// --------------------------------------------

	
	if (animSarcofago) 
	{
		// Variable para el ratio de apertura/cierre (0.0 a 1.0)
		float ratio = 0.0f;

		switch (animState)
		{
		case 1: // Estado 1: Abriendo Tapa (MOVER A LA DERECHA)
			tapaPosX += tapaAnimSpeed; // Mover hacia la derecha (AHORA MÁS LENTO)

			// --- LÓGICA DE LEVITACIÓN (EJE Z) ---
			// Calcula qué tan "abierto" está (de 0.0 a 1.0)
			ratio = tapaPosX / tapaPosAbierto;
			// Usa sin() para que suba y baje. (sin(0) = 0, sin(PI/2) = 1, sin(PI) = 0)
			// 3.14159f es el valor de PI
			tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
			// --------------------------------------------

			if (tapaPosX > tapaPosAbierto) // Comprobar si llegó al punto ABIERTO
			{
				tapaPosX = tapaPosAbierto; // Asegurar que no se pase
				tapaPosZ = tapaPosBaseZ;   // Asegurar que vuelve a la base Z
				animState = 2;             // Siguiente estado: caminar adelante
				animMummy = true;          // Activar animación de caminar
			}
			break;

		case 2: // Estado 2: Momia caminando hacia adelante
		{ 

			// Moverse "hacia adelante" relativo a la rotación de la momia
			mummyPos.x += sinf(glm::radians(mummyRot)) * mummyMoveSpeed;
			mummyPos.z += cosf(glm::radians(mummyRot)) * mummyMoveSpeed;

			mummyWalkDistance += mummyMoveSpeed; // Acumular la distancia caminada

			
			// Interpolar la escala basada en la distancia caminada
			float walkRatio = mummyWalkDistance / mummyWalkTargetDist; // Sube de 0.0 a 1.0
			mummyScale = MUMMY_SCALE_MIN + (walkRatio * (MUMMY_SCALE_MAX - MUMMY_SCALE_MIN));
			if (mummyScale > MUMMY_SCALE_MAX) mummyScale = MUMMY_SCALE_MAX; // Asegurar límite
			// ------------------------------------

			if (mummyWalkDistance > mummyWalkTargetDist) // Comprobar si llegó al objetivo
			{
				mummyScale = MUMMY_SCALE_MAX; // Asegurar que termina en escala MÁXIMA
				animState = 3;               // Siguiente estado: saludar
				animMummy = false;             // Detener animación de caminar (piernas)
				stateTimer = 0.0f;
			}
		} 
		break;

		case 3: // Estado 3: Momia en pausa (brazos estirados)
			stateTimer += deltaTime; // Incrementar el temporizador

			

			// Espera por 3 segundos
			if (stateTimer > 3.0f)
			{
				animState = 4;       // Siguiente estado: caminar atrás
				animMummy = true;    // Activar animación de caminar (piernas)
				mummyArmRot = 0.0f;  // Resetear brazo (por si acaso)
			}
			break;

		case 4: // Estado 4: Momia caminando hacia atrás
		{ 

			// Moverse "hacia atrás" relativo a la rotación
			mummyPos.x -= sinf(glm::radians(mummyRot)) * mummyMoveSpeed;
			mummyPos.z -= cosf(glm::radians(mummyRot)) * mummyMoveSpeed;

			mummyWalkDistance -= mummyMoveSpeed; // Reducir la distancia caminada

			
			// Interpolar la escala basada en la distancia (ahora baja de 1.0 a 0.0)
			float walkRatioBack = mummyWalkDistance / mummyWalkTargetDist;
			mummyScale = MUMMY_SCALE_MIN + (walkRatioBack * (MUMMY_SCALE_MAX - MUMMY_SCALE_MIN));
			if (mummyScale < MUMMY_SCALE_MIN) mummyScale = MUMMY_SCALE_MIN; // Asegurar límite
			// ---------------------------------------


			if (mummyWalkDistance < 0.0f)
			{
				mummyWalkDistance = 0.0f;
				mummyPos = sarcofagoPos; // Resetear la momia a la pos original
				mummyScale = MUMMY_SCALE_MIN; // ¡NUEVO! Resetear escala a pequeña
				animState = 5;         // Siguiente estado: cerrar tapa
				animMummy = false;     // Detener animación de caminar (piernas)
			}
		} 
		break;

		case 5: // Estado 5: Cerrando Tapa (MOVER A LA IZQUIERDA)
			tapaPosX -= tapaAnimSpeed; // Mover hacia la izquierda 

			// --- LÓGICA DE LEVITACIÓN (EJE Z) ---
			// Calcula qué tan "abierto" está (de 1.0 a 0.0)
			ratio = tapaPosX / tapaPosAbierto;
			// Usa sin() para que suba y baje. (sin(PI) = 0, sin(PI/2) = 1, sin(0) = 0)
			tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
			// ---------------------------------------------

			if (tapaPosX < tapaPosCerrado) // Comprobar si llegó al punto CERRADO
			{
				tapaPosX = tapaPosCerrado; // Asegurar que no se pase
				tapaPosZ = tapaPosBaseZ;   // Asegurar que vuelve a la base Z
				animState = 0;             // Volver al estado IDLE
				animSarcofago = false;     // Terminar la secuencia
			}
			break;
		}
	}
	// -----------------------------------------------
}



void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xPos; // <--- Cast a GLfloat
		lastY = (GLfloat)yPos; // <--- Cast a GLfloat
		firstMouse = false;
	}

	GLfloat xOffset = (GLfloat)xPos - lastX; // <--- Cast a GLfloat
	GLfloat yOffset = lastY - (GLfloat)yPos;  // <--- Cast a GLfloat

	lastX = (GLfloat)xPos; // <--- Cast a GLfloat
	lastY = (GLfloat)yPos; // <--- Cast a GLfloat

	camera.ProcessMouseMovement(xOffset, yOffset);
}
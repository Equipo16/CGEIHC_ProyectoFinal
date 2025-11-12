#include <iostream>
#include <cmath> 
#include <string> 

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
void KeyCallback(GLFWwindow* window, int key, int scode, int action, int mode);
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
bool active = false; // Inicializada a false

// ----------------------------------------------------
// Todas las luces
const int NR_POINT_LIGHTS = 17; 

glm::vec3 pointLightPositions[NR_POINT_LIGHTS] = {
	

	// Sala Escultura Clásica (4 luces) 
	glm::vec3(-42.0042f, 7.5931f, 2.59023f),  // T + (-37.0042, 7.5931, 4.59023)
	glm::vec3(-42.0042f, 7.5931f, 11.0841f),  // T + (-37.0042, 7.5931, 13.0841)
	glm::vec3(-42.0042f, 7.5931f, -6.27355f), // T + (-37.0042, 7.5931, -4.27355)
	glm::vec3(-42.0042f, 7.5931f, -15.0841f), // T + (-37.0042, 7.5931, -13.0841)

	// Sala Egipcia (4 luces) 
	glm::vec3(33.6007f, 7.5931f, 2.9147f),    // T + (38.6007, 7.5931, 4.9147)
	glm::vec3(33.6007f, 7.5931f, 11.0841f),   // T + (38.6007, 7.5931, 13.0841)
	glm::vec3(33.6007f, 7.5931f, -5.48307f),  // T + (38.6007, 7.5931, -3.48307)
	glm::vec3(33.6007f, 7.5931f, -14.3678f),  // T + (38.6007, 7.5931, -12.3678)

	// Sala Pintura Clásica (5 luces) 
	glm::vec3(23.5071f, 5.91557f, 33.131f),   // T + (28.5071, 5.91557, 35.131)
	glm::vec3(18.5369f, 5.91557f, 33.131f),   // T + (23.5369, 5.91557, 35.131)
	glm::vec3(13.5369f, 5.91557f, 33.131f),   // T + (18.5369, 5.91557, 35.131)
	glm::vec3(8.5369f, 5.91557f, 33.131f),    // T + (13.5369, 5.91557, 35.131)
	glm::vec3(3.5369f, 5.91557f, 33.131f),    // T + (8.5369, 5.91557, 35.131)

	// Sala Arte Moderno (4 luces) 
	glm::vec3(-13.2757f, 6.70094f, 33.2526f), // T + (-8.2757, 6.70094, 35.2526)
	glm::vec3(-21.9787f, 6.70094f, 33.2526f), // T + (-16.9787, 6.70094, 35.2526)
	glm::vec3(-30.636f, 6.70094f, 33.2526f),  // T + (-25.636, 6.70094, 35.2526)
	glm::vec3(-39.2367f, 6.70094f, 33.2526f)  // T + (-34.2367, 6.70094, 35.2526)
};

// Arreglo para guardar el color de cada luz
glm::vec3 pointLightColors[NR_POINT_LIGHTS];

// Constantes de color (Amarillo para todas)
const glm::vec3 COLOR_YELLOW_ON = glm::vec3(1.0f, 1.0f, 0.0f);
const glm::vec3 COLOR_OFF = glm::vec3(0.0f);
// ----------------------------------------------------


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

//Variables Animación Momia/Sarcófago
bool animMummy = false;      // Controla si la animación está activa
float mummyLegRot = 0.0f;    // Rotación actual de las piernas/brazos
bool mummyStep = false;      // Controla la dirección del paso
glm::vec3 mummyPos = glm::vec3(37.0f, 2.8f, 0.0f); // Posición inicial de la momia (ANIMADA)
float mummyRot = 270.0f;      // Rotación inicial de la momia

const float MUMMY_SCALE_MIN = 1.0f; // Escala pequeña (dentro del sarcófago)
const float MUMMY_SCALE_MAX = 1.4f; // Escala normal (fuera)
float mummyScale = MUMMY_SCALE_MIN; // Escala actual (empieza pequeña)



bool animSarcofago = false;   // Controla si la animación *general* está activa
glm::vec3 sarcofagoPos = glm::vec3(37.0f, 2.8f, 0.0f); // Posición fija del sarcófago
float tapaPosCerrado = 0.0f;  // Posición X CERRADA (relativa al punto de inicio)
float tapaPosAbierto = 0.5f;  // Posición X ABIERTA (0.5 unidades a la DERECHA)
float tapaPosX = tapaPosCerrado; // Posición X actual (empieza CERRADA)
float tapaAnimSpeed = 0.0002f;
float tapaPosBaseZ = 0.2f;      // Posición Z base 
float tapaLevitateAmount = 0.3f; // Cuánto se eleva/adelanta (ej. 0.3 unidades)
float tapaPosZ = tapaPosBaseZ;  // Posición Z actual (empieza en la base)
float tapaShakeAmount = 0.008f;


int animState = 0;             // 0=idle, 1=opening, 2=walk_fwd, 3=wave, 4=walk_back, 5=closing
float mummyMoveSpeed = 0.001f; // Velocidad de caminata
float mummyWalkTargetDist = 1.0f; 
float mummyWalkDistance = 0.0f;   // Cuánto ha caminado
float stateTimer = 0.0f;       // Timer para el estado de saludo
float mummyArmRot = 0.0f;      
float mummyArmSpeed = 1.0f;    // Velocidad del saludo 
float mummyArmMaxAngle = 90.0f; // Ángulo del saludo 
bool mummyArmWavingUp = true;


// Variables para animación de manecillas
bool animacionActiva = false;
float anguloHorario = 0.0f;
float anguloMinutero = 0.0f;

// Variables para el péndulo
float anguloPendulo = 0.0f;
float amplitudPendulo = 5.0f; // Ángulo máximo de oscilación (grados)
glm::vec3 pivotManecillas(0.460781f, 2.10701f, 0.006229f);
glm::vec3 pivotPendulo(0.295819f, 1.60216f, 0.006229f);

// Variables para engranajes
float anguloEng1 = 0.0f;
float anguloEng2 = 0.0f;
glm::vec3 pivotEng1(0.150284f, 1.14464f, -0.375564f);
glm::vec3 pivotEng2(0.151259f, 0.477735f, -0.321072f);

// Variables para campana y badajo
float anguloCampana = 0.0f;
float escalaCampana = 1.0f;
bool campanaSonando = false;
float tiempoCampana = 0.0f;
glm::vec3 pivotCampana(0.168226f, 2.9f, 0.0f);

// Traslación global para todos los modelos del reloj
glm::vec3 traslacionGlobalReloj(-42.6714f, 0.58425f, -0.088454f);



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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Museo", nullptr, nullptr);

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


	//Modelos de las extremidades de la momia
	Model MummyBody((char*)"Models/cuerpo.obj");
	Model MummyArmL((char*)"Models/brazo_izq.obj");
	Model MummyArmR((char*)"Models/brazo_der.obj");
	Model MummyLegR((char*)"Models/pierna_der.obj");
	Model MummyLegL((char*)"Models/pierna_izq.obj");
	

	//Modelo del sarcófago
	Model Sarcofago((char*)"Models/sarcofago.obj");
	Model TapaSarcofago((char*)"Models/tapa_sarcofago.obj");
	


	Model SalasMuseo((char*)"Models/museo.obj");

	//Modelo Reloj y sus componentes
	Model reloj((char*)"Models/relojp.obj");
	Model Pend((char*)"Models/pend.obj");
	Model min((char*)"Models/minutero.obj");
	Model hor((char*)"Models/horario.obj");
	Model Eng1((char*)"Models/eng1.obj");
	Model Eng2((char*)"Models/eng2.obj");
	Model Campana((char*)"Models/camp.obj");
	Model Badajo((char*)"Models/bad.obj");
	

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


	//Inicializar todas las luces como apagadas
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		pointLightColors[i] = COLOR_OFF;
	}

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
		glm::mat4 sarcofagoModelTemp = glm::mat4(1.0f); // Matriz temporal para el sarcófago

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		//Loop para enviar todas las luces puntuales al shader
		for (int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			// Crear los nombres de los uniforms dinámicamente
			std::string posName = "pointLights[" + std::to_string(i) + "].position";
			std::string ambName = "pointLights[" + std::to_string(i) + "].ambient";
			std::string diffName = "pointLights[" + std::to_string(i) + "].diffuse";
			std::string specName = "pointLights[" + std::to_string(i) + "].specular";
			std::string constName = "pointLights[" + std::to_string(i) + "].constant";
			std::string linName = "pointLights[" + std::to_string(i) + "].linear";
			std::string quadName = "pointLights[" + std::to_string(i) + "].quadratic";

			// Enviar datos al shader
			glUniform3f(glGetUniformLocation(lightingShader.Program, posName.c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, ambName.c_str()), pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, diffName.c_str()), pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, specName.c_str()), pointLightColors[i].x, pointLightColors[i].y, pointLightColors[i].z);

			
			glUniform1f(glGetUniformLocation(lightingShader.Program, constName.c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, linName.c_str()), 0.022f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, quadName.c_str()), 0.035f);
		}
		


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
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		SalasMuseo.Draw(lightingShader);


		sarcofagoModelTemp = glm::mat4(1);
		sarcofagoModelTemp = glm::translate(sarcofagoModelTemp, sarcofagoPos);
		sarcofagoModelTemp = glm::rotate(sarcofagoModelTemp, glm::radians(mummyRot), glm::vec3(0.0f, 1.0f, 0.0f));



		model = glm::mat4(1); // Reiniciar matriz

		// 1. Aplicar transformaciones base (Posición y Rotación)
		model = glm::translate(model, mummyPos);
		model = glm::rotate(model, glm::radians(mummyRot), glm::vec3(0.0f, 1.0f, 0.0f));


		// Se aplica aquí para que afecte a todas las partes del cuerpo por igual.
		model = glm::scale(model, glm::vec3(mummyScale, mummyScale, mummyScale));

		// Guardar esta matriz base (la del cuerpo)
		modelTemp = model;

		// 2. Dibujar el Cuerpo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyBody.Draw(lightingShader);

		// 3. Dibujar Pierna Izquierda 
		model = modelTemp; // Volver a la pos del cuerpo
		model = glm::translate(model, glm::vec3(0.08f, -0.9f, 0.09f));
		model = glm::rotate(model, glm::radians(mummyLegRot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyLegL.Draw(lightingShader);

		// 4. Dibujar Pierna Derecha 
		model = modelTemp; // Volver a la pos del cuerpo
		model = glm::translate(model, glm::vec3(-0.1f, -0.9f, 0.0f));
		model = glm::rotate(model, glm::radians(-mummyLegRot), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyLegR.Draw(lightingShader);


		// 5. Dibujar Brazo Izquierdo 
		model = modelTemp; // Volver a la pos del cuerpo
		model = glm::translate(model, glm::vec3(0.24f, 0.15f, 0.0f)); // Mover al hombro izquierdo

		float leftArmXRot = 0.0f;
		float leftArmYRot = 0.0f;

		if (animState == 2 || animState == 3) 
		{
			leftArmXRot = -90.0f; // Brazo estirado al frente
			leftArmYRot = 0.0f;
		}


		else 
		{
			leftArmXRot = -75.0f; // Rotar hacia adelante (casi 90)
			leftArmYRot = 35.0f;  // Rotar hacia adentro (cruzado)
		}

		// Aplicar rotaciones
		model = glm::rotate(model, glm::radians(leftArmXRot), glm::vec3(1.0f, 0.0f, 0.0f)); // Adelante/Atrás
		model = glm::rotate(model, glm::radians(leftArmYRot), glm::vec3(0.0f, 1.0f, 0.0f)); // Cruzar

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyArmL.Draw(lightingShader);

		// 6. Dibujar Brazo Derecho 

		model = modelTemp; 
		model = glm::translate(model, glm::vec3(-0.24f, 0.15f, 0.0f)); // Mover al hombro derecho

		float rightArmXRot = 0.0f;
		float rightArmYRot = 0.0f;

		if (animState == 2 || animState == 3) 
		{
			rightArmXRot = -90.0f; // Brazo estirado al frente
			rightArmYRot = 0.0f;
		}

		else 
		{
			rightArmXRot = -75.0f; // Rotar hacia adelante (casi 90)
			rightArmYRot = -35.0f; // Rotar hacia adentro (cruzado) - opuesto al izquierdo
		}

		// APLICAR ROTACIONES
		model = glm::rotate(model, glm::radians(rightArmXRot), glm::vec3(1.0f, 0.0f, 0.0f)); // Adelante/Atrás
		model = glm::rotate(model, glm::radians(rightArmYRot), glm::vec3(0.0f, 1.0f, 0.0f)); // Cruzar

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MummyArmR.Draw(lightingShader);


		model = sarcofagoModelTemp;

		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sarcofago.Draw(lightingShader);


		float shakeOffsetY = 0.0f; //levitacion de la tapa
		if (animState == 1 || animState == 5)
		{

			shakeOffsetY = sinf(currentFrame * 10.0f) * tapaShakeAmount;
		}


		model = sarcofagoModelTemp;
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::translate(model, glm::vec3(tapaPosX, -0.1f + shakeOffsetY, tapaPosZ));
		

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TapaSarcofago.Draw(lightingShader);

		//Reloj

		// Dibujar reloj principal
		glm::mat4 modelReloj = glm::mat4(1.0f);
		modelReloj = glm::translate(modelReloj, traslacionGlobalReloj);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelReloj));
		reloj.Draw(lightingShader);

		// Dibujar péndulo con animación
		glm::mat4 modelPend = glm::mat4(1.0f);
		modelPend = glm::translate(modelPend, traslacionGlobalReloj);

		// Aplicar rotación con pivote del péndulo - oscilación en eje X
		modelPend = glm::translate(modelPend, pivotPendulo);
		modelPend = glm::rotate(modelPend, glm::radians(anguloPendulo), glm::vec3(1.0f, 0.0f, 0.0f)); // Oscila en X
		modelPend = glm::translate(modelPend, -pivotPendulo);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPend));
		Pend.Draw(lightingShader);

		// Dibujar minutero con rotación y pivote ajustado
		glm::mat4 modelMin = glm::mat4(1.0f);
		modelMin = glm::translate(modelMin, traslacionGlobalReloj);

		// Aplicar rotación con pivote corregido
		modelMin = glm::translate(modelMin, pivotManecillas);
		modelMin = glm::rotate(modelMin, glm::radians(anguloMinutero), glm::vec3(1.0f, 0.0f, 0.0f)); // Eje X
		modelMin = glm::translate(modelMin, -pivotManecillas);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMin));
		min.Draw(lightingShader);

		// Dibujar horario con rotación y pivote ajustado
		glm::mat4 modelHor = glm::mat4(1.0f);
		modelHor = glm::translate(modelHor, traslacionGlobalReloj);

		// Aplicar rotación con pivote corregido
		modelHor = glm::translate(modelHor, pivotManecillas);
		modelHor = glm::rotate(modelHor, glm::radians(anguloHorario), glm::vec3(1.0f, 0.0f, 0.0f)); // Eje X
		modelHor = glm::translate(modelHor, -pivotManecillas);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHor));
		hor.Draw(lightingShader);

		// Dibujar engranaje 1 (pequeño) con animación
		glm::mat4 modelEng1 = glm::mat4(1.0f);
		modelEng1 = glm::translate(modelEng1, traslacionGlobalReloj);

		// Aplicar rotación con pivote
		modelEng1 = glm::translate(modelEng1, pivotEng1);
		modelEng1 = glm::rotate(modelEng1, glm::radians(anguloEng1), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en eje Z
		modelEng1 = glm::translate(modelEng1, -pivotEng1);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng1));
		Eng1.Draw(lightingShader);

		// Dibujar engranaje 2 (grande) con animación
		glm::mat4 modelEng2 = glm::mat4(1.0f);
		modelEng2 = glm::translate(modelEng2, traslacionGlobalReloj);

		// Aplicar rotación con pivote
		modelEng2 = glm::translate(modelEng2, pivotEng2);
		modelEng2 = glm::rotate(modelEng2, glm::radians(anguloEng2), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en eje Z
		modelEng2 = glm::translate(modelEng2, -pivotEng2);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng2));
		Eng2.Draw(lightingShader);

		// Dibujar campana con animación de vibración
		glm::mat4 modelCampana = glm::mat4(1.0f);
		modelCampana = glm::translate(modelCampana, traslacionGlobalReloj);

		// Aplicar animación de vibración con pivote
		modelCampana = glm::translate(modelCampana, pivotCampana);
		modelCampana = glm::rotate(modelCampana, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f)); // Vibración en eje X
		modelCampana = glm::scale(modelCampana, glm::vec3(escalaCampana, escalaCampana, escalaCampana)); // Efecto de vibración con escala
		modelCampana = glm::translate(modelCampana, -pivotCampana);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCampana));
		Campana.Draw(lightingShader);

		// Dibujar badajo (se mueve con la campana)
		glm::mat4 modelBadajo = glm::mat4(1.0f);
		modelBadajo = glm::translate(modelBadajo, traslacionGlobalReloj);

		// Aplicar misma animación que la campana
		modelBadajo = glm::translate(modelBadajo, pivotCampana);
		modelBadajo = glm::rotate(modelBadajo, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f)); // Vibración en eje X
		modelBadajo = glm::scale(modelBadajo, glm::vec3(escalaCampana, escalaCampana, escalaCampana)); // Misma escala que campana
		modelBadajo = glm::translate(modelBadajo, -pivotCampana);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBadajo));
		Badajo.Draw(lightingShader);
		

		// Dibujar todas las lámparas 
		lampShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			// Solo dibuja la lámpara si está encendida
			if (pointLightColors[i] == COLOR_YELLOW_ON)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f)); // Un cubo pequeño
				glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		glBindVertexArray(0);


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
		pointLightPositions[1].x += 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[1].x -= 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].y += 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].y -= 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[1].z -= 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[1].z += 0.1f;
		std::cout << "Luz 1: X=" << pointLightPositions[1].x << ", Y=" << pointLightPositions[1].y << ", Z=" << pointLightPositions[1].z << std::endl;
	}

}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scode, int action, int mode)
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

	// La barra espaciadora enciende/apaga TODAS las luces
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		active = !active; // Invierte el estado

		// Determina el color basado en el estado
		glm::vec3 newColor = active ? COLOR_YELLOW_ON : COLOR_OFF;

		// Aplica el nuevo color a todas las luces
		for (int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			pointLightColors[i] = newColor;
		}
	}
	

	//Tecla M para la momia
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		animMummy = !animMummy; // Esto sigue funcionando para la animación de caminar simple
	}
	

	//Tecla N para destapar el sarcofago
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		
		if (animState == 0)
		{
			animSarcofago = true; // Activa la animación general
			animState = 1;        // Pasa al primer estado "abrir tapa"
		}
	}
	

	
	// Tecla R para activar/desactivar animación del reloj
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		animacionActiva = !animacionActiva;
	}
	
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
		mummyLegRot = 0.0f;
	}
	


	if (animSarcofago)
	{
		
		float ratio = 0.0f;

		switch (animState)
		{
		case 1: // Estado 1: Abriendo Tapa 
			tapaPosX += tapaAnimSpeed; // Mover hacia la derecha 

			// Levitación de la tapa
			ratio = tapaPosX / tapaPosAbierto;
			tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
			

			if (tapaPosX > tapaPosAbierto) // Comprobar si llegó al punto abierto
			{
				tapaPosX = tapaPosAbierto; // Asegurar que no se pase
				tapaPosZ = tapaPosBaseZ;   // Asegurar que vuelve a la base Z
				animState = 2;             // Siguiente estado: caminar adelante
				animMummy = true;          // Activar animación de caminar
			}
			break;

		case 2: // Estado 2: Momia caminando hacia adelante
		{

			
			mummyPos.x += sinf(glm::radians(mummyRot)) * mummyMoveSpeed;
			mummyPos.z += cosf(glm::radians(mummyRot)) * mummyMoveSpeed;

			mummyWalkDistance += mummyMoveSpeed; // Acumular la distancia caminada


			// Interpolar la escala basada en la distancia caminada
			float walkRatio = mummyWalkDistance / mummyWalkTargetDist; 
			mummyScale = MUMMY_SCALE_MIN + (walkRatio * (MUMMY_SCALE_MAX - MUMMY_SCALE_MIN));
			if (mummyScale > MUMMY_SCALE_MAX) mummyScale = MUMMY_SCALE_MAX; //límite
			

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
				mummyArmRot = 0.0f;  // Resetear brazo 
			}
			break;

		case 4: // Estado 4: Momia caminando hacia atrás
		{

			mummyPos.x -= sinf(glm::radians(mummyRot)) * mummyMoveSpeed;
			mummyPos.z -= cosf(glm::radians(mummyRot)) * mummyMoveSpeed;

			mummyWalkDistance -= mummyMoveSpeed; // Reducir la distancia caminada


			// Interpolar la escala basada en la distancia 
			float walkRatioBack = mummyWalkDistance / mummyWalkTargetDist;
			mummyScale = MUMMY_SCALE_MIN + (walkRatioBack * (MUMMY_SCALE_MAX - MUMMY_SCALE_MIN));
			if (mummyScale < MUMMY_SCALE_MIN) mummyScale = MUMMY_SCALE_MIN; //límite
			


			if (mummyWalkDistance < 0.0f)
			{
				mummyWalkDistance = 0.0f;
				mummyPos = sarcofagoPos; // Resetear la momia a la pos original
				mummyScale = MUMMY_SCALE_MIN; // Resetear escala a pequeña
				animState = 5;         // Siguiente estado: cerrar tapa
				animMummy = false;     // Detener animación de caminar 
			}
		}
		break;

		case 5: // Estado 5: Cerrando Tapa 
			tapaPosX -= tapaAnimSpeed; // Mover hacia la izquierda 

			// Levitación de la tapa del sarcofago
			ratio = tapaPosX / tapaPosAbierto;
			tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
			

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
	


	// Animación del reloj
	if (animacionActiva)
	{
		// Horario: más lento (30 grados por segundo)
		anguloHorario += 30.0f * deltaTime;
		// Minutero: más rápido (360 grados por segundo)
		anguloMinutero += 360.0f * deltaTime;

		// Mantener ángulos en rango [0, 360]
		if (anguloHorario >= 360.0f) anguloHorario -= 360.0f;
		if (anguloMinutero >= 360.0f) anguloMinutero -= 360.0f;

		// Animación del péndulo - oscilación suave en X (ida y vuelta)
		// Usar glfwGetTime() para una oscilación suave independiente
		float oscilacion = sin(glfwGetTime() * 2.0f) * amplitudPendulo;
		anguloPendulo = oscilacion;

		// Animación de engranajes - relación 2:1 (eng1 más rápido)
		anguloEng1 += 180.0f * deltaTime;  // Engranaje pequeño más rápido
		anguloEng2 -= 90.0f * deltaTime;   // Engranaje grande más lento (sentido contrario)

		// Mantener ángulos en rango [0, 360]
		if (anguloEng1 >= 360.0f) anguloEng1 -= 360.0f;
		if (anguloEng2 <= -360.0f) anguloEng2 += 360.0f;

		// Animación de la campana - suena cada 5 segundos (para prueba)
		static float tiempoDesdeUltimaCampana = 0.0f;
		tiempoDesdeUltimaCampana += deltaTime;

		if (tiempoDesdeUltimaCampana >= 5.0f) { // Suena cada 5 segundos
			campanaSonando = true;
			tiempoCampana = 0.0f;
			tiempoDesdeUltimaCampana = 0.0f;
		}

		// Controlar animación de la campana
		if (campanaSonando) {
			tiempoCampana += deltaTime;

			// Duración total de la animación de la campana (2 segundos)
			if (tiempoCampana < 2.0f) {
				// Movimiento oscilatorio rápido (vibración) - MÁS NOTABLE
				anguloCampana = sin(tiempoCampana * 40.0f) * 8.0f; // 8 grados de amplitud

				// Efecto de escala (crece y encoge para simular vibración) - MÁS NOTABLE
				escalaCampana = 1.0f + sin(tiempoCampana * 30.0f) * 0.2f; // 20% de variación
			}
			else {
				// Terminar animación
				campanaSonando = false;
				anguloCampana = 0.0f;
				escalaCampana = 1.0f;
			}
		}
	}
	
}



void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xPos; // Cast a GLfloat
		lastY = (GLfloat)yPos; // Cast a GLfloat
		firstMouse = false;
	}

	GLfloat xOffset = (GLfloat)xPos - lastX; // Cast a GLfloat
	GLfloat yOffset = lastY - (GLfloat)yPos;  // Cast a GLfloat

	lastX = (GLfloat)xPos; // Cast a GLfloat
	lastY = (GLfloat)yPos; // Cast a GLfloat

	camera.ProcessMouseMovement(xOffset, yOffset);
}
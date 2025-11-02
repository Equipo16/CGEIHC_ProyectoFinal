//Práctica 8
//Sandra Laparra Miranda
//Fecha de entrega: 12 de octubre de 2025
//Número de cuenta: 311243563

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes
bool dayMode = true;
float sunAngle = 0.0f;
float moonAngle = glm::radians(90.0f);


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 Sandra Laparra", nullptr, nullptr);
    
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

    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model tierra((char*)"Models/Tierra.obj");
    Model saturno((char*)"Models/Saturno.obj");
    Model jupiter((char*)"Models/Jupiter.obj");
    Model nave((char*)"Models/nave.obj");
    Model star((char*)"Models/star.obj");
    Model sol((char*)"Models/sol.obj");
    Model luna((char*)"Models/luna.obj");
    
    //glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    
    // Constantes para escalas y posiciones en el eje Z para cada objeto
    const float GLOBAL_SCALE = 0.02f;
    const float NAVE_SCALE = 0.06f;
    const float PLANET_SCALE = 0.35f;
    const float PLANET_Z = -30.0f;

    // Posiciones de estrellas
    std::vector<glm::vec3> starPositions = {
        // Parte superior
        glm::vec3(-20.0f, 18.0f, -28.0f),
        glm::vec3(-10.0f, 19.0f, -30.0f),
        glm::vec3(0.0f, 20.0f, -27.0f),
        glm::vec3(10.0f, 18.0f, -32.0f),
        glm::vec3(20.0f, 17.0f, -29.0f),
        glm::vec3(-15.0f, 16.0f, -31.0f),
        glm::vec3(5.0f, 15.0f, -28.0f),
        glm::vec3(-25.0f, 14.0f, -33.0f),
        glm::vec3(15.0f, 13.0f, -29.0f),
        glm::vec3(-5.0f, 12.0f, -34.0f),

        // Parte media
        glm::vec3(-18.0f,  8.0f, -30.0f),
        glm::vec3(-8.0f,  9.0f, -27.0f),
        glm::vec3(2.0f, 10.0f, -32.0f),
        glm::vec3(12.0f,  7.0f, -29.0f),
        glm::vec3(22.0f,  8.0f, -31.0f),
        glm::vec3(-12.0f,  5.0f, -28.0f),
        glm::vec3(8.0f,  6.0f, -33.0f),
        glm::vec3(-22.0f,  7.0f, -29.0f),
        glm::vec3(18.0f,  5.0f, -32.0f),
        glm::vec3(-2.0f,  4.0f, -27.0f),

        // Parte inferior
        glm::vec3(-20.0f, -5.0f, -29.0f),
        glm::vec3(-10.0f, -6.0f, -31.0f),
        glm::vec3(0.0f, -7.0f, -28.0f),
        glm::vec3(10.0f, -8.0f, -32.0f),
        glm::vec3(20.0f, -6.0f, -30.0f),
        glm::vec3(-15.0f, -9.0f, -27.0f),
        glm::vec3(5.0f, -10.0f, -33.0f),
        glm::vec3(-25.0f, -7.0f, -31.0f),
        glm::vec3(15.0f, -11.0f, -29.0f),
        glm::vec3(-5.0f, -12.0f, -34.0f)
    };

    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        if (keys[GLFW_KEY_Z]) {
            if (!dayMode) { sunAngle = 0.0f; }
            dayMode = true;
            sunAngle += 0.5f * deltaTime;
            if (sunAngle > glm::pi<float>()) { sunAngle = 0.0f; }
        } else if (keys[GLFW_KEY_X]) {
            if (dayMode) { moonAngle = glm::radians(90.0f); }
            dayMode = false;
            moonAngle += 0.8f * deltaTime;
        }

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        
        glm::vec3 activeLightPos;
        glm::vec3 ambientColor, diffuseColor, specularColor, lightModelColor;

        if (dayMode) {

            // Trayectoria del sol 
            const float sunArcRadius = 25.0f; // Radio para que no se salga de la pantalla
            const float sunArcHeight = 15.0f;
            activeLightPos.x = -cos(sunAngle) * sunArcRadius;
            activeLightPos.y = sin(sunAngle) * sunArcHeight;
            activeLightPos.z = -35.0f; // Un poco atrás de los planetas

            ambientColor = glm::vec3(0.4f, 0.4f, 0.3f);
            diffuseColor = glm::vec3(1.0f, 1.0f, 0.8f);
            specularColor = glm::vec3(1.0f, 1.0f, 0.8f);
            lightModelColor = glm::vec3(1.0f, 0.9f, 0.0f);

        } else {

            //Trayectoria de la luna
            const float moonRadius = 20.0f;
            const float moonHeightOffset = 5.0f;
            activeLightPos.x = 0.0f;
            activeLightPos.y = sin(moonAngle) * moonRadius - moonHeightOffset;
            activeLightPos.z = cos(moonAngle) * moonRadius;
            ambientColor = glm::vec3(0.1f, 0.1f, 0.2f);
            diffuseColor = glm::vec3(0.3f, 0.4f, 0.6f);
            specularColor = glm::vec3(0.3f, 0.4f, 0.6f);
            lightModelColor = glm::vec3(0.6f, 0.7f, 0.8f);
        }

        // Set lights properties
        glUniform3f(glGetUniformLocation(shader.Program, "light.position"), activeLightPos.x, activeLightPos.y, activeLightPos.z);
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), specularColor.x, specularColor.y, specularColor.z);
        glUniform1f(glGetUniformLocation(shader.Program, "shininess"), 32.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        
        // Objetos del escenario que van a recibir luz
        glUniform1i(glGetUniformLocation(shader.Program, "isLightSource"), 0); // Le decimos al shader que no es fuente de luz
        glUniform3f(glGetUniformLocation(shader.Program, "objectColor"), 1.0f, 1.0f, 1.0f);

        // Draw the loaded model
        glm::mat3 normalMatrix;
        glm::mat4 model(1);
        
        // Perro
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.2f, -2.0f, -0.6f));
        model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.8f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        dog.Draw(shader);

        // Nave
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.2f, -2.0f, -0.6f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(NAVE_SCALE * GLOBAL_SCALE * 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        nave.Draw(shader);

        // Tierra 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-8.0f, 5.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 1.0f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        tierra.Draw(shader);

        // Saturno
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.8f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        saturno.Draw(shader);

        // Jupiter
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 1.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.6f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        jupiter.Draw(shader);

        // Estrellas
        for (unsigned int i = 0; i < starPositions.size(); i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, starPositions[i]);
            model = glm::scale(model, glm::vec3(1.2f * GLOBAL_SCALE));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
            glUniformMatrix3fv(glGetUniformLocation(shader.Program, "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
            star.Draw(shader);
        }

        // Sol y Luna
        glUniform1i(glGetUniformLocation(shader.Program, "isLightSource"), 1); // Le decimos al shader que sí es fuente de luz
        glUniform3f(glGetUniformLocation(shader.Program, "lightColor"), lightModelColor.x, lightModelColor.y, lightModelColor.z);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, activeLightPos);
        if (dayMode) {
            model = glm::scale(model, glm::vec3(2.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            sol.Draw(shader);
        } else {
            model = glm::scale(model, glm::vec3(1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            luna.Draw(shader);
        }
        
        // Swap the buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
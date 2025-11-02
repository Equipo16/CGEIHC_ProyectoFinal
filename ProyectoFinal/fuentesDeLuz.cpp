//Práctica 9
//Sandra Laparra Miranda
//Fecha de entrega: 19 de octubre de 2025
//Número de cuenta: 311243563

#include <iostream>
#include <cmath>

#include <vector>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

using std::string;
using std::to_string;

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
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

const int NUM_POINT_LIGHTS = 17;

// Variables de control de luz para los tres modelos satelite, estrellas, nave
bool spotlightActive = true;
bool satelliteLightActive = true;
bool starLightsActive = true;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 Sandra Laparra", nullptr, nullptr);
    
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

    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders y Modelos
    Shader shader("Shader/lighting.vs", "Shader/lighting.frag");
    Model tierra((char*)"Models/Tierra.obj");
    Model saturno((char*)"Models/Saturno.obj");
    Model jupiter((char*)"Models/Jupiter.obj");
    Model nave((char*)"Models/nave.obj");
    Model star((char*)"Models/star.obj");
    Model satelite((char*)"Models/satelite.obj");

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    const float GLOBAL_SCALE = 0.02f;
    const float NAVE_SCALE = 0.06f;
    const float PLANET_SCALE = 0.35f;
    const float PLANET_Z = -30.0f;

    glm::vec3 tierraPos = glm::vec3(-8.0f, 5.0f, PLANET_Z);
    glm::vec3 satelitePos = glm::vec3(-10.0f, 10.0f, PLANET_Z);

    glm::vec3 pointLightPositions[NUM_POINT_LIGHTS] = {
        glm::vec3(-20.0f, 18.0f, -28.0f), glm::vec3(0.0f, 20.0f, -27.0f),
        glm::vec3(20.0f, 17.0f, -29.0f), glm::vec3(-15.0f, 16.0f, -31.0f),
        glm::vec3(15.0f, 10.0f, -33.0f), glm::vec3(-10.0f, 8.0f, -30.0f),
        glm::vec3(5.0f, -5.0f, -25.0f),  glm::vec3(-25.0f, 0.0f, -35.0f),
        glm::vec3(25.0f, 5.0f, -32.0f),  glm::vec3(-22.0f, -8.0f, -26.0f),
        glm::vec3(0.0f, -15.0f, -28.0f), glm::vec3(18.0f, -10.0f, -30.0f),
        glm::vec3(-18.0f, -12.0f, -33.0f), glm::vec3(12.0f, 12.0f, -24.0f),
        glm::vec3(-5.0f, 15.0f, -35.0f), glm::vec3(22.0f, -2.0f, -29.0f),
        satelitePos
    };

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

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glUniform3f(glGetUniformLocation(shader.Program, "globalAmbient"), 0.15f, 0.15f, 0.15f);

        // Luz de las estrellas tipo puntual
        for (int i = 0; i < NUM_POINT_LIGHTS - 1; i++) {
            string number = to_string(i);
            glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].position").c_str()), 1, &pointLightPositions[i][0]);
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].linear").c_str()), 0.014f);
            glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.0007f);
            glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].ambient").c_str()), 0.1f, 0.1f, 0.1f);

            if (starLightsActive) {
                glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].diffuse").c_str()), 0.8f, 0.8f, 0.6f);
                glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
            }
            else {
                glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].diffuse").c_str()), 0.0f, 0.0f, 0.0f);
                glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + number + "].specular").c_str()), 0.0f, 0.0f, 0.0f);
            }
        }

        // Luz del satelite tipo puntual
        string sateliteLightIndex = to_string(NUM_POINT_LIGHTS - 1);
        glUniform3fv(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].position").c_str()), 1, &pointLightPositions[NUM_POINT_LIGHTS - 1][0]);
        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].constant").c_str()), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].linear").c_str()), 0.027f);
        glUniform1f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].quadratic").c_str()), 0.0028f);
        glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].ambient").c_str()), 0.1f, 0.1f, 0.1f);
        if (satelliteLightActive) {
            glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].diffuse").c_str()), 80.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].specular").c_str()), 1.0f, 1.0f, 1.0f);
        }
        else {
            glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].diffuse").c_str()), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, ("pointLights[" + sateliteLightIndex + "].specular").c_str()), 0.0f, 0.0f, 0.0f);
        }

        // Luz del motor de la nave espacial tipo reflector o spot
        glm::vec3 navePos = glm::vec3(5.2f, -2.0f, -0.6f);
        glm::vec3 spotLightPos = navePos + glm::vec3(0.0f, 0.0f, 0.4f);
        glm::vec3 spotLightDir = glm::normalize(tierraPos - spotLightPos);

        glUniform3fv(glGetUniformLocation(shader.Program, "spotLight.position"), 1, &spotLightPos[0]);
        glUniform3fv(glGetUniformLocation(shader.Program, "spotLight.direction"), 1, &spotLightDir[0]);
        glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), 0.1f, 0.08f, 0.0f);

        if (spotlightActive) {
            glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 2.0f, 1.5f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
        }
        else {
            glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        }

        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), 0.007f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), 0.0002f);
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(15.0f)));
        glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(22.5f)));

        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);

        // Dibujo de los modelos
        glm::mat4 model(1);
        model = glm::mat4(1);
        model = glm::translate(model, navePos);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(NAVE_SCALE * GLOBAL_SCALE * 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nave.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, tierraPos);
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 1.0f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tierra.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.8f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        saturno.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 1.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.6f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        jupiter.Draw(shader);

        for (int i = 0; i < NUM_POINT_LIGHTS - 1; i++) {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(1.2f * GLOBAL_SCALE));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            star.Draw(shader);
        }

        model = glm::mat4(1);
        model = glm::translate(model, satelitePos);
        model = glm::scale(model, glm::vec3(0.2f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        satelite.Draw(shader);


        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Tecla "L" para apagar/encender la luz del motor de la nave
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        spotlightActive = !spotlightActive;
    }

    // Tecla "P" para apagar/encender la luz del satélite
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        satelliteLightActive = !satelliteLightActive;
    }

    // Tecla "O" para apagar/encender la luz de las estrellas
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        starLightsActive = !starLightsActive;
    }
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
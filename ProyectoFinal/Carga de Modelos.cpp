//Práctica 6
//Sandra Laparra Miranda
//Fecha de entrega: 21 de septiembre de 2025
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

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 6 Sandra Laparra", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent( window );

    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    // OpenGL options
    glEnable( GL_DEPTH_TEST );

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");

    // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model tierra((char*)"Models/Tierra.obj");
    Model saturno((char*)"Models/Saturno.obj");
    Model jupiter((char*)"Models/Jupiter.obj");
    Model nave((char*)"Models/nave.obj");
    Model star((char*)"Models/star.obj");   

    //glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 100.0f); // para ver los modelos más de cerca

    // Constantes para escalas y posiciones en el eje Z para cada objeto
    const float GLOBAL_SCALE = 0.02f;
    const float DOG_SCALE = 6.0f;
    const float DOG_Z = -0.6f;
    const float NAVE_SCALE = 0.06f;
    const float NAVE_Z = -6.0f;
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

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Cambio de color a negro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1); //matriz base

        // Perro
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.2f, -2.0f, -0.6f));
        model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.8f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);

        // Nave
        model = glm::mat4(1);
        float naveX = 3.2f + 2.0f;
        float naveY = -2.0f;
        float naveZ = -0.6f;
        model = glm::translate(model, glm::vec3(naveX, naveY, naveZ));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-12.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(NAVE_SCALE * GLOBAL_SCALE * 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nave.Draw(shader);

        // Tierra 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-8.0f, 5.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 1.0f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tierra.Draw(shader);

        // Saturno 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.8f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        saturno.Draw(shader);

        // Jupiter
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(8.0f, 1.0f, PLANET_Z));
        model = glm::scale(model, glm::vec3(PLANET_SCALE * 0.6f * GLOBAL_SCALE));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        jupiter.Draw(shader);

        // Estrellas
        for (unsigned int i = 0; i < starPositions.size(); i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, starPositions[i]);
            model = glm::scale(model, glm::vec3(1.2f * GLOBAL_SCALE)); 
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            star.Draw(shader);
        }


        // Swap the buffers
        glfwSwapBuffers( window );
    }

    glfwTerminate( );
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




}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

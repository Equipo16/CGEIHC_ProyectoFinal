//Proyecto final
//Autores: 422132844, 311243563
//Fecha de entrega: 12 de noviembre de 2025

#include <iostream>
#include <cmath>
#include <fstream>  
#include <vector>
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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
bool cargarModelo(const std::string& path, Model*& modelo);
void Animation(GLfloat deltaTime);

// keyframes momia
void AnimationMomia();
void saveFrameMomia(void);
void resetElementsMomia(void);
void interpolationMomia(void);
void saveFramesMomia(const char* filename);
void loadFramesMomia(const char* filename);


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


const int NR_POINT_LIGHTS = 17;
glm::vec3 pointLightPositions[] = {
    // Sala 1 (Egipcia - cálida) 
    glm::vec3(30.0f, 5.0f, 5.0f),
    glm::vec3(30.0f, 5.0f, -5.0f),
    glm::vec3(40.0f, 5.0f, 5.0f),
    glm::vec3(40.0f, 5.0f, -5.0f),

    // Sala 2 (Arte - blanca) 
    glm::vec3(-10.0f, 5.0f, 10.0f),
    glm::vec3(-10.0f, 5.0f, 20.0f),
    glm::vec3(-20.0f, 5.0f, 10.0f),
    glm::vec3(-20.0f, 5.0f, 20.0f),

    // Sala 3 (Historia - azulada) 
    glm::vec3(-10.0f, 5.0f, -10.0f),
    glm::vec3(-10.0f, 5.0f, -20.0f),
    glm::vec3(-20.0f, 5.0f, -10.0f),
    glm::vec3(-20.0f, 5.0f, -20.0f),

    // Sala 4 (Reloj - verde) 
    glm::vec3(-35.0f, 5.0f, 5.0f),
    glm::vec3(-35.0f, 5.0f, -5.0f),
    glm::vec3(-45.0f, 5.0f, 5.0f),
    glm::vec3(-45.0f, 5.0f, -5.0f),

    // Luz central/general 
    glm::vec3(0.0f, 2.0f, 0.0f) 
};

glm::vec3 pointLightColors[] = {
    // Sala 1 (Cálida)
    glm::vec3(1.0f, 0.8f, 0.6f),
    glm::vec3(1.0f, 0.8f, 0.6f),
    glm::vec3(1.0f, 0.8f, 0.6f),
    glm::vec3(1.0f, 0.8f, 0.6f),

    // Sala 2 (Blanca)
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),

    // Sala 3 (Azulada)
    glm::vec3(0.7f, 0.7f, 1.0f),
    glm::vec3(0.7f, 0.7f, 1.0f),
    glm::vec3(0.7f, 0.7f, 1.0f),
    glm::vec3(0.7f, 0.7f, 1.0f),

    // Sala 4 (Verde)
    glm::vec3(0.7f, 1.0f, 0.7f),
    glm::vec3(0.7f, 1.0f, 0.7f),
    glm::vec3(0.7f, 1.0f, 0.7f),
    glm::vec3(0.7f, 1.0f, 0.7f),

    // Luz central (variable)
    glm::vec3(1.0f, 1.0f, 1.0f) 
};



// Variables para animación de manecillas (Reloj)
bool animacionActiva = false;
float anguloHorario = 0.0f;
float anguloMinutero = 0.0f;

// Variables para el péndulo (Reloj)
float anguloPendulo = 0.0f;
float amplitudPendulo = 5.0f;
bool penduloDerecha = true;
glm::vec3 pivotManecillas(0.460781f, 2.10701f, 0.006229f);
glm::vec3 pivotPendulo(0.295819f, 1.60216f, 0.006229f);

// Variables para engranajes (Reloj)
float anguloEng1 = 0.0f;
float anguloEng2 = 0.0f;
glm::vec3 pivotEng1(0.150284f, 1.14464f, -0.375564f);
glm::vec3 pivotEng2(0.151259f, 0.477735f, -0.321072f);

// Variables para campana y badajo (Reloj)
float anguloCampana = 0.0f;
float escalaCampana = 1.0f;
bool campanaSonando = false;
float tiempoCampana = 0.0f;
glm::vec3 pivotCampana(0.168226f, 2.9f, 0.0f);

// Traslación global para todos los modelos (Reloj)
glm::vec3 traslacionGlobal(-42.6714f, 0.58425f, -0.088454f);



bool animSarcofago = false;
glm::vec3 sarcofagoPos = glm::vec3(37.0f, 2.8f, 0.0f);
float tapaPosCerrado = 0.0f;
float tapaPosAbierto = 0.5f;
float tapaPosX = tapaPosCerrado;
float tapaAnimSpeed = 0.0002f;
float tapaPosBaseZ = 0.2f;
float tapaLevitateAmount = 0.3f;
float tapaPosZ = tapaPosBaseZ;
float tapaShakeAmount = 0.008f;
int animState = 0;
float mummyMoveSpeed = 0.001f;
float mummyWalkTargetDist = 1.0f;
float mummyWalkDistance = 0.0f;
float stateTimer = 0.0f;
float mummyArmSpeed = 1.0f;
float mummyArmMaxAngle = 90.0f;
bool mummyArmWavingUp = true;



// keyframes variables globales
glm::vec3 momiaPos = glm::vec3(37.0f, 2.8f, 0.0f);
float momiaRotY = 270.0f;
float momiaEscala = 1.0f;
float momiaBrazoL_X = -75.0f;
float momiaBrazoL_Y = 35.0f;
float momiaBrazoR_X = -75.0f;
float momiaBrazoR_Y = -35.0f;
float momiaPiernaL_X = 0.0f;
float momiaPiernaR_X = 0.0f;

#define MAX_FRAMES_MOMIA 10
int i_max_steps_momia = 600;
int i_curr_steps_momia = 0;

typedef struct _frame_momia {
    // Variables de estado
    glm::vec3 momiaPos;
    float momiaRotY;
    float momiaEscala;
    float momiaBrazoL_X;
    float momiaBrazoL_Y;
    float momiaBrazoR_X;
    float momiaBrazoR_Y;
    float momiaPiernaL_X;
    float momiaPiernaR_X;
    // Variables de incremento
    glm::vec3 momiaPosInc;
    float momiaRotYInc;
    float momiaEscalaInc;
    float momiaBrazoL_XInc;
    float momiaBrazoL_YInc;
    float momiaBrazoR_XInc;
    float momiaBrazoR_YInc;
    float momiaPiernaL_XInc;
    float momiaPiernaR_XInc;
} FRAME_MOMIA;

FRAME_MOMIA KeyFrameMomia[MAX_FRAMES_MOMIA];
int FrameIndexMomia = 0;
bool playMomia = false;
int playIndexMomia = 0;




struct DirLightLocations {
    GLint direction;
    GLint ambient;
    GLint diffuse;
    GLint specular;
};

struct PointLightLocations {
    GLint position;
    GLint ambient;
    GLint diffuse;
    GLint specular;
    GLint constant;
    GLint linear;
    GLint quadratic;
};

struct SpotLightLocations {
    GLint position;
    GLint direction;
    GLint cutOff;
    GLint outerCutOff;
    GLint constant;
    GLint linear;
    GLint quadratic;
    GLint ambient;
    GLint diffuse;
    GLint specular;
};

// Array global para guardar las ubicaciones de las 17 luces
PointLightLocations pointLightLocs[NR_POINT_LIGHTS];
// Variables globales para las otras luces
DirLightLocations dirLightLocs;
SpotLightLocations spotLightLocs;



// Estructura para manejar modelos 
struct ModeloInfo {
    std::string nombre;
    Model* modelo;
    bool cargado;
};

std::vector<ModeloInfo> modelos;
std::vector<ModeloInfo> modelosCargados;

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

// Vértices del Skybox
GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f
};

// Índices para el cubo (lámpara)
GLuint indices[] =
{
    0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19, 20,21,22,23, 24,25,26,27, 28,29,30,31, 32,33,34,35
};


glm::vec3 Light1 = glm::vec3(0); // Variable para animar el color de la luz 17

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Función para cargar modelos con manejo de errores
bool cargarModelo(const std::string& path, Model*& modelo) {
    try {
        modelo = new Model((char*)path.c_str());
        std::cout << "Modelo cargado: " << path << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error cargando modelo: " << path << " - " << e.what() << std::endl;
        modelo = nullptr;
        return false;
    }
}

// Función para inicializar la lista de modelos
void inicializarListaModelos() {
    // Lista de todos los modelos a cargar
    std::vector<std::string> listaModelos = {
        "eg1", "eg2", "eg3", "eg4", "eg5", "eg6", "eg7", "eg8", "eg9", "eg10",
        "eg11", "eg12", "eg13", "eg14", "eg15", "eg16", "eg17", "eg18", "eg19", "eg20",
        "eg21", "eg22", "esc1", "esc2", "esc3", "esc4", "esc5", "esc6", "esc7", "esc8",
        "esc9", "esc10", "esc11", "esc12", "esc13", "esc14", "esc15", "esc16", "esc17",
        "pir", "puertas1", "puertas2", "puertas3", "puertas4", "sala1", "sala2", "sala3", "sala4",
        "ac1", "ac2", "ac3", "ac4", "ac5", "ac6", "ac7", "ac8", "ac9", "ac10",
        "ac11", "ac12", "ac13", "ac14", "ac15", "ac16", "ac17", "ac18", "ac19", "ac20",
        "ac21", "ac22", "ac23", "ac24", "ac25", "ac26", "ac27", "ac28",
        "cont", "cont1", "cont2", "cont3", "cont4", "cont5", "cont6", "cont7", "cont8", "cont9",
        "cont10", "cont11", "cont12", "cont13", "cont14", "cont15", "cont16", "cont17", "cont18", "cont19",
        "cont20", "cont21", "cont22", "cont23", "cont24", "cont25", "cont26", "cont27", "cont28", "cont29", "cont30"
    };

    for (const auto& nombre : listaModelos) {
        modelos.push_back({ nombre, nullptr, false });
    }
}

// Función para cargar un lote de modelos
void cargarLoteModelos(int inicio, int fin) {
    std::cout << "Cargando lote de modelos " << inicio << " a " << fin << std::endl;

    for (int i = inicio; i <= fin && i < modelos.size(); i++) {
        if (!modelos[i].cargado) {
            std::string path = "Models/" + modelos[i].nombre + ".obj";
            if (cargarModelo(path, modelos[i].modelo)) {
                modelos[i].cargado = true;
                modelosCargados.push_back(modelos[i]);
            }
        }
    }
}

int main()
{
    // Init GLFW
    glfwInit();

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
    glewExperimental = GL_TRUE;
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

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

    // Inicializar lista de modelos 
    inicializarListaModelos();

    // Cargar modelos 
    std::cout << "Cargando modelos críticos..." << std::endl;
    // Modelos Reloj
    Model* reloj = nullptr;
    Model* Piso = nullptr;
    Model* Pend = nullptr;
    Model* min = nullptr;
    Model* hor = nullptr;
    Model* Eng1 = nullptr;
    Model* Eng2 = nullptr;
    Model* Campana = nullptr;
    Model* Badajo = nullptr;
    // Modelos Momia/Sarcófago
    Model* MummyBody = nullptr;
    Model* MummyArmL = nullptr;
    Model* MummyArmR = nullptr;
    Model* MummyLegR = nullptr;
    Model* MummyLegL = nullptr;
    Model* Sarcofago = nullptr;
    Model* TapaSarcofago = nullptr;


    if (!cargarModelo("Models/relojp.obj", reloj) ||
        !cargarModelo("Models/piso.obj", Piso) ||
        !cargarModelo("Models/pend.obj", Pend) ||
        !cargarModelo("Models/minutero.obj", min) ||
        !cargarModelo("Models/horario.obj", hor) ||
        !cargarModelo("Models/eng1.obj", Eng1) ||
        !cargarModelo("Models/eng2.obj", Eng2) ||
        !cargarModelo("Models/camp.obj", Campana) ||
        !cargarModelo("Models/bad.obj", Badajo) ||
        !cargarModelo("Models/cuerpo.obj", MummyBody) ||
        !cargarModelo("Models/brazo_izq.obj", MummyArmL) ||
        !cargarModelo("Models/brazo_der.obj", MummyArmR) ||
        !cargarModelo("Models/pierna_der.obj", MummyLegR) ||
        !cargarModelo("Models/pierna_izq.obj", MummyLegL) ||
        !cargarModelo("Models/sarcofago.obj", Sarcofago) ||
        !cargarModelo("Models/tapa_sarcofago.obj", TapaSarcofago)
        ) {
        std::cout << "Error cargando modelos críticos!" << std::endl;
        return EXIT_FAILURE;
    }

    // Cargar primeros 20 modelos adicionales 
    cargarLoteModelos(0, 19);

    // Variables para carga progresiva
    int loteActual = 0;
    const int MODELOS_POR_LOTE = 20;
    float tiempoUltimaCarga = 0.0f;
    bool todosModelosCargados = false;


    
    for (int i = 0; i < MAX_FRAMES_MOMIA; i++)
    {
        KeyFrameMomia[i].momiaPos = glm::vec3(0.0f);
        KeyFrameMomia[i].momiaRotY = 0.0f;
        KeyFrameMomia[i].momiaEscala = 0.0f;
        KeyFrameMomia[i].momiaBrazoL_X = 0.0f;
        KeyFrameMomia[i].momiaBrazoL_Y = 0.0f;
        KeyFrameMomia[i].momiaBrazoR_X = 0.0f;
        KeyFrameMomia[i].momiaBrazoR_Y = 0.0f;
        KeyFrameMomia[i].momiaPiernaL_X = 0.0f;
        KeyFrameMomia[i].momiaPiernaR_X = 0.0f;

        KeyFrameMomia[i].momiaPosInc = glm::vec3(0.0f);
        KeyFrameMomia[i].momiaRotYInc = 0.0f;
        KeyFrameMomia[i].momiaEscalaInc = 0.0f;
        KeyFrameMomia[i].momiaBrazoL_XInc = 0.0f;
        KeyFrameMomia[i].momiaBrazoL_YInc = 0.0f;
        KeyFrameMomia[i].momiaBrazoR_XInc = 0.0f;
        KeyFrameMomia[i].momiaBrazoR_YInc = 0.0f;
        KeyFrameMomia[i].momiaPiernaL_XInc = 0.0f;
        KeyFrameMomia[i].momiaPiernaR_XInc = 0.0f;
    }
    KeyFrameMomia[0].momiaPos = momiaPos;
    KeyFrameMomia[0].momiaRotY = momiaRotY;
    KeyFrameMomia[0].momiaEscala = 1.0f;
    KeyFrameMomia[0].momiaBrazoL_X = -75.0f;
    KeyFrameMomia[0].momiaBrazoL_Y = 35.0f;
    KeyFrameMomia[0].momiaBrazoR_X = -75.0f;
    KeyFrameMomia[0].momiaBrazoR_Y = -35.0f;
    KeyFrameMomia[0].momiaPiernaL_X = 0.0f;
    KeyFrameMomia[0].momiaPiernaR_X = 0.0f;
    KeyFrameMomia[1].momiaPos = momiaPos;
    KeyFrameMomia[1].momiaRotY = momiaRotY;
    KeyFrameMomia[1].momiaEscala = 1.0f;
    KeyFrameMomia[1].momiaBrazoL_X = -90.0f;
    KeyFrameMomia[1].momiaBrazoL_Y = 0.0f;
    KeyFrameMomia[1].momiaBrazoR_X = -90.0f;
    KeyFrameMomia[1].momiaBrazoR_Y = 0.0f;
    KeyFrameMomia[1].momiaPiernaL_X = 0.0f;
    KeyFrameMomia[1].momiaPiernaR_X = 0.0f;
    KeyFrameMomia[2] = KeyFrameMomia[1];
    KeyFrameMomia[3] = KeyFrameMomia[2];
    KeyFrameMomia[3].momiaBrazoR_X = -70.0f;
    KeyFrameMomia[4] = KeyFrameMomia[3];
    KeyFrameMomia[5] = KeyFrameMomia[2];
    KeyFrameMomia[6] = KeyFrameMomia[0];
    KeyFrameMomia[7] = KeyFrameMomia[0];
    FrameIndexMomia = 8;
    resetElementsMomia();
    


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


    
    std::cout << "Buscando ubicaciones de uniformes de luces..." << std::endl;
    lightingShader.Use(); 

    // 1. Directional Light
    dirLightLocs.direction = glGetUniformLocation(lightingShader.Program, "dirLight.direction");
    dirLightLocs.ambient = glGetUniformLocation(lightingShader.Program, "dirLight.ambient");
    dirLightLocs.diffuse = glGetUniformLocation(lightingShader.Program, "dirLight.diffuse");
    dirLightLocs.specular = glGetUniformLocation(lightingShader.Program, "dirLight.specular");

    // 2. Point Lights (Loop de 17)
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        std::string i_str = std::to_string(i);

        pointLightLocs[i].position = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].position").c_str());
        pointLightLocs[i].ambient = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].ambient").c_str());
        pointLightLocs[i].diffuse = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].diffuse").c_str());
        pointLightLocs[i].specular = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].specular").c_str());
        pointLightLocs[i].constant = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].constant").c_str());
        pointLightLocs[i].linear = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].linear").c_str());
        pointLightLocs[i].quadratic = glGetUniformLocation(lightingShader.Program, ("pointLights[" + i_str + "].quadratic").c_str());

        // Comprobación
        if (i == 0 && (pointLightLocs[i].position == -1 || dirLightLocs.direction == -1)) {
            std::cout << "¡¡ERROR!! No se encontraron uniforms basicos de luz." << std::endl;
            std::cout << "Asegurate que 'lighting.frag' este usando 'dirLight' y 'pointLights'." << std::endl;
        }
    }

    // 3. Spot Light
    spotLightLocs.position = glGetUniformLocation(lightingShader.Program, "spotLight.position");
    spotLightLocs.direction = glGetUniformLocation(lightingShader.Program, "spotLight.direction");
    spotLightLocs.cutOff = glGetUniformLocation(lightingShader.Program, "spotLight.cutOff");
    spotLightLocs.outerCutOff = glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff");
    spotLightLocs.constant = glGetUniformLocation(lightingShader.Program, "spotLight.constant");
    spotLightLocs.linear = glGetUniformLocation(lightingShader.Program, "spotLight.linear");
    spotLightLocs.quadratic = glGetUniformLocation(lightingShader.Program, "spotLight.quadratic");
    spotLightLocs.ambient = glGetUniformLocation(lightingShader.Program, "spotLight.ambient");
    spotLightLocs.diffuse = glGetUniformLocation(lightingShader.Program, "spotLight.diffuse");
    spotLightLocs.specular = glGetUniformLocation(lightingShader.Program, "spotLight.specular");

    std::cout << "Ubicaciones de luces cargadas." << std::endl;
    


    // Skybox
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
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // Carga progresiva de modelos
        if (!todosModelosCargados && currentFrame - tiempoUltimaCarga > 1.0f) {
            int inicio = loteActual * MODELOS_POR_LOTE;
            if (inicio < modelos.size()) {
                int fin = inicio + MODELOS_POR_LOTE - 1;
                if (fin >= modelos.size()) fin = modelos.size() - 1;
                cargarLoteModelos(inicio, fin);
                loteActual++;
                tiempoUltimaCarga = currentFrame;
                std::cout << "Progreso: " << modelosCargados.size() << "/" << modelos.size() << " modelos cargados" << std::endl;
            }
            else {
                todosModelosCargados = true;
                std::cout << "Todos los modelos han sido cargados!" << std::endl;
            }
        }

        Animation(deltaTime);       // Animación de Sarcófago y Reloj
        AnimationMomia();           // Animación de Keyframes Momia


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelTemp = glm::mat4(1.0f);
        glm::mat4 sarcofagoModelTemp = glm::mat4(1.0f);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0); // Material

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos"); // Posición de la cámara
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        

        // 1. Directional light
        glUniform3f(dirLightLocs.direction, -0.2f, -1.0f, -0.3f);
        glUniform3f(dirLightLocs.ambient, 0.6f, 0.6f, 0.6f);
        glUniform3f(dirLightLocs.diffuse, 0.6f, 0.6f, 0.6f);
        glUniform3f(dirLightLocs.specular, 0.3f, 0.3f, 0.3f);

        // 2. Point Lights (Loop de 17)
        for (int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            // Definir color para la luz
            glm::vec3 lightColor;
            if (i == 16) { // Luz 17 (índice 16) es la animada y movible
                lightColor.x = abs(sin(glfwGetTime() * Light1.x));
                lightColor.y = abs(sin(glfwGetTime() * Light1.y));
                lightColor.z = sin(glfwGetTime() * Light1.z);
            }
            else {
                lightColor = pointLightColors[i];
            }

            // Atenuación
            float linear = (i == 16) ? 0.045f : 0.09f;
            float quadratic = (i == 16) ? 0.075f : 0.032f;
            float ambientFactor = (i == 16) ? 1.0f : 0.1f; // Luces de sala más sutiles

            // Usamos las ubicaciones guardadas en el array pointLightLocs
            glUniform3f(pointLightLocs[i].position, pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(pointLightLocs[i].ambient, lightColor.x * ambientFactor, lightColor.y * ambientFactor, lightColor.z * ambientFactor);
            glUniform3f(pointLightLocs[i].diffuse, lightColor.x, lightColor.y, lightColor.z);
            glUniform3f(pointLightLocs[i].specular, 1.0f, 1.0f, 1.0f); // Specular blanco para todas
            glUniform1f(pointLightLocs[i].constant, 1.0f);
            glUniform1f(pointLightLocs[i].linear, linear);
            glUniform1f(pointLightLocs[i].quadratic, quadratic);
        }

        // 3. SpotLight
        glUniform3f(spotLightLocs.position, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(spotLightLocs.direction, camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(spotLightLocs.ambient, 0.2f, 0.2f, 0.8f);
        glUniform3f(spotLightLocs.diffuse, 0.2f, 0.2f, 0.8f);
        glUniform3f(spotLightLocs.specular, 0.0f, 0.0f, 0.0f);
        glUniform1f(spotLightLocs.constant, 1.0f);
        glUniform1f(spotLightLocs.linear, 0.3f);
        glUniform1f(spotLightLocs.quadratic, 0.7f);
        glUniform1f(spotLightLocs.cutOff, glm::cos(glm::radians(12.0f)));
        glUniform1f(spotLightLocs.outerCutOff, glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);
        

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar piso
        glm::mat4 modelPiso = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiso));
        Piso->Draw(lightingShader);

        
        for (const auto& modeloInfo : modelosCargados) {
            if (modeloInfo.cargado && modeloInfo.modelo) {
                glm::mat4 model = glm::mat4(1.0f);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                modeloInfo.modelo->Draw(lightingShader);
            }
        }

        //Reloj
        glm::mat4 modelReloj = glm::mat4(1.0f);
        modelReloj = glm::translate(modelReloj, traslacionGlobal);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelReloj));
        reloj->Draw(lightingShader);

        glm::mat4 modelPend = glm::mat4(1.0f);
        modelPend = glm::translate(modelPend, traslacionGlobal);
        modelPend = glm::translate(modelPend, pivotPendulo);
        modelPend = glm::rotate(modelPend, glm::radians(anguloPendulo), glm::vec3(1.0f, 0.0f, 0.0f));
        modelPend = glm::translate(modelPend, -pivotPendulo);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPend));
        Pend->Draw(lightingShader);

        glm::mat4 modelMin = glm::mat4(1.0f);
        modelMin = glm::translate(modelMin, traslacionGlobal);
        modelMin = glm::translate(modelMin, pivotManecillas);
        modelMin = glm::rotate(modelMin, glm::radians(anguloMinutero), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMin = glm::translate(modelMin, -pivotManecillas);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMin));
        min->Draw(lightingShader);

        glm::mat4 modelHor = glm::mat4(1.0f);
        modelHor = glm::translate(modelHor, traslacionGlobal);
        modelHor = glm::translate(modelHor, pivotManecillas);
        modelHor = glm::rotate(modelHor, glm::radians(anguloHorario), glm::vec3(1.0f, 0.0f, 0.0f));
        modelHor = glm::translate(modelHor, -pivotManecillas);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHor));
        hor->Draw(lightingShader);

        glm::mat4 modelEng1_mat = glm::mat4(1.0f);
        modelEng1_mat = glm::translate(modelEng1_mat, traslacionGlobal);
        modelEng1_mat = glm::translate(modelEng1_mat, pivotEng1);
        modelEng1_mat = glm::rotate(modelEng1_mat, glm::radians(anguloEng1), glm::vec3(0.0f, 0.0f, 1.0f));
        modelEng1_mat = glm::translate(modelEng1_mat, -pivotEng1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng1_mat));
        Eng1->Draw(lightingShader);

        glm::mat4 modelEng2_mat = glm::mat4(1.0f);
        modelEng2_mat = glm::translate(modelEng2_mat, traslacionGlobal);
        modelEng2_mat = glm::translate(modelEng2_mat, pivotEng2);
        modelEng2_mat = glm::rotate(modelEng2_mat, glm::radians(anguloEng2), glm::vec3(0.0f, 0.0f, 1.0f));
        modelEng2_mat = glm::translate(modelEng2_mat, -pivotEng2);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEng2_mat));
        Eng2->Draw(lightingShader);

        glm::mat4 modelCampana_mat = glm::mat4(1.0f);
        modelCampana_mat = glm::translate(modelCampana_mat, traslacionGlobal);
        modelCampana_mat = glm::translate(modelCampana_mat, pivotCampana);
        modelCampana_mat = glm::rotate(modelCampana_mat, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCampana_mat = glm::scale(modelCampana_mat, glm::vec3(escalaCampana, escalaCampana, escalaCampana));
        modelCampana_mat = glm::translate(modelCampana_mat, -pivotCampana);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCampana_mat));
        Campana->Draw(lightingShader);

        glm::mat4 modelBadajo_mat = glm::mat4(1.0f);
        modelBadajo_mat = glm::translate(modelBadajo_mat, traslacionGlobal);
        modelBadajo_mat = glm::translate(modelBadajo_mat, pivotCampana);
        modelBadajo_mat = glm::rotate(modelBadajo_mat, glm::radians(anguloCampana), glm::vec3(1.0f, 0.0f, 0.0f));
        modelBadajo_mat = glm::scale(modelBadajo_mat, glm::vec3(escalaCampana, escalaCampana, escalaCampana));
        modelBadajo_mat = glm::translate(modelBadajo_mat, -pivotCampana);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBadajo_mat));
        Badajo->Draw(lightingShader);
        


        // Sarcofago
        sarcofagoModelTemp = glm::mat4(1);
        sarcofagoModelTemp = glm::translate(sarcofagoModelTemp, sarcofagoPos);
        sarcofagoModelTemp = glm::rotate(sarcofagoModelTemp, glm::radians(momiaRotY), glm::vec3(0.0f, 1.0f, 0.0f));
        


        // Dinujo de momia
        glm::mat4 model(1);

        model = glm::translate(model, momiaPos);
        model = glm::rotate(model, glm::radians(momiaRotY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(momiaEscala, momiaEscala, momiaEscala));
        modelTemp = model;

        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        MummyBody->Draw(lightingShader);

        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.08f, -0.9f, 0.09f));
        model = glm::rotate(model, glm::radians(momiaPiernaL_X), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        MummyLegL->Draw(lightingShader);

        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.1f, -0.9f, 0.0f));
        model = glm::rotate(model, glm::radians(momiaPiernaR_X), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        MummyLegR->Draw(lightingShader);

        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.24f, 0.15f, 0.0f));
        model = glm::rotate(model, glm::radians(momiaBrazoL_X), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(momiaBrazoL_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        MummyArmL->Draw(lightingShader);

        
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.24f, 0.15f, 0.0f));
        model = glm::rotate(model, glm::radians(momiaBrazoR_X), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(momiaBrazoR_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        MummyArmR->Draw(lightingShader);
        


        // Dibujo de sarcofago
        model = sarcofagoModelTemp;
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Sarcofago->Draw(lightingShader);

        float shakeOffsetY = 0.0f;
        if (animState == 1 || animState == 5)
        {
            shakeOffsetY = sinf(currentFrame * 10.0f) * tapaShakeAmount;
        }

        model = sarcofagoModelTemp;
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::translate(model, glm::vec3(tapaPosX, -0.1f + shakeOffsetY, tapaPosZ));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        TapaSarcofago->Draw(lightingShader);
        


        // 17 lámparas
        glBindVertexArray(0);

        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO); 
        for (int i = 0; i < NR_POINT_LIGHTS; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0); 
        


        // SkyBox
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
        glActiveTexture(GL_TEXTURE0);
        


        glfwSwapBuffers(window);
    }

    // Limpieza
    delete reloj;
    delete Piso;
    delete Pend;
    delete min;
    delete hor;
    delete Eng1;
    delete Eng2;
    delete Campana;
    delete Badajo;
    delete MummyBody;
    delete MummyArmL;
    delete MummyArmR;
    delete MummyLegR;
    delete MummyLegL;
    delete Sarcofago;
    delete TapaSarcofago;

    for (auto& modeloInfo : modelos) {
        if (modeloInfo.modelo) {
            delete modeloInfo.modelo;
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);


    glfwTerminate();
    return 0;
}


void DoMovement()
{
    // controles para momia
    if (keys[GLFW_KEY_1])
    {
        momiaBrazoL_X += 0.1f;
        std::cout << "momiaBrazoL_X: " << momiaBrazoL_X << std::endl;
    }
    if (keys[GLFW_KEY_2])
    {
        momiaBrazoL_X -= 0.1f;
        std::cout << "momiaBrazoL_X: " << momiaBrazoL_X << std::endl;
    }
    if (keys[GLFW_KEY_3])
    {
        momiaPos.x += 0.01f;
        std::cout << "momiaPos.x: " << momiaPos.x << std::endl;
    }
    if (keys[GLFW_KEY_4])
    {
        momiaPos.x -= 0.01f;
        std::cout << "momiaPos.x: " << momiaPos.x << std::endl;
    }
    


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

    // Controles de luz (PointLight 17, índice 16)
    if (keys[GLFW_KEY_T]) { pointLightPositions[16].x += 0.01f; }
    if (keys[GLFW_KEY_G]) { pointLightPositions[16].x -= 0.01f; }
    if (keys[GLFW_KEY_Y]) { pointLightPositions[16].y += 0.01f; }
    if (keys[GLFW_KEY_H]) { pointLightPositions[16].y -= 0.01f; }
    if (keys[GLFW_KEY_U]) { pointLightPositions[16].z -= 0.1f; }
    if (keys[GLFW_KEY_J]) { pointLightPositions[16].z += 0.01f; }
}


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    
    if (action == GLFW_PRESS)
    {
        // Tecla L: Keyframes
        if (key == GLFW_KEY_L)
        {
            if (playMomia == false && (FrameIndexMomia > 1)) {
                if (animSarcofago) {
                    animSarcofago = false;
                    animState = 0;
                    std::cout << "Animacion Sarcofago (N) detenida." << std::endl;
                }
                std::cout << "Iniciando animacion de Keyframes Momia (L)" << std::endl;
                resetElementsMomia();
                interpolationMomia();
                playMomia = true;
                playIndexMomia = 0;
                i_curr_steps_momia = 0;
            }
            else {
                std::cout << "Deteniendo animacion de Keyframes Momia (L)" << std::endl;
                playMomia = false;
            }
        }

        // Tecla K: Guardar Frame
        if (key == GLFW_KEY_K)
        {
            if (FrameIndexMomia < MAX_FRAMES_MOMIA)
            {
                std::cout << "Guardando frame " << FrameIndexMomia << std::endl;
                saveFrameMomia();
            }
            else
            {
                std::cout << "Maximo de frames alcanzado" << std::endl;
            }
        }

        // Tecla P: Guardar Archivo
        if (key == GLFW_KEY_P)
        {
            saveFramesMomia("keyframes_momia.txt");
        }

        // Tecla O: Cargar Archivo
        if (key == GLFW_KEY_O)
        {
            loadFramesMomia("keyframes_momia.txt");
        }

        // Tecla N: Sarcófago
        if (key == GLFW_KEY_N)
        {
            if (animState == 0) {
                if (playMomia) {
                    playMomia = false;
                    std::cout << "Animacion Keyframes (L) detenida." << std::endl;
                }
                std::cout << "Iniciando animacion Sarcofago (N)" << std::endl;
                animSarcofago = true;
                animState = 1;
            }
        }

        // Tecla R: Reloj
        if (key == GLFW_KEY_R)
        {
            animacionActiva = !animacionActiva;
        }

        // Tecla SPACE: Luz (animación de color de la luz 17)
        if (key == GLFW_KEY_SPACE)
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
    }

    // Lógica de Teclas Sostenidas (para DoMovement) 
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
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}



// Keyframes de momia
void saveFrameMomia(void)
{
    KeyFrameMomia[FrameIndexMomia].momiaPos = momiaPos;
    KeyFrameMomia[FrameIndexMomia].momiaRotY = momiaRotY;
    KeyFrameMomia[FrameIndexMomia].momiaEscala = momiaEscala;
    KeyFrameMomia[FrameIndexMomia].momiaBrazoL_X = momiaBrazoL_X;
    KeyFrameMomia[FrameIndexMomia].momiaBrazoL_Y = momiaBrazoL_Y;
    KeyFrameMomia[FrameIndexMomia].momiaBrazoR_X = momiaBrazoR_X;
    KeyFrameMomia[FrameIndexMomia].momiaBrazoR_Y = momiaBrazoR_Y;
    KeyFrameMomia[FrameIndexMomia].momiaPiernaL_X = momiaPiernaL_X;
    KeyFrameMomia[FrameIndexMomia].momiaPiernaR_X = momiaPiernaR_X;
    FrameIndexMomia++;
}

void resetElementsMomia(void)
{
    momiaPos = KeyFrameMomia[0].momiaPos;
    momiaRotY = KeyFrameMomia[0].momiaRotY;
    momiaEscala = KeyFrameMomia[0].momiaEscala;
    momiaBrazoL_X = KeyFrameMomia[0].momiaBrazoL_X;
    momiaBrazoL_Y = KeyFrameMomia[0].momiaBrazoL_Y;
    momiaBrazoR_X = KeyFrameMomia[0].momiaBrazoR_X;
    momiaBrazoR_Y = KeyFrameMomia[0].momiaBrazoR_Y;
    momiaPiernaL_X = KeyFrameMomia[0].momiaPiernaL_X;
    momiaPiernaR_X = KeyFrameMomia[0].momiaPiernaR_X;
}

void interpolationMomia(void)
{
    KeyFrameMomia[playIndexMomia].momiaPosInc = (KeyFrameMomia[playIndexMomia + 1].momiaPos - KeyFrameMomia[playIndexMomia].momiaPos) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaRotYInc = (KeyFrameMomia[playIndexMomia + 1].momiaRotY - KeyFrameMomia[playIndexMomia].momiaRotY) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaEscalaInc = (KeyFrameMomia[playIndexMomia + 1].momiaEscala - KeyFrameMomia[playIndexMomia].momiaEscala) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaBrazoL_XInc = (KeyFrameMomia[playIndexMomia + 1].momiaBrazoL_X - KeyFrameMomia[playIndexMomia].momiaBrazoL_X) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaBrazoL_YInc = (KeyFrameMomia[playIndexMomia + 1].momiaBrazoL_Y - KeyFrameMomia[playIndexMomia].momiaBrazoL_Y) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaBrazoR_XInc = (KeyFrameMomia[playIndexMomia + 1].momiaBrazoR_X - KeyFrameMomia[playIndexMomia].momiaBrazoR_X) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaBrazoR_YInc = (KeyFrameMomia[playIndexMomia + 1].momiaBrazoR_Y - KeyFrameMomia[playIndexMomia].momiaBrazoR_Y) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaPiernaL_XInc = (KeyFrameMomia[playIndexMomia + 1].momiaPiernaL_X - KeyFrameMomia[playIndexMomia].momiaPiernaL_X) / (float)i_max_steps_momia;
    KeyFrameMomia[playIndexMomia].momiaPiernaR_XInc = (KeyFrameMomia[playIndexMomia + 1].momiaPiernaR_X - KeyFrameMomia[playIndexMomia].momiaPiernaR_X) / (float)i_max_steps_momia;
}

void AnimationMomia()
{
    if (playMomia)
    {
        if (i_curr_steps_momia >= i_max_steps_momia)
        {
            playIndexMomia++;
            if (playIndexMomia > FrameIndexMomia - 2)
            {
                std::cout << "Termina animacion momia (L)" << std::endl;
                playIndexMomia = 0;
                playMomia = false;
                resetElementsMomia();
            }
            else
            {
                i_curr_steps_momia = 0;
                interpolationMomia();
            }
        }
        else
        {
            momiaPos += KeyFrameMomia[playIndexMomia].momiaPosInc;
            momiaRotY += KeyFrameMomia[playIndexMomia].momiaRotYInc;
            momiaEscala += KeyFrameMomia[playIndexMomia].momiaEscalaInc;
            momiaBrazoL_X += KeyFrameMomia[playIndexMomia].momiaBrazoL_XInc;
            momiaBrazoL_Y += KeyFrameMomia[playIndexMomia].momiaBrazoL_YInc;
            momiaBrazoR_X += KeyFrameMomia[playIndexMomia].momiaBrazoR_XInc;
            momiaBrazoR_Y += KeyFrameMomia[playIndexMomia].momiaBrazoR_YInc;
            momiaPiernaL_X += KeyFrameMomia[playIndexMomia].momiaPiernaL_XInc;
            momiaPiernaR_X += KeyFrameMomia[playIndexMomia].momiaPiernaR_XInc;
            i_curr_steps_momia++;
        }
    }
}

void saveFramesMomia(const char* filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo " << filename << " para escribir." << std::endl;
        return;
    }
    outFile << FrameIndexMomia << std::endl;
    for (int i = 0; i < FrameIndexMomia; i++) {
        outFile << KeyFrameMomia[i].momiaPos.x << " " << KeyFrameMomia[i].momiaPos.y << " " << KeyFrameMomia[i].momiaPos.z << " ";
        outFile << KeyFrameMomia[i].momiaRotY << " ";
        outFile << KeyFrameMomia[i].momiaEscala << " ";
        outFile << KeyFrameMomia[i].momiaBrazoL_X << " " << KeyFrameMomia[i].momiaBrazoL_Y << " ";
        outFile << KeyFrameMomia[i].momiaBrazoR_X << " " << KeyFrameMomia[i].momiaBrazoR_Y << " ";
        outFile << KeyFrameMomia[i].momiaPiernaL_X << " " << KeyFrameMomia[i].momiaPiernaR_X << std::endl;
    }
    outFile.close();
    std::cout << "¡Keyframes de momia guardados en " << filename << "!" << std::endl;
}

void loadFramesMomia(const char* filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cout << "Error: No se pudo abrir el archivo " << filename << " para leer." << std::endl;
        return;
    }
    int framesToLoad = 0;
    inFile >> framesToLoad;
    if (framesToLoad > MAX_FRAMES_MOMIA) {
        std::cout << "Error: El archivo tiene más frames (" << framesToLoad
            << ") que el máximo permitido (" << MAX_FRAMES_MOMIA << ")" << std::endl;
        inFile.close();
        return;
    }
    for (int i = 0; i < framesToLoad; i++) {
        inFile >> KeyFrameMomia[i].momiaPos.x >> KeyFrameMomia[i].momiaPos.y >> KeyFrameMomia[i].momiaPos.z;
        inFile >> KeyFrameMomia[i].momiaRotY;
        inFile >> KeyFrameMomia[i].momiaEscala;
        inFile >> KeyFrameMomia[i].momiaBrazoL_X >> KeyFrameMomia[i].momiaBrazoL_Y;
        inFile >> KeyFrameMomia[i].momiaBrazoR_X >> KeyFrameMomia[i].momiaBrazoR_Y;
        inFile >> KeyFrameMomia[i].momiaPiernaL_X >> KeyFrameMomia[i].momiaPiernaR_X;
    }
    FrameIndexMomia = framesToLoad;
    inFile.close();
    resetElementsMomia();
    std::cout << "¡Keyframes de momia cargados desde " << filename << "!" << std::endl;
}



// Función Animation (Sarcófago y Reloj) 
void Animation(GLfloat deltaTime) {

    // Máquina de Estados Sarcófago (anim 'N')
    if (animSarcofago)
    {
        float ratio = 0.0f;
        switch (animState)
        {
        case 1: // Abriendo Tapa
            tapaPosX += tapaAnimSpeed;
            ratio = tapaPosX / tapaPosAbierto;
            tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
            if (tapaPosX > tapaPosAbierto) {
                tapaPosX = tapaPosAbierto;
                tapaPosZ = tapaPosBaseZ;
                animState = 2;
            }
            break;
        case 2: // Momia caminando hacia adelante
        {
            momiaPos.x += sinf(glm::radians(momiaRotY)) * mummyMoveSpeed;
            momiaPos.z += cosf(glm::radians(momiaRotY)) * mummyMoveSpeed;
            mummyWalkDistance += mummyMoveSpeed;
            float walkRatio = mummyWalkDistance / mummyWalkTargetDist;
            momiaEscala = 1.0f + (walkRatio * (1.4f - 1.0f));
            if (momiaEscala > 1.4f) momiaEscala = 1.4f;
            if (mummyWalkDistance > mummyWalkTargetDist) {
                momiaEscala = 1.4f;
                animState = 3;
                stateTimer = 0.0f;
            }
        }
        break;
        case 3: // Momia en pausa
            stateTimer += deltaTime;
            if (stateTimer > 3.0f) {
                animState = 4;
            }
            break;
        case 4: // Momia caminando hacia atrás
        {
            momiaPos.x -= sinf(glm::radians(momiaRotY)) * mummyMoveSpeed;
            momiaPos.z -= cosf(glm::radians(momiaRotY)) * mummyMoveSpeed;
            mummyWalkDistance -= mummyMoveSpeed;
            float walkRatioBack = mummyWalkDistance / mummyWalkTargetDist;
            momiaEscala = 1.0f + (walkRatioBack * (1.4f - 1.0f));
            if (momiaEscala < 1.0f) momiaEscala = 1.0f;
            if (mummyWalkDistance < 0.0f) {
                mummyWalkDistance = 0.0f;
                momiaPos = sarcofagoPos;
                momiaEscala = 1.0f;
                animState = 5;
            }
        }
        break;
        case 5: // Cerrando Tapa
            tapaPosX -= tapaAnimSpeed;
            ratio = tapaPosX / tapaPosAbierto;
            tapaPosZ = tapaPosBaseZ + (sinf(ratio * 3.14159f) * tapaLevitateAmount);
            if (tapaPosX < tapaPosCerrado) {
                tapaPosX = tapaPosCerrado;
                tapaPosZ = tapaPosBaseZ;
                animState = 0;
                animSarcofago = false;
                resetElementsMomia();
            }
            break;
        }
    }

    // Animación Reloj (anim 'R')
    if (animacionActiva)
    {
        anguloHorario += 30.0f * deltaTime;
        anguloMinutero += 360.0f * deltaTime;
        if (anguloHorario >= 360.0f) anguloHorario -= 360.0f;
        if (anguloMinutero >= 360.0f) anguloMinutero -= 360.0f;
        float oscilacion = sin(glfwGetTime() * 2.0f) * amplitudPendulo;
        anguloPendulo = oscilacion;
        anguloEng1 += 180.0f * deltaTime;
        anguloEng2 -= 90.0f * deltaTime;
        if (anguloEng1 >= 360.0f) anguloEng1 -= 360.0f;
        if (anguloEng2 <= -360.0f) anguloEng2 += 360.0f;

        static float tiempoDesdeUltimaCampana = 0.0f;
        tiempoDesdeUltimaCampana += deltaTime;

        if (tiempoDesdeUltimaCampana >= 5.0f) {
            campanaSonando = true;
            tiempoCampana = 0.0f;
            tiempoDesdeUltimaCampana = 0.0f;
        }

        if (campanaSonando) {
            tiempoCampana += deltaTime;
            if (tiempoCampana < 2.0f) {
                anguloCampana = sin(tiempoCampana * 40.0f) * 8.0f;
                escalaCampana = 1.0f + sin(tiempoCampana * 30.0f) * 0.2f;
            }
            else {
                campanaSonando = false;
                anguloCampana = 0.0f;
                escalaCampana = 1.0f;
            }
        }
    }
}


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>

const unsigned int NO_SPOTLIGHTS = 18;
const unsigned int NO_POINTLIGHTS = 3;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat4 lightRotation;

glm::mat4 cubeModel;
glm::mat4 waterModel;
glm::mat4 discoModel;
glm::mat4 cubeNormalMatrix;
glm::mat4 waterNormalMatrix;
glm::mat4 discoNormalMatrix;
glm::mat4 lightModel;
glm::mat4 discoLightsModel;

glm::vec3 cameraPosition;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor = glm::vec3(0.4f, 0.4f, 0.4f);
glm::mat4 lightSpaceTrMatrix;


glm::vec3 pointLightPositions[] =
{
    glm::vec3(-47.5f, 10.0f, 4.77f),
    glm::vec3(-23.72f, 11.8f, 25.04f),
    glm::vec3(-37.2f, 15.13f, -0.36f),
};
glm::vec3 pointLightColors[] =
{
    glm::vec3(0.0f, 0.7f, 0.5f),
    glm::vec3(0.85f, 0.28f, 0.72f),
    glm::vec3(0.77f, 0.59f, 0.90f)
};
GLfloat pointLightConsts[] =
{
    0.7f,
    0.7f,
    0.7f
};
GLfloat pointLightLinears[] =
{
    0.09f,
    0.15f,
    0.15f
};
GLfloat pointLightQuads[] =
{
    0.032f,
    0.052f,
    0.052f
};

glm::vec3 spotLightPositions[] =
{
    glm::vec3(-30.0f, 8.0f, 28.5f),
    glm::vec3(-28.7f, 13.0f, 9.5f), //over the disco ball
    glm::vec3(-28.7f, 6.0f, 9.5f), //under the disco ball
    glm::vec3(-40.1f, 14.35f, -6.48f), //overhead1
    glm::vec3(-16.24f, 13.7f, 22.3f), //overhead2
    glm::vec3(-15.64f, 11.75f, -8.76f), //overhead3
    glm::vec3(-15.64f, 11.75f, -8.76f), //overhead4
    glm::vec3(-0.71f, 8.57f, 10.48f), //room

    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_1
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_2
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_3
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_4
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_5
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_6
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_7
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_8
    glm::vec3(-28.67f, 11.00f, 9.5f), //discoball_9
    glm::vec3(-28.67f, 11.00f, 9.5f) //discoball_10

};
glm::vec3 spotLightDirections[] =
{
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f), //over discoball
    glm::vec3(0.0f, -1.0f, 0.0f), //under discoball
    glm::vec3(0.59f, -0.67f, 0.43f), //overhead1
    glm::vec3(-0.43f, -0.16f, -0.66f), //overhead2
    glm::vec3(-0.56f, -0.39f, 0.56f), //overhead3
    glm::vec3(-0.67f, -0.15f, 0.72f), //overhead4
    glm::vec3(-0.08f, -0.24f, -0.75f),  //room

    glm::vec3(0.0f, 0.0f, 1.0f),        //discoball_1
    glm::vec3(-0.84f, -0.53f, -0.015f),  //discoball_2
    glm::vec3(-0.94f, -0.26f, -0.20f),  //discoball_3
    glm::vec3(-0.60f, -0.09f, 0.8f),  //discoball_4
    glm::vec3(-0.08f, 0.33f, 0.93f),  //discoball_5
    glm::vec3(0.92f, -0.17f, 0.32f),  //discoball_6
    glm::vec3(0.85f, -0.55f, -0.17f),  //discoball_7
    glm::vec3(0.83f, -0.11f, -0.54f),  //discoball_8
    glm::vec3(0.81f, 0.41f, -0.41f),  //discoball_9
    glm::vec3(0.48f, -0.09f, -0.87f)  //discoball_10
};

glm::vec3 spotLightColors[] =
{
    glm::vec3(1.0f, 0.2f, 0.2f),
    glm::vec3(1.0f, 1.0f, 1.0f), //over discoball
    glm::vec3(0.4f, 0.0f, 1.0f), //under discoball
    glm::vec3(1.0f, 0.0f, 0.0f), //overhead1
    glm::vec3(0.0f, 1.0f, 0.0f), //overhead2
    glm::vec3(0.0f, 0.0f, 1.0f), //overhead3
    glm::vec3(1.0f, 0.0f, 1.0f), //overhead4
    glm::vec3(1.0f, 0.0f, 1.0f), //room

    glm::vec3(0.0f, 1.0f, 0.0f), //discoball_1
    glm::vec3(1.0f, 1.0f, 0.0f), //discoball_2
    glm::vec3(0.0f, 1.0f, 1.0f), //discoball_3
    glm::vec3(1.0f, 0.0f, 0.0f), //discoball_4
    glm::vec3(1.0f, 0.0f, 1.0f), //discoball_5
    glm::vec3(0.0f, 0.0f, 0.0f), //discoball_6
    glm::vec3(0.0f, 0.0f, 1.0f), //discoball_7
    glm::vec3(0.0f, 0.5f, 1.0f), //discoball_8
    glm::vec3(0.2f, 0.5f, 1.0f), //discoball_9
    glm::vec3(0.0f, 1.0f, 1.0f) //discoball_10
};
GLfloat spotLightConsts[] =
{
    0.7f,
    0.7f, //over discoball
    0.7f, //under discoball
    0.7f, //overhead1
    0.7f, //overhead2
    0.7f, //overhead3
    0.7f, //overhead4
    0.7f, //room

    0.5f, //discoball_1
    0.5f, //discoball_2
    0.5f, //discoball_3
    0.5f, //discoball_4
    0.5f, //discoball_5
    0.5f, //discoball_6
    0.5f, //discoball_7
    0.5f, //discoball_8
    0.5f, //discoball_9
    0.5f //discoball_10
};
GLfloat spotLightLinears[] =
{
    0.009f,
    0.009f, //over discoball
    0.009f, //under discoball
    0.002f, //overhead1
    0.008f, //overhead2
    0.002f, //overhead3
    0.002f, //overhead4
    0.009f, //room

    0.0004f, //discoball_1
    0.0004f, //discoball_2
    0.0004f, //discoball_3
    0.0004f, //discoball_4
    0.0004f, //discoball_5
    0.0004f, //discoball_6
    0.0004f, //discoball_7
    0.0004f, //discoball_8
    0.0004f, //discoball_9
    0.0004f //discoball_10
};
GLfloat spotLightQuads[] =
{
    0.0032f,
    0.0032f, //over discoball
    0.0032f, //under discoball
    0.0002f, //overhead1
    0.0005f, //overhead2
    0.0002f, //overhead3
    0.0002f, //overhead4
    0.0032f, //room

    0.00012f, //discoball_1
    0.00012f, //discoball_2
    0.00012f, //discoball_3
    0.00012f, //discoball_4
    0.00012f, //discoball_5
    0.00012f, //discoball_6
    0.00012f, //discoball_7
    0.00012f, //discoball_8
    0.00012f, //discoball_9
    0.00012f //discoball_10
};
GLfloat spotCutoffs[] =
{
    glm::cos(glm::radians(12.5f)),
    glm::cos(glm::radians(12.5f)), //over discoball
    glm::cos(glm::radians(12.5f)), //under discoball
    glm::cos(glm::radians(32.5f)), //overhead1
    glm::cos(glm::radians(12.5f)), //overhead2
    glm::cos(glm::radians(32.5f)), //overhead3
    glm::cos(glm::radians(7.5f)), //overhead4
    glm::cos(glm::radians(22.5f)), //room

    glm::cos(glm::radians(6.5f)), //discoball_1
    glm::cos(glm::radians(6.5f)), //discoball_2
    glm::cos(glm::radians(6.5f)), //discoball_3
    glm::cos(glm::radians(6.5f)), //discoball_4
    glm::cos(glm::radians(6.5f)), //discoball_5
    glm::cos(glm::radians(6.5f)), //discoball_6
    glm::cos(glm::radians(6.5f)), //discoball_7
    glm::cos(glm::radians(6.5f)), //discoball_8
    glm::cos(glm::radians(6.5f)), //discoball_9
    glm::cos(glm::radians(6.5f)) //discoball_10
};
GLfloat spotOuterCutoffs[] =
{
    glm::cos(glm::radians(13.5f)),
    glm::cos(glm::radians(27.5f)), //over discoball
    glm::cos(glm::radians(13.5f)), //under discoball
    glm::cos(glm::radians(33.5f)), //overhead1
    glm::cos(glm::radians(13.5f)), //overhead2
    glm::cos(glm::radians(33.5f)), //overhead3
    glm::cos(glm::radians(8.5f)), //overhead4
    glm::cos(glm::radians(23.5f)), //room

    glm::cos(glm::radians(7.5f)), //discoball_1
    glm::cos(glm::radians(7.5f)), //discoball_2
    glm::cos(glm::radians(7.5f)), //discoball_3
    glm::cos(glm::radians(7.5f)), //discoball_4
    glm::cos(glm::radians(7.5f)), //discoball_5
    glm::cos(glm::radians(7.5f)), //discoball_6
    glm::cos(glm::radians(7.5f)), //discoball_7
    glm::cos(glm::radians(7.5f)), //discoball_8
    glm::cos(glm::radians(7.5f)), //discoball_9
    glm::cos(glm::radians(7.5f)) //discoball_10
};


// shader uniform locations
GLuint modelLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;
GLuint lightSpaceTrMatrixLoc;


// camera
gps::Camera myCamera(
    glm::vec3(-23.2f, 2.72f, -12.9f),
    glm::vec3(-23.2f, 2.72f, -11.9f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.1f;
GLfloat lightAngle = 45.0f;

GLboolean pressedKeys[1024];

// models
gps::Model3D map;
gps::Model3D cube1;
gps::Model3D discoball;
gps::Model3D testCube;
gps::Model3D lightCube;
gps::Model3D water;
GLfloat angle;
float yaw = 1.5f;
float pitch = 0;

// shaders
gps::Shader myBasicShader;
gps::Shader lightShader;
gps::Shader depthMapShader;
gps::Shader waterShader;
gps::Shader currentShader;

// mouse position
float lastX = 400, lastY = 300;
bool firstMouse = true;
float sensitivity = 0.01f;

int retina_width;
int retina_height;

short int something;
float cubeHeight;
GLfloat cubeAngle;
GLfloat discoAngle;

GLuint shadowMapFBO;
GLuint depthMapTexture;
bool disco;
bool isPresenting;
int currLight = 1;
bool moves[15];
float localLime;

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void animateCamera() 
{
    if (moves[0]) {
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        if (myCamera.getPosition().x < -28.0f) {
            moves[0] = false;
            moves[1] = true;
        }
    }
    else
        if (moves[1]) {
            myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
            if (myCamera.getPosition().z > 2.0f) {
                moves[1] = false;
                moves[2] = true;
            }
        }
        else if (moves[2]) {
            if (yaw < 3.0f) yaw += 0.01;
            if (pitch < 0.7) pitch += 0.01;

            if (yaw > 3.0f && pitch > 0.7) {
                moves[2] = false;
                moves[3] = true;
            }
        }
        else if (moves[3]) {
            yaw -= 0.01;
            if (yaw < 1.7f) {
                moves[3] = false;
                moves[4] = true;
            }
        }
        else if (moves[4]) {
            pitch -= 0.01;
            if (pitch < -0.3) {
                moves[4] = false;
                moves[5] = true;
                localLime = glfwGetTime();
            }
        }
        else if (moves[5]) {
            if (glfwGetTime() - localLime > 1.0f) {
                moves[5] = false;
                moves[6] = true;
            }
        }
        else if (moves[6]) {
            pitch += 0.01;
            if (pitch > 0.03) {
                moves[6] = false;
                moves[7] = true;
            }
        }
        else if (moves[7]) {
            if (yaw > 1.4f) {
                yaw -= 0.005;
            }
            if (myCamera.getPosition().z > -15.0f) {
                myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
            }
            if (yaw < 1.4f && myCamera.getPosition().z < -15.0f) {
                moves[7] = false;
                isPresenting = false;
            }
        }

    myCamera.rotate(pitch, yaw);
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
    //for RETINA display
    glfwGetFramebufferSize(myWindow.getWindow(), &retina_width, &retina_height);

    myBasicShader.useShaderProgram();

    //set projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    //send matrix data to shader
    GLint projLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    lightShader.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //set Viewport transform
    glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    myCamera.rotate(pitch, yaw);
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    waterShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    cubeNormalMatrix = glm::mat3(glm::inverseTranspose(view * cubeModel));
    waterNormalMatrix = glm::mat3(glm::inverseTranspose(view * waterModel));
    discoNormalMatrix = glm::mat3(glm::inverseTranspose(view * discoModel));
}

void processMovement() {
    if (pressedKeys[GLFW_KEY_W]) {
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_S]) {
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_A]) {
        myCamera.move(gps::MOVE_LEFT, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_D]) {
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_Q]) {
        yaw -= 1 * sensitivity;
        myCamera.rotate(pitch, yaw);
    }

    if (pressedKeys[GLFW_KEY_H]) {
        spotLightDirections[0].x += 0.03f;
    }

    if (pressedKeys[GLFW_KEY_J]) {
        spotLightDirections[0].x -= 0.03f;
    }

    if (pressedKeys[GLFW_KEY_K]) {
        spotLightDirections[0].z -= 0.03f;
        //pointLightPositions[currLight].x -= 0.3f;

    }

    if (pressedKeys[GLFW_KEY_L]) {
        spotLightDirections[0].z += 0.03f;
        //pointLightPositions[currLight].x += 0.3f;

    }

    if (pressedKeys[GLFW_KEY_U]) {
        spotLightPositions[0].x += 0.03f;
        //pointLightPositions[currLight].z += 0.3f;
    }

    if (pressedKeys[GLFW_KEY_I]) {
        spotLightPositions[0].x -= 0.03f;
        //pointLightPositions[currLight].z -= 0.3f;

    }

    if (pressedKeys[GLFW_KEY_SPACE]) {
        myCamera.moveUp(cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_ENTER]) {
        isPresenting = true;
    }

    if (pressedKeys[GLFW_KEY_R]) {
        isPresenting = false;
    }

    if (pressedKeys[GLFW_KEY_LEFT_SHIFT]) {
        myCamera.moveDown(cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_E]) {
        yaw += 1 * sensitivity;
        myCamera.rotate(pitch, yaw);
    }

    if (pressedKeys[GLFW_KEY_L]) {
        lightAngle += 1.0f;
    }
    if (pressedKeys[GLFW_KEY_M]) {
        disco = false;
    }
    if (pressedKeys[GLFW_KEY_N]) {
        disco = true;
    }
    if (pressedKeys[GLFW_KEY_P]) { //wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (pressedKeys[GLFW_KEY_O]) { //solid
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (pressedKeys[GLFW_KEY_U]) { //smooth
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    }

    if (pressedKeys[GLFW_KEY_Y]) { //polygonal
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
}

void initOpenGLWindow() {
    myWindow.Create(1920, 1080, "Portal Disco");
}

void setWindowCallbacks() {
    glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
    glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void initOpenGLState() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

    glEnable(GL_FRAMEBUFFER_SRGB);
}

void initModels() {
    map.LoadModel("models/map/maplevelcpy.obj"); //teapot is actually the map
    cube1.LoadModel("models/portal_cube/portalcube.obj");
    discoball.LoadModel("models/discoball/discoball.obj");
    testCube.LoadModel("models/portal_cube/portalcube.obj");
    water.LoadModel("models/water/wave.obj"); //doesn't load texture for some reason
    lightCube.LoadModel("objects/cube/cube.obj");
}

void initShaders() {
    myBasicShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
    //myBasicShader.loadShader("shaders/waterShader.vert", "shaders/waterShader.frag");
    lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
    depthMapShader.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
    waterShader.loadShader("shaders/waterShader.vert", "shaders/waterShader.frag");
}

void initFBO() {
    //generate FBO ID
    glGenFramebuffers(1, &shadowMapFBO);

    //generarea texturii de adancime
    //create depth texture for FBO
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //atasam textura creata mai sus la frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

    //aici nu am inteles f bine dar noi facem doar harta de adancime, nu ne folosim de culoara, asa ca vom marca ca nu le folosim
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    //dezactivam framebuffer ul pana la folosire (acum doar este creat)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

glm::mat4 computeLightSpaceTrMatrix() {
    glm::mat4 lightView = glm::lookAt(spotLightPositions[2], spotLightPositions[2] + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    const GLfloat near_plane = 0.1f, far_plane = 10.0f;
    glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
    return lightSpaceTrMatrix;
}

void initUniforms() {
    myBasicShader.useShaderProgram();

    model = glm::mat4(1.0f);
    modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // get view matrix for current camera
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix 
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // create projection matrix
    projection = glm::perspective(glm::radians(45.0f),
        (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
        0.1f, 1000.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    cameraPosition = myCamera.getPosition();
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "viewPos"), 1,
        glm::value_ptr(cameraPosition));


    //set the light direction (direction towards the light)
    //lightDir = glm::vec3(-14.6823f, 12.5882f, -9.686f); 
    lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
    glUniform3fv(lightDirLoc, 1, glm::value_ptr((glm::mat3(lightRotation)) * lightDir));


    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

    for (int i = 0; i < NO_POINTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightColors[" + no + "]").c_str()), 1, glm::value_ptr(pointLightColors[i]));
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightConsts[" + no + "]").c_str()), pointLightConsts[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightLinears[" + no + "]").c_str()), pointLightLinears[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightQuads[" + no + "]").c_str()), pointLightQuads[i]);
    }


    for (int i = 0; i < NO_SPOTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(spotLightPositions[i]));
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightDirections[" + no + "]").c_str()), 1, glm::value_ptr(spotLightDirections[i]));
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightColors[" + no + "]").c_str()), 1, glm::value_ptr(spotLightColors[i]));
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightConsts[" + no + "]").c_str()), spotLightConsts[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightLinears[" + no + "]").c_str()), spotLightLinears[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightQuads[" + no + "]").c_str()), spotLightQuads[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("spotCutoffs[" + no + "]").c_str()), spotCutoffs[i]);
        glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, ("spotOuterCutoffs[" + no + "]").c_str()), spotOuterCutoffs[i]);
    }
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "ambientStrength"), 0.05f);

    waterShader.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(waterModel));
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPosition));
    glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, "lightDir"), 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir));
    
    for (int i = 0; i < NO_POINTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("pointLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("pointLightColors[" + no + "]").c_str()), 1, glm::value_ptr(pointLightColors[i]));
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("pointLightConsts[" + no + "]").c_str()), pointLightConsts[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("pointLightLinears[" + no + "]").c_str()), pointLightLinears[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("pointLightQuads[" + no + "]").c_str()), pointLightQuads[i]);
    }

    for (int i = 0; i < NO_SPOTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("spotLightDirections[" + no + "]").c_str()), 1, glm::value_ptr(spotLightDirections[i]));
        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("spotLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(spotLightPositions[i]));
        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("spotLightColors[" + no + "]").c_str()), 1, glm::value_ptr(spotLightColors[i]));
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("spotLightConsts[" + no + "]").c_str()), spotLightConsts[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("spotLightLinears[" + no + "]").c_str()), spotLightLinears[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("spotLightQuads[" + no + "]").c_str()), spotLightQuads[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("spotCutoffs[" + no + "]").c_str()), spotCutoffs[i]);
        glUniform1f(glGetUniformLocation(waterShader.shaderProgram, ("spotOuterCutoffs[" + no + "]").c_str()), spotOuterCutoffs[i]);
    }
    //glUniform1f(glGetUniformLocation(waterShader.shaderProgram, "ambientStrength"), 0.05f);
}

void drawObjects(gps::Shader shader, bool depthPass) {
    view = myCamera.getViewMatrix();
    cameraPosition = myCamera.getPosition();

    shader.useShaderProgram();;
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(shader.shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPosition));
    waterShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPosition));


    waterModel = glm::translate(glm::mat4(1.0f), glm::vec3(-35.0f, -2.5f, 4.0f));
    waterModel = glm::scale(waterModel, glm::vec3(1.5f, 1.0f, 1.5f));

    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(waterModel));
    glUniformMatrix4fv(glGetUniformLocation(waterShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    if (!depthPass) {
        waterNormalMatrix = glm::mat3(glm::inverseTranspose(waterModel));
        glUniformMatrix3fv(glGetUniformLocation(waterShader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(waterNormalMatrix));
    }
    glUniform1f(glGetUniformLocation(waterShader.shaderProgram, "time"), glfwGetTime());
    water.Draw(waterShader);


    shader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(model));
        glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    map.Draw(shader);

    something++;
    cubeHeight = 0.7f + 0.5 * glm::sin(2 * 3.1415 * 0.003 * something);
    cubeAngle++;
    cubeModel = glm::translate(glm::mat4(1.0f), glm::vec3(-28.7f, cubeHeight, 9.5f));
    cubeModel = glm::rotate(cubeModel, glm::radians(0.5f * cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        cubeNormalMatrix = glm::mat3(glm::inverseTranspose(cubeModel));
        glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(cubeNormalMatrix));
    }
    cube1.Draw(shader);

    discoModel = glm::translate(glm::mat4(1.0f), glm::vec3(-28.7f, 10.0f, 9.5f));
    if(disco) discoModel = glm::rotate(discoModel, glm::radians(0.2f * cubeAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    discoModel = glm::scale(discoModel, glm::vec3(0.01f, 0.01f, 0.01f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(discoModel));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        discoNormalMatrix = glm::mat3(glm::inverseTranspose(discoModel));
        glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(discoNormalMatrix));
    }
    discoball.Draw(shader);
}

void positionLights() {
    myBasicShader.useShaderProgram();

    for (int i = 0; i < NO_POINTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("pointLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
    }

    for (int i = 0; i < NO_SPOTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(spotLightPositions[i]));
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightDirections[" + no + "]").c_str()), 1, glm::value_ptr(spotLightDirections[i]));
    }


    waterShader.useShaderProgram();

    for (int i = 0; i < NO_POINTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("pointLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(pointLightPositions[i]));
    }

    for (int i = 0; i < NO_SPOTLIGHTS; i++) {
        std::string no = std::to_string(i);

        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("spotLightDirections[" + no + "]").c_str()), 1, glm::value_ptr(spotLightDirections[i]));
        glUniform3fv(glGetUniformLocation(waterShader.shaderProgram, ("spotLightPositions[" + no + "]").c_str()), 1, glm::value_ptr(spotLightPositions[i]));
    }
}

void discoTime() {
    discoLightsModel = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    for (int i = 8; i < NO_SPOTLIGHTS; i++) {
        std::string no = std::to_string(i);

        spotLightDirections[i] = glm::normalize(glm::vec3(discoLightsModel * glm::vec4(spotLightDirections[i], 1.0f)));
        glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, ("spotLightDirections[" + no + "]").c_str()), 1, glm::value_ptr(spotLightDirections[i]));
    }
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "ambientStrength"), 0.005f);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (isPresenting)
    {
        animateCamera();
    }
    //positionLights();
    myBasicShader.useShaderProgram();
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "spotLightPositions[0]"), 1, glm::value_ptr(spotLightPositions[0]));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPositions[1]"), 1, glm::value_ptr(pointLightPositions[currLight]));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "spotLightDirections[0]"), 1, glm::value_ptr(spotLightDirections[0]));

    if(disco) discoTime();
    else glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "ambientStrength"), 0.05f);

    glm::vec3 cpos = myCamera.getPosition();
    glm::vec3 dir = myCamera.getFrontDirection();
    glm::vec3 target = myCamera.getTarget();

    //std::cout << "cpos" << cpos.x << " " << cpos.y << " " << cpos.z;
    //std::cout << " dir" << dir.x << " " << dir.y << " " << dir.z;
    //std::cout << " camTarget " << target.x << " " << target.y << " " << target.z << std::endl;

    //std::cout << " light " << pointLightPositions[currLight].x << " " << pointLightPositions[currLight].y << " " << pointLightPositions[currLight].z << std::endl;

    depthMapShader.useShaderProgram();
    lightSpaceTrMatrix = computeLightSpaceTrMatrix();
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceTrMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawObjects(depthMapShader, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //render
        glViewport(0, 0, (float)myWindow.getWindowDimensions().width, (float)myWindow.getWindowDimensions().height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myBasicShader.useShaderProgram();

        lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniform3fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightDir"), 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
        //glUniform3fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightDir2"), 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir2));

        //bind the shadow map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 3);

        glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(computeLightSpaceTrMatrix()));

        drawObjects(myBasicShader, false);

        lightShader.useShaderProgram();

        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        lightModel = glm::translate(glm::mat4(1.0f), spotLightPositions[0]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        lightCube.Draw(lightShader);
}

void cleanup() {
    //cleanup code for your own data
    glDeleteTextures(1, &depthMapTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &shadowMapFBO);
    myWindow.Delete();
}

int main(int argc, const char* argv[]) {

    try {
        initOpenGLWindow();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
    initModels();
    initShaders();
    initUniforms();
    initFBO();
    setWindowCallbacks();
    //PlaySound("music/portal_radio_tune.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    isPresenting = true;
    moves[0] = true;

    glCheckError();
    // application loop
    while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
        renderScene();
        glfwPollEvents();
        glfwSwapBuffers(myWindow.getWindow());
        glCheckError();
    }
    cleanup();
    return EXIT_SUCCESS;
}

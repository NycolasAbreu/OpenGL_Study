#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/gtc/matrix_transform.hpp"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

Camera camera;

// Mouse input variables
double lastX = WINDOW_WIDTH * 0.5f;
double lastY = WINDOW_HEIGHT * 0.5f;
bool firstMouse = true;

// Keyboard input variables
double deltaTime = 0.0f;
double lastFrame = 0.0f;

//--------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window);
GLFWwindow* InitContext();
void MeasureFps(int& nbFrames, double& lastTime, GLFWwindow* window);
void MeasureDeltaTime();
//--------------------------------------------------------------------------------------------

int main()
{
  GLFWwindow* window = InitContext();

  glEnable(GL_DEPTH_TEST);

  const Shader textureShader(RESOURCES_PATH "VertexShader.vs", RESOURCES_PATH "FragmentShader.fs");
  const Shader lightShader(RESOURCES_PATH "VertexLightShader.vs", RESOURCES_PATH "FragmentLightShader.fs");
  
  const Texture wallTexture(RESOURCES_PATH "WallTexture.jpg", JPG);
  const Texture smileTexture(RESOURCES_PATH "SmileTexture.png", PNG, /*flipTexture*/true);

  // Creating all the spaces
  glm::mat4 model(1.0f);
  glm::mat4 projection = glm::perspective(glm::radians((float)camera.GetZoom()),
                                          (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
  //glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 15.0f);

  const float vertices[] = {
        //    Vertices              Normal         TextureMap
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };
  
  unsigned int cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  VertexBuffer VBO(sizeof(vertices), vertices);
  //IndexBuffer EBO(sizeof(indices), indices);

  // (indice, tamanho, tipo, noramlizado, stride (tamanho total de todos os atributos), offset)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  VBO.Bind();
  // Only using the first attribute, but need to set the right stride size
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  // Seting uniform values for the textures, need to set the same value in the texture
  // don't forget to activate the shader before setting uniforms
  textureShader.Use();  
  textureShader.SetIntUniform("texture1", 0);
  textureShader.SetIntUniform("texture2", 1);

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // light positioning
    glm::vec3 lightPos(2 * sin(glfwGetTime()), 2 * cos(glfwGetTime()), 2 * cos(glfwGetTime()));

    // seting texture to the uniform variable in shader
    wallTexture.UseUnit(0);
    smileTexture.UseUnit(1);

    textureShader.Use();
    textureShader.SetVec3Uniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    textureShader.SetVec3Uniform("lightPos", lightPos);
    textureShader.SetVec3Uniform("viewPos", camera.GetPosition());

    // view/projection transformations
    textureShader.SetMat4Uniform("projection", projection);
    textureShader.SetMat4Uniform("view", camera.GetViewMatrix());

    // world transformation
    model = glm::mat4(1.0f);
    textureShader.SetMat4Uniform("model", model);
    textureShader.SetMat4Uniform("inverseModel", glm::inverse(model));

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the light cube object
    lightShader.Use();
    lightShader.SetMat4Uniform("projection", projection);
    lightShader.SetMat4Uniform("view", camera.GetViewMatrix());
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.5f));
    lightShader.SetMat4Uniform("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);

    MeasureDeltaTime();
    MeasureFps(nbFrames, lastTime, window);
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);

  glfwTerminate();
  return 0;
}

//--------------------------------------------------------------------------------------------

void MeasureDeltaTime()
{
  const double currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

//--------------------------------------------------------------------------------------------

void MeasureFps(
  int& nbFrames,
  double& lastTime,
  GLFWwindow* window
)
{
  const double currentTime = glfwGetTime();
  nbFrames++;
  if (currentTime - lastTime >= 1.0) {
    glfwSetWindowTitle(window, ("FPS: " + std::to_string(nbFrames)).c_str());
    nbFrames = 0;
    lastTime += 1.0;
  }
}

//--------------------------------------------------------------------------------------------

void framebuffer_size_callback(
  GLFWwindow* /*window*/,
  int width, int height
)
{
  glViewport(0, 0, width, height);
}

//--------------------------------------------------------------------------------------------

void mouse_callback(GLFWwindow* /*window*/, double xPos, double yPos)
{
  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  const double xOffset = xPos - lastX;
  const double yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

  lastX = xPos;
  lastY = yPos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

//--------------------------------------------------------------------------------------------

void scroll_callback(GLFWwindow* /*window*/, double /*xOffset*/, double yOffset)
{
  camera.ProcessMouseScroll(yOffset);
}

//--------------------------------------------------------------------------------------------

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

//--------------------------------------------------------------------------------------------

GLFWwindow* InitContext()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // Set the window callbacks functions
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  return window;
}

//--------------------------------------------------------------------------------------------

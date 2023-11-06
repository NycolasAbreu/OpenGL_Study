#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glm/gtc/matrix_transform.hpp"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

//--------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* InitContext();
void MeasureFps(int& nbFrames, double& lastTime, GLFWwindow* window);

//--------------------------------------------------------------------------------------------

int main()
{
  GLFWwindow* window = InitContext();

  glEnable(GL_DEPTH_TEST);

  const Shader simpleShader(RESOURCES_PATH "VertexShader.vs", RESOURCES_PATH "FragmentShader.fs");
  const Texture wallTexture(RESOURCES_PATH "WallTexture.jpg", JPG);
  const Texture smileTexture(RESOURCES_PATH "SmileTexture.png", PNG, /*flipTexture*/true);

  // Creating all the spaces
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);
  // note that we're translating the scene in the reverse direction of where we want to move
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
  //::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -10.0f, 15.0f);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  std::vector<glm::vec3> cubePositions = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  VertexBuffer VBO(sizeof(vertices), vertices);
  //IndexBuffer EBO(sizeof(indices), indices);

  // (indice, tamanho, tipo, noramlizado, stride (tamanho total de todos os atributos), offset)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Seting uniform values for the textures, need to set the same value in the texture
  // don't forget to activate the shader before setting uniforms
  simpleShader.Use();  
  simpleShader.SetIntUniform("texture1", 0);
  simpleShader.SetIntUniform("texture2", 1);

  double lastTime = glfwGetTime();
  int nbFrames = 0;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    // seting texture to the uniform variable in shader
    wallTexture.UseUnit(0);
    smileTexture.UseUnit(1);

    // Creating cubes in theirs positions and rotating them
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < cubePositions.size(); i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions.at(i));
      float angle = 20.0f * i + 10;
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      
      glm::mat4 mvp(projection * view * model);
      simpleShader.SetMat4Uniform("mvp", mvp);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);

    MeasureFps(nbFrames, lastTime, window);
  }

  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
  return 0;
}

//--------------------------------------------------------------------------------------------

void MeasureFps(
  int& nbFrames,
  double& lastTime,
  GLFWwindow* window
)
{
  double currentTime = glfwGetTime();
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

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}

//--------------------------------------------------------------------------------------------

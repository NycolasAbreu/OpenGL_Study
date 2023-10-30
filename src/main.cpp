#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Texture.h"
#include "Shader.h"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

//--------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* InitContext();

//--------------------------------------------------------------------------------------------

int main()
{
  GLFWwindow* window = InitContext();

  const Shader simpleShader(RESOURCES_PATH "VertexShader.vs", RESOURCES_PATH "FragmentShader.fs");
  const Texture wallTexture(RESOURCES_PATH "WallTexture.jpg", JPG);
  const Texture smileTexture(RESOURCES_PATH "SmileTexture.png", PNG, /*flipTexture*/true);

  float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 2,
      0, 2, 3
  };
  
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // (indice, tamanho, tipo, noramlizado, stride (tamanho total de todos os atributos), offset)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Seting uniform values for the textures, need to set the same value in the texture
  // don't forget to activate the shader before setting uniforms
  simpleShader.Use();  
  simpleShader.SetIntUniform("texture1", 0);
  simpleShader.SetIntUniform("texture2", 1);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // seting texture to the uniform variable in shader
    wallTexture.UseUnit(0);
    smileTexture.UseUnit(1);

    simpleShader.Use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // check and call events and swap the buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
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
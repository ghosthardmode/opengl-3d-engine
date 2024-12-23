#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "glad/glad.h"
#include "mesh.h"
#include <glfw/glfw3.h>

void update(GLFWwindow *window);
void render(GLFWwindow *window);
void mainLoop(GLFWwindow *window);
int getInput(GLFWwindow *window);
int init();

Mesh mesh;
GLuint vao, vbo, ebo, shaderProg;

int main() {
  return init();
}

int init() {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW\n");
    return 1;
  }
  //printf("%s\n", glfwGetVersionString());
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  static const int WINDOW_WIDTH = 1200;
  static const int WINDOW_HEIGHT = 640;
  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL!!!", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    glfwTerminate();
    return 1;
  }
  static const int VIEWPORT_X = 0;
  static const int VIEWPORT_Y = 0;
  glViewport(VIEWPORT_X, VIEWPORT_Y, WINDOW_WIDTH, WINDOW_HEIGHT);
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  compileShaderProgramFromPath(shaderProg, "../vertex.vert", "../fragment.frag");
  loadMeshStatic(vertices, vao, vbo, ebo);

  mainLoop(window);
  return 0;
}

void mainLoop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    render(window);
    update(window);
  }
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void update(GLFWwindow *window) {
  glfwPollEvents();
  if (getInput(window) == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, true);
  }
}

void render(GLFWwindow *window) {
  static const float RED = 0.3f;
  static const float GREEN = 0.4f;
  static const float BLUE = 0.5f;
  static const float ALPHA = 1.0f;
  glClearColor(RED, GREEN, BLUE, ALPHA);
  glClear(GL_COLOR_BUFFER_BIT);
  //render start
  drawMeshStatic(shaderProg, vao, 3);
  //render end
  glfwSwapBuffers(window);
}

int getInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    return GLFW_KEY_ESCAPE;
  }
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    int indices[] = {    };
    FILE *pVer = fopen("../vertex.vert", "r");
    FILE *pFrag = fopen("../fragment.frag", "r");
    if (pVer == NULL || pFrag == NULL) {
        printf("Error opening shader files\n");
        exit(1);
    }
    char vertexSrc[512];
    char fragmentSrc[512];
    for (int i = 0; i < 512; i++) {
        vertexSrc[i] = fgetc(pVer);
        if (vertexSrc[i] == EOF) {
            vertexSrc[i] = '\0';
            break;
        }
    }
    for (int i = 0; i < 512; i++) {
        fragmentSrc[i] = fgetc(pFrag);
        if (fragmentSrc[i] == EOF) {
            fragmentSrc[i] = '\0';
            break;
        }
    }
    fclose(pVer);
    fclose(pFrag);

    const GLchar *vertexShaderSrc = vertexSrc;
    const GLchar *fragmentShaderSrc = fragmentSrc;
    int wireframe = 0;
    GLuint VBO, VAO, EBO;
    GLuint shaderProg;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "test", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);
    int verSuccess, fragSuccess, progSuccess;
    char verInfoLog[512], fragInfoLog[512], progInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &verSuccess);
    if (!verSuccess) {
        glGetShaderInfoLog(vertexShader, 512, NULL, verInfoLog);
        printf("Error compiling vertex shader\n\n%s", verInfoLog);
        exit(1);
    }
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);
    if (!fragSuccess) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragInfoLog);
        printf("Error compiling fragment shader\n\n%s", fragInfoLog);
        exit(1);
    }
    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glGetProgramiv(shaderProg, GL_LINK_STATUS, &progSuccess);
    if (!progSuccess) {
        glGetProgramInfoLog(shaderProg, 512, NULL, progInfoLog);
        printf("Error linking shader\n\n%s", progInfoLog);
        exit(1);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(float));
        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            glfwWaitEventsTimeout(1.5);
            wireframe = !wireframe;
            //printf("wireframe = %d\n", wireframe);
        }
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProg);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

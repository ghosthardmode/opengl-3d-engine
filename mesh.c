#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"

typedef struct {
    GLuint VBO;
    GLuint VAO;
    GLuint shaderProg;
}Mesh;

void compileShaderProgramFromPath(GLuint shaderProgBuff, char *vertexShaderPath, char *fragmentShaderPath) {
    char vertexShaderBuffer[1024], fragmentShaderBuffer[1024];
    int success;
    char infoLog[512];

    FILE *vertexShaderFile = fopen(vertexShaderPath, "r");
    if (vertexShaderFile == NULL) {
        printf("failed to read/open vertex shader file: PATH: %s\n", vertexShaderPath);
        exit(1);
    }
    for (int i = 0; i < 1024; i++) {
        vertexShaderBuffer[i] = fgetc(vertexShaderFile);
        if (vertexShaderBuffer[i] == EOF) {
            vertexShaderBuffer[i] = '\0';
            break;
        }
    }
    FILE *fragmentShaderFile = fopen(fragmentShaderPath, "r");
    if (fragmentShaderFile == NULL) {
        printf("failed to read/open fragment shader file: PATH: %s\n", fragmentShaderPath);
        exit(1);
    }
    for (int i = 0; i < 1024; i++) {
        fragmentShaderBuffer[i] = fgetc(fragmentShaderFile);
        if (fragmentShaderBuffer[i] == EOF) {
            fragmentShaderBuffer[i] = '\0';
            break;
        }
    }
    fclose(vertexShaderFile);
    fclose(fragmentShaderFile);
    const GLchar *vertexShaderSource = vertexShaderBuffer;
    const GLchar *fragmentShaderSource = fragmentShaderBuffer;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Error compiling vertex shader");
        FILE *infoLogFile = fopen("vertex shader compilation info log", "w");
        fprintf(infoLogFile, "%s\n", infoLog);
        fclose(infoLogFile);
        exit(1);
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Error compiling fragment shader");
        FILE *infoLogFile = fopen("fragment shader compilation info log", "w");
        fprintf(infoLogFile, "%s\n", infoLog);
        fclose(infoLogFile);
        exit(1);
    }
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error linking shader program");
        FILE *infoLogFile = fopen("shader program link info log", "w");
        fprintf(infoLogFile, "%s\n", infoLog);
        fclose(infoLogFile);
        exit(1);
    }
    shaderProgBuff = shaderProgram;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
void loadMeshStatic(float vertices[], GLuint VAOBuff, GLuint VBOBuff, GLuint EBOBuff) {
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    VAOBuff = VAO;
    VBOBuff = VBO;
    EBOBuff = EBO;
}

void drawMeshStatic(GLuint shaderProg, GLuint VAO, int verticesCount) {
    glUseProgram(shaderProg);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesCount);
    glBindVertexArray(0);
}
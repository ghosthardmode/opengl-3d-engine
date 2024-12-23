#ifndef MESH_H
  #define MESH_H
#endif

typedef struct{
    GLuint VBO;
    GLuint VAO;
    GLuint shaderProg;
}Mesh;

void compileShaderProgramFromPath(GLuint shaderProgBuff, char *vertexShaderPath, char *fragmentShaderPath);
void loadMeshStatic(float vertices[], GLuint VAOBuff, GLuint VBOBuff, GLuint EBOBuff);
void drawMeshStatic(GLuint shaderProg, GLuint VAO, int verticesCount);
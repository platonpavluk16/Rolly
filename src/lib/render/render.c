#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "render.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"uniform vec2 uPosition;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(\n"
"        aPos.x + uPosition.x,\n"
"        aPos.y + uPosition.y,\n"
"        aPos.z,\n"
"        1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\n";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D uTexture;\n"
"uniform vec4 uColor;\n"
"uniform int uUseTexture;\n"
"uniform int uUseAlphaCutoff;\n"
"uniform float uAlphaThreshold;\n"
"void main()\n"
"{\n"
"    vec4 color = uUseTexture == 1 ? texture(uTexture, TexCoord) : uColor;\n"
"    if (uUseAlphaCutoff == 1 && color.a < uAlphaThreshold)\n"
"        discard;\n"
"    FragColor = color;\n"
"}\n";

typedef struct
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    GLsizei indexCount;
    float posX;
    float posY;
} RenderObject;

static RenderObject *objects = NULL;
static size_t objectsCount = 0;

static unsigned int shaderProgram;
static unsigned int textureID;
static int useTexture = 0;
static int useAlphaCutoff = 0;
static float alphaThreshold = 0.1f;
static int colorLocation = -1;
static int useTextureLocation = -1;
static int useAlphaCutoffLocation = -1;
static int alphaThresholdLocation = -1;
static int positionLocation = -1;

void ColorBG(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize)
{
    RenderObject obj;

    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(1, &obj.VBO);
    glGenBuffers(1, &obj.EBO);

    glBindVertexArray(obj.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    obj.indexCount = (GLsizei)(indicesSize / sizeof(unsigned int));
    obj.posX = 0.0f;
    obj.posY = 0.0f;

    // append to objects array
    RenderObject *newArr = realloc(objects, (objectsCount + 1) * sizeof(RenderObject));
    if (!newArr)
    {
        fprintf(stderr, "Failed to allocate objects array\n");
        // cleanup created buffers
        glDeleteVertexArrays(1, &obj.VAO);
        glDeleteBuffers(1, &obj.VBO);
        glDeleteBuffers(1, &obj.EBO);
        return (unsigned int)-1;
    }

    objects = newArr;
    objects[objectsCount] = obj;
    unsigned int id = (unsigned int)objectsCount;
    objectsCount++;

    return id;
}

void CreateTextureFromFile(const char *path)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (!data)
    {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

unsigned int CreateStandartShader(void)
{
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    int texLoc = glGetUniformLocation(shaderProgram, "uTexture");
    if (texLoc != -1)
        glUniform1i(texLoc, 0);

    colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    useTextureLocation = glGetUniformLocation(shaderProgram, "uUseTexture");
    useAlphaCutoffLocation = glGetUniformLocation(shaderProgram, "uUseAlphaCutoff");
    alphaThresholdLocation = glGetUniformLocation(shaderProgram, "uAlphaThreshold");
    positionLocation = glGetUniformLocation(shaderProgram, "uPosition");

    if (useTextureLocation != -1)
        glUniform1i(useTextureLocation, useTexture);
    if (useAlphaCutoffLocation != -1)
        glUniform1i(useAlphaCutoffLocation, useAlphaCutoff);
    if (alphaThresholdLocation != -1)
        glUniform1f(alphaThresholdLocation, alphaThreshold);
    if (positionLocation != -1)
        glUniform2f(positionLocation, 0.0f, 0.0f);

    return shaderProgram;
}

void SetColors(float r, float g, float b, float a)
{
    glUseProgram(shaderProgram);
    if (colorLocation != -1)
        glUniform4f(colorLocation, r, g, b, a);
}

void SetTextureEnabled(int enabled)
{
    useTexture = enabled ? 1 : 0;
    glUseProgram(shaderProgram);
    if (useTextureLocation != -1)
        glUniform1i(useTextureLocation, useTexture);
}

void SetAlphaCutoffEnabled(int enabled)
{
    useAlphaCutoff = enabled ? 1 : 0;
    glUseProgram(shaderProgram);
    if (useAlphaCutoffLocation != -1)
        glUniform1i(useAlphaCutoffLocation, useAlphaCutoff);
}

void SetAlphaThreshold(float threshold)
{
    alphaThreshold = threshold;
    glUseProgram(shaderProgram);
    if (alphaThresholdLocation != -1)
        glUniform1f(alphaThresholdLocation, alphaThreshold);
}

void SetObjectPosition(unsigned int objectId, float x, float y)
{
    if (objectId >= objectsCount)
        return;
    objects[objectId].posX = x;
    objects[objectId].posY = y;
}

void DrawAll(void)
{
    glUseProgram(shaderProgram);
    if (useTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    for (size_t i = 0; i < objectsCount; ++i)
    {
        RenderObject *o = &objects[i];
        if (positionLocation != -1)
            glUniform2f(positionLocation, o->posX, o->posY);

        glBindVertexArray(o->VAO);
        glDrawElements(GL_TRIANGLES, o->indexCount, GL_UNSIGNED_INT, 0);
    }
}

void DeleteTrash(void)
{
    glDeleteTextures(1, &textureID);
    for (size_t i = 0; i < objectsCount; ++i)
    {
        glDeleteVertexArrays(1, &objects[i].VAO);
        glDeleteBuffers(1, &objects[i].VBO);
        glDeleteBuffers(1, &objects[i].EBO);
    }
    free(objects);
    objects = NULL;
    objectsCount = 0;
    glDeleteProgram(shaderProgram);
}
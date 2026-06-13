#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <stddef.h>

void ColorBG(float r, float g, float b, float a);

// Create a renderable object from vertex and index data.
// Returns an object id (0-based) used by other functions.
unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize);

unsigned int CreateStandartShader(void);

void CreateTextureFromFile(const char *path);

void SetColors(float r, float g, float b, float a);

void SetTextureEnabled(int enabled);

void SetAlphaCutoffEnabled(int enabled);

void SetAlphaThreshold(float threshold);

// Set per-object position
void SetObjectPosition(unsigned int objectId, float x, float y);

// Draw all created objects
void DrawAll(void);

void DeleteTrash(void);

#endif
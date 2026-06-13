#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void ColorBG(float r, float g, float b, float a);

unsigned int CreateStandartShader(void);

unsigned int CreateObject(float *vertices,
                          size_t verticesSize,
                          unsigned int *indices,
                          size_t indicesSize,
                          const char *texturePath, float r, float g, float b, float a);

void SetObjectPosition(unsigned int objectId, float x, float y);

void SetObjectAlphaCutoff(unsigned int objectId, float cutoff);
void SetObjectRemoveBackground(unsigned int objectId, int enable);

void DrawAll(void);

void SetColors(float r, float g, float b, float a);

void DeleteTrash(void);

#ifdef __cplusplus
}
#endif

#endif
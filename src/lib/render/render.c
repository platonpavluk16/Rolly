#include <glad/glad.h>

int ColorBG(int r, int g, int b, int alpha)
{
    glClearColor(r, g, b, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}
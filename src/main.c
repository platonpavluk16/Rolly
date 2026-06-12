#include "lib/win/window.h"
#include "lib/render/render.h"

int main()
{
    Init();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    Window *win = create_window(600, 600, "Title");

    if (win == NULL)
    {
        return -2;
    }

    MakeContext(win);

    if (errorGLAD(win) != 0)
    {
        return 1;
    }
    
    InitEP(
        vertices,
        sizeof(vertices),
        indices,
        sizeof(indices)
    );

    CreateStandartShader();

    loop(win)
    {
        ColorBG(1.0, 0.0, 0.0, 1.0);
        SetColors(1.0f, 1.0f, 0.0f, 1.0f);
        Draw();
    }

    DeleteTrash();

    Destroy(win);
    Terminate();
}
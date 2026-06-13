#include "lib/win/window.h"
#include "lib/render/render.h"

int main(void)
{
    Init();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    Window *win = create_window(600, 600, "Title");

    if (win == NULL)
        return -1;

    MakeContext(win);

    if (errorGLAD(win) != 0)
        return -1;

    InitEP(
        vertices,
        sizeof(vertices),
        indices,
        sizeof(indices)
    );

    CreateStandartShader();
    CreateTextureFromFile("src/assets/sim.png");
    SetColors(0.0f, 0.0f, 1.0f, 1.0f);
    SetTextureEnabled(1);
    SetAlphaThreshold(0.1f);
    SetAlphaCutoffEnabled(1);


    float playerX = 0.0f;
    float playerY = 0.0f;

    loop(win)
    {
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            playerX -= 0.01f;

        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            playerX += 0.01f;

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            playerY += 0.01f;

        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            playerY -= 0.01f;

        SetPosition(playerX, playerY);

        ColorBG(
            1.0f,
            0.0f,
            0.0f,
            1.0f
        );

        Draw();
    }

    DeleteTrash();

    Destroy(win);
    Terminate();

    return 0;
}
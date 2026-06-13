#include <stdio.h>

#include "lib/win/window.h"
#include "lib/render/render.h"
#include "lib/physics/collision.h"

int main(void)
{
    Init();

    Window *win = create_window(1920, 1080, "Title");

    if (!win)
        return -1;

    MakeContext(win);

    if (errorGLAD(win) != 0)
        return -1;

    CreateStandartShader();

    float vertices[] = {
        -0.12f, -0.12f, 0.0f, 0.0f, 0.0f,
         0.12f, -0.12f, 0.0f, 1.0f, 0.0f,
         0.12f,  0.12f, 0.0f, 1.0f, 1.0f,
        -0.12f,  0.12f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int obj1 =
        CreateObject(vertices, sizeof(vertices),
                     indices, sizeof(indices),
                     NULL,
                     1.0f, 1.0f, 1.0f, 1.0f);

    unsigned int obj2 =
        CreateObject(vertices, sizeof(vertices),
                     indices, sizeof(indices),
                     NULL,
                     1.0f, 1.0f, 1.0f, 1.0f);

    if (obj1 == (unsigned int)-1 || obj2 == (unsigned int)-1) {
        fprintf(stderr, "Failed to create object(s). Check texture paths.\n");
        Destroy(win);
        Terminate();
        return -1;
    }

    SetObjectPosition(obj1, 0.0f, 0.0f);
    SetObjectPosition(obj2, 0.8f, 0.0f);

    SetObjectRemoveBackground(obj1, 1);
    SetObjectRemoveBackground(obj2, 1);

    float playerX = 0.0f;
    float playerY = 0.0f;

    int wasHit = 0;

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

        SetObjectPosition(obj1, playerX, playerY);

        Collider player = {
            playerX - 0.12f,
            playerY - 0.12f,
            0.24f,
            0.24f
        };

        Collider enemy = {
            0.8f,
            0.0f,
            0.24f,
            0.24f
        };

        ColorBG(0.0f, 0.0f, 0.0f, 1.0f);

        if (CheckCollision(player, enemy)) {
            if (!wasHit) {
                printf("Hit\n");
                wasHit = 1;
            }
        } else {
            wasHit = 0;
        }

        DrawAll();
    }

    DeleteTrash();

    Destroy(win);
    Terminate();

    return 0;
}
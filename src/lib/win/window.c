#include <stdio.h>
#include <glad/glad.h>
#include "window.h"

typedef GLFWwindow Window;

#define loop(window)                                              \
    while (!glfwWindowShouldClose(window))                        \
        for (int _once = 1; _once;                                \
             glfwSwapBuffers(window), glfwPollEvents(), _once = 0)

Window *create_window(int w, int h, const char *t)
{
    if (!glfwInit())
        return NULL;

    return glfwCreateWindow(w, h, t, NULL, NULL);
}

void MakeContext(Window *window)
{
    glfwMakeContextCurrent(window);
}

void Destroy(Window *window)
{
    glfwDestroyWindow(window);
}

void Terminate(void)
{
    glfwTerminate();
}

void Init()
{
    glfwInit();
}

int errorGLAD(Window *window)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    return 0;
}

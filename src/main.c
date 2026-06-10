#include "lib/win/window.c"
#include <glad/glad.h>

int main()
{
    Init();

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

    loop(win)
    {

    }
    
    Destroy(win);
    Terminate();
}
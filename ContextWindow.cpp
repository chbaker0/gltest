#include "ContextWindow.hpp"

bool ContextWindow::create()
{
    if(isCreated)
        return true;

    win = glfwCreateWindow(width, height,
                           title.c_str(),
                           monitor, share);
    if(win == nullptr)
        return false;
    else
        return isCreated = true;
}

bool ContextWindow::destroy()
{
    if(!isCreated)
        return false;

    glfwDestroyWindow(win);
    isCreated = false;
    return true;
}

unsigned int ContextWindow::winCount = 0;
void ContextWindow::initGLFW()
{
    if(winCount++ == 0)
        glfwInit();
}
void ContextWindow::terminateGLFW()
{
    if(--winCount == 0)
        glfwTerminate();
}

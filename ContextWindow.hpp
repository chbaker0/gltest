#ifndef CONTEXT_WINDOW_HPP_INCLUDED
#define CONTEXT_WINDOW_HPP_INCLUDED

#include "Includes.h"

class ContextWindow
{
public:
    ContextWindow():
        win(nullptr), share(nullptr),
        width(0), height(0),
        monitor(nullptr), isCreated(false)
    {
        initGLFW();
    }
    ~ContextWindow()
    {
        destroy();
        terminateGLFW();
    }

    bool create();
    bool destroy();

    void setDimensions(unsigned int _width, unsigned int _height)
    {
        width = _width, height = _height;
    }
    void setTitle(std::string _title)
    {
        title = std::move(_title);
    }
    void setFullscreen(bool isFullscreen)
    {
        if(isFullscreen)
            monitor = glfwGetPrimaryMonitor();
        else
            monitor = nullptr;
    }
    static void setHint(int target, int hint)
    {
        glfwWindowHint(target, hint);
    }
    void makeCurrent()
    {
        glfwMakeContextCurrent(win);
    }
    void swapBuffers()
    {
        glfwSwapBuffers(win);
    }
    bool shouldClose() const
    {
        return glfwWindowShouldClose(win);
    }
    static void pollEvents()
    {
        glfwPollEvents();
    }

private:
    GLFWwindow *win, *share;
    unsigned int width, height;
    std::string title;
    GLFWmonitor *monitor;
    bool isCreated;

    static unsigned int winCount;
    static void initGLFW();
    static void terminateGLFW();
};

#endif // CONTEXT_WINDOW_HPP_INCLUDED

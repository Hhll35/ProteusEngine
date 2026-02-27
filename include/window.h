#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
    public:
        Window(float width, float height, const char* name);
        ~Window();

        bool shouldClose() const;
        void pollEvents();
        void swapBuffers();

        GLFWwindow* getGLFWWindow() const;
    private:
        GLFWwindow* window;
};

#endif
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window
{
    public:
        Window(float width, float height, const char* name);
        ~Window();

        bool init(int width, int height, const char* name);
        bool shouldClose() const;


        GLFWwindow* getGLFWWindow() const;
    private:
        GLFWwindow* window;
};

#endif
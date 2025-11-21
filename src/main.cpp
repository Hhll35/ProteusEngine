#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shader.h>
#include <stb_image.h>

using namespace std; 

// Callback function that gets called whenever the window is resized.
// It updates the OpenGL viewport to match the new window dimensions.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW (the library that handles windows & input)
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // Configure GLFW: enable 4x multisampling (anti-aliasing)
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Request OpenGL version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window (800x600). If creation fails, exit.
    GLFWwindow* window = glfwCreateWindow(800, 600, "Opengl Engine", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }

    // Make the OpenGL context of the window current in this thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD (loads all OpenGL function pointers)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // Set the initial viewport size
    glViewport(0, 0, 800, 600);

    // Register the callback so that the viewport updates when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Shader textureShader("C:/Users/Eli/EngineProject/shaders/basicVertex.vert","C:/Users/Eli/EngineProject/shaders/basicFrag.frag");

    //Shader firstShader("shaders/basic1.vert","shaders/color1.frag");
    //Shader secondShader("shaders/basic2.vert","shaders/color2.frag");

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };


    unsigned int VAO, VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    //Bind first VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // Main render loop: runs until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.42f, 0.55f, 0.63f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textureShader.use();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);

        // Swap the front and back buffers 
        // (This displays whatever was drawn in the previous frame)
        glfwSwapBuffers(window);
        // Process pending events (keyboard, mouse, etc.)
        glfwPollEvents();    
    }

    // Clean up and close the program
    glfwTerminate();
    return 0;
}
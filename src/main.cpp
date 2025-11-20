#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

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

    Shader firstShader("C:/Users/Eli/EngineProject/shaders/basic1.vert","C:/Users/Eli/EngineProject/shaders/color1.frag");
    Shader secondShader("C:/Users/Eli/EngineProject/shaders/basic2.vert","C:/Users/Eli/EngineProject/shaders/color2.frag");

    //Shader firstShader("shaders/basic1.vert","shaders/color1.frag");
    //Shader secondShader("shaders/basic2.vert","shaders/color2.frag");

    // A simple triangle defined by three vertices
    float vertices1[] = {
        -0.9f, -0.5f, 0.0f,  // left
        0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
    };

    float vertices2[] = {
        0.0f,-0.5f,0.0f, //left
        0.9f,-0.5f,0.0f, //right
        0.45f,0.5f,0.0f //bottom
    };

    // float vertices[] = {
    //      0.5f,  0.5f, 0.0f,  // top right
    //      0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  // bottom left
    //     -0.5f,  0.5f, 0.0f   // top left 
    // };

    //indices for rectangle
    // unsigned int indices[] = {
    //     0,1,3,
    //     1,2,3
    // };


    unsigned int VAO[2], VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2,VBO);

    //Bind first VAO and VBO
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    //Bind second VAO and VBO
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    // Main render loop: runs until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.42f, 0.55f, 0.63f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //create first triangle with first shader program
        firstShader.use();

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //create second triangle with second shader program

        secondShader.use();

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue)/ 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(secondShader,"ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0 ,3);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

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
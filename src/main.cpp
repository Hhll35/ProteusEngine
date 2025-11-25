#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <stb_image.h>
#include <hex_converter.h>


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
        std::cout << "Failed to initialize GLFW" << std::endl;
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
        std::cout << "Failed to open GLFW window" << std::endl;
        return -1;
    }

    // Make the OpenGL context of the window current in this thread
    glfwMakeContextCurrent(window);

    // Initialize GLAD (loads all OpenGL function pointers)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
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
        //Positions             colors       texture coords
         0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,  1.0f,0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,// bottom left
        -0.5f,  0.5f, 0.0f,  0.0f,1.0f,0.0f,  0.0f,1.0f// top left 
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    //create first texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("C:/Users/Eli/EngineProject/assets/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //create second texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("C:/Users/Eli/EngineProject/assets/awesomeface.png", &width, &height, &nrChannels,0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);    

    textureShader.use();
    glUniform1i(glGetUniformLocation(textureShader.ID, "texture1"), 0);
    textureShader.setInt("texture2", 1);

    // Main render loop: runs until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.44444f, 0.66222f, 1.05333f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        //start rendering
        textureShader.use();

        glBindVertexArray(VAO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
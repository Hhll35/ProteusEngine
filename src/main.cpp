#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <stb_image.h>
#include <camera.h>


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
// Callback function that gets called whenever the window is resized.
// It updates the OpenGL viewport to match the new window dimensions.

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

glm::vec3 coral(1.0f, 0.5f, 0.31f);

glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
glm::vec3 toyColor(1.0f, 0.5f, 0.31f);

glm::vec3 result = lightColor * toyColor;
glm::vec3 lightPos(0.0f, 0.0f, 1.5f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



int main()
{

    const int SCR_WIDTH = 800;
    const int SCR_HEIGHT = 600;
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proteus Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    Shader basicShader("C:/Users/Eli/ProteusEngine/shaders/basicVertex.vert", "C:/Users/Eli/ProteusEngine/shaders/basicFrag.frag");
    Shader lightCubeShader("C:/Users/Eli/ProteusEngine/shaders/lightCube.vert", "C:/Users/Eli/ProteusEngine/shaders/lightCube.frag");

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
        // positions          // normals           // texcoords
        // Back face
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

        // Left face
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        // Right face
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,    0.0f, 1.0f
    };


    glm::vec3  cubePositions[] =
    {
        glm::vec3( 0.0f,  0.0f,  0.0f),
    };


    unsigned int VAO, VBO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    //Bind first VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int LightVAO;
    glGenVertexArrays(1, &LightVAO);
    glBindVertexArray(LightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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
    unsigned char *data = stbi_load("C:/Users/Eli/ProteusEngine/assets/diffuse.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //create first texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("C:/Users/Eli/ProteusEngine/assets/specular.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
    

    // Main render loop: runs until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //start rendering
        basicShader.use();
        basicShader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        basicShader.setInt("material.specular",1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        basicShader.setVec3("viewPos", camera.Position);
        basicShader.setVec3("light.position", lightPos);

        basicShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        basicShader.setFloat("material.shininess", 150.0f);

        basicShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        basicShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        basicShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        basicShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        basicShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        basicShader.setMat4("model", model);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 1; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            float spinangle = glfwGetTime() * 50.0; // 50 = rotation speed
            model = glm::rotate(model, glm::radians(spinangle), glm::vec3(1.0f, 0.3f, 0.5f));;
            basicShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        float time = glfwGetTime();

        lightCubeShader.use();
        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(LightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}
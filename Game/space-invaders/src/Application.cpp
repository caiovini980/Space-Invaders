#include "Application.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if(!glfwInit())
    {
        return -1;        
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Activate V-Sync
    glfwSwapInterval(1);

    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: Glew wasn't started properly\n";
    }

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // OpenGL configuration
    // --------------------
    GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // initialize game
    // ---------------
    SpaceInvaders->Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // manage user input
        // -----------------
        SpaceInvaders->ProcessInput(deltaTime);

        // update game state
        // -----------------
        SpaceInvaders->Update(deltaTime);

        // render
        // ------
        SpaceInvaders->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear stuff
    // ---------------------------------------------------------
    SpaceInvaders->Close();
    SpaceInvaders.reset();

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {

        }

        else if (action == GLFW_RELEASE)
        {

        }
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
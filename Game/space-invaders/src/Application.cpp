#include "Application.h"

#include <iostream>

#include "Audio.h"
#include "Input.h"
#include "utils/GameTime.h"
#include "utils/Random.h"

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
    Audio::Init();
    Random::Init();
    m_SpaceInvaders->Init();

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

        // Update GameTime class
        GameTime::Time = currentFrame;
        GameTime::DeltaTime = deltaTime;

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // manage user input
        // -----------------
        m_SpaceInvaders->ProcessInput(deltaTime, *m_InputHandler);

        // update game state
        // -----------------
        m_SpaceInvaders->Update(deltaTime);

        // render
        // ------
        m_SpaceInvaders->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clear stuff
    // ---------------------------------------------------------
    m_SpaceInvaders->Close();
    m_SpaceInvaders.reset();

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
            m_InputHandler->SetKeyStatus(key, true);
        }

        else if (action == GLFW_RELEASE)
        {
            m_InputHandler->SetKeyStatus(key, false);
        }
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
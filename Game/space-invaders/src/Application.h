#pragma once
#include <memory>

#include "Core.h"
#include <GLFW/glfw3.h>

#include "Game.h"

// GLFW function declarations
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Screen dimensions
constexpr unsigned int SCREEN_WIDTH = 800;
constexpr unsigned int SCREEN_HEIGHT = 600;

// Create Game
std::unique_ptr<Game> SpaceInvaders = std::make_unique<Game>();
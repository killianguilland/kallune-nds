#define GLFW_INCLUDE_NONE
#include "tools/matrix_stack.hpp"
#include <iostream>
#include <cmath>
#include "graphics.hpp"
#include <iostream>

using namespace glbasimac;

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double FRAMERATE_IN_SECONDS{1. / 60.};
float aspectRatio{};

float GL_VIEW_SIZE = 9.0f;
float viewWidth = 16.0f;
float viewHeight = 9.0f;

/* Error handling function */
void Graphics::onError(int error, const char *description)
{
    std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void Graphics::onWindowResized(GLFWwindow *window, int width, int height)
{
    std::cout << "Window resized to " << width << "x" << height << std::endl;

    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;

    aspectRatio = width / (float)height;
    glfwGetFramebufferSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (aspectRatio > 1.0f)
    {
        viewWidth = GL_VIEW_SIZE * aspectRatio;
        viewHeight = GL_VIEW_SIZE;
    }
    else
    {
        viewWidth = GL_VIEW_SIZE;
        viewHeight = GL_VIEW_SIZE / aspectRatio;
    }

    GameEngine.set2DProjection(
        -viewWidth / 2.0f, viewWidth / 2.0f,
        -viewHeight / 2.0f, viewHeight / 2.0f);
}

Graphics::Graphics()
{
    // Set error callback
    glfwSetErrorCallback(onError);

    // Initialize the library
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a windowed mode window and its OpenGL context
    this->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kallune", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW Window");
    }

    // -- Callbacks --
    glfwSetWindowSizeCallback(window, onWindowResized);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSetWindowAspectRatio(window, 16, 9);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize Glad");
    }

    // Initialize Rendering Engine

    GameEngine.mode2D = true;

    GameEngine.initGL();

    onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    game_scene = new GameScene();
    menu_scene = new MenuScene();
    settings_scene = new SettingsScene();
    end_scene = new EndScene();
    pause_scene = new PauseScene();

    GameEngine.activateTexturing(true);

    cursorSprite = new Sprite("cursors/cursor.png", 0.7f, 0.7f);
    cursorAnimatedSprite = new AnimatedSprite("cursors/animated_cursor.png", 0.7f, 0.7f, 3, 1, 6);
}

void Graphics::render(double deltaTime, Router *router, InputState inputState, Game game)
{
    /* Get time (in second) at loop beginning */
    double startTime = glfwGetTime();

    double x_world = (inputState.mouseX / WINDOW_WIDTH) * viewWidth * 2 - viewWidth / 2.0f;
    double y_world = (inputState.mouseY / WINDOW_HEIGHT) * viewHeight * -2 + viewHeight / 2.0f;

    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    switch (router->currentScene)
    {
    case Scene::Menu:
        menu_scene->draw(deltaTime);
        break;
    case Scene::Settings:
        settings_scene->draw(deltaTime);
        break;
    case Scene::Playing:
        game_scene->draw(deltaTime, game);
        break;
    case Scene::End:
        end_scene->draw();
        break;
    case Scene::Pause:
        pause_scene->draw();
        break;
    }

    GameEngine.mvMatrixStack.pushMatrix();
    GameEngine.mvMatrixStack.addTranslation(Vector3D(0.1f, -0.2f, 0.0f));
    if (startTime - inputState.cursorClickTime < CURSOR_ANIMATION_DURATION)
    {
        cursorAnimatedSprite->update(deltaTime);
        GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(inputState.x_world, inputState.y_world, 0.0f));
        GameEngine.updateMvMatrix();
        cursorAnimatedSprite->draw();
        GameEngine.mvMatrixStack.popMatrix();
    }
    else
    {
        GameEngine.mvMatrixStack.pushMatrix();
        GameEngine.mvMatrixStack.addTranslation(Vector3D(inputState.x_world, inputState.y_world, 0.0f));
        GameEngine.updateMvMatrix();
        cursorSprite->draw();
        GameEngine.mvMatrixStack.popMatrix();
    }
    GameEngine.mvMatrixStack.popMatrix();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Elapsed time computation from loop begining */
    double elapsedTime = glfwGetTime() - startTime;
    /* If to few time is spend vs our wanted FPS, we wait */
    while (elapsedTime < FRAMERATE_IN_SECONDS)
    {
        glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
        elapsedTime = glfwGetTime() - startTime;
    }
}

void Graphics::update(double deltaTime, Game game, InputState state, Router *router)
{
    // std::cout << "deltaTime :" << deltaTime << std::endl; 
    switch (router->currentScene)
    {
    case Scene::Menu:
        menu_scene->update(state, router, window);
        break;

    case Scene::Settings:
        settings_scene->update(state, router);
        break;

    case Scene::Playing:
        if (!game.isPlayerAlive())
        {
            router->goTo(Scene::End);
            return;
        }
        else
        {
            game_scene->update(deltaTime, state, router, game);
            break;
        }

    case Scene::Pause:
        pause_scene->update(state, router);
        break;
    
    case Scene::End:
        end_scene->update(state, router);
        break;

    default:
        std::cerr << "Euh y a un truc pas normal chef" << std::endl;
        break;
    }
}

bool Graphics::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Graphics::close()
{
    glfwTerminate();
}
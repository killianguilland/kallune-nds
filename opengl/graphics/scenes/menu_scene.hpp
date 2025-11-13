#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/basic_mesh.hpp"

#include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/menu_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"

using namespace glbasimac;

/* OpenGL Engine */
extern GLBI_Engine GameEngine;
extern double mouseX, mouseY;
extern double x_world;
extern double y_world;
extern bool cursorClicked;
extern double cursorClickTime;

class MenuScene
{
public:
    MenuScene();
    ~MenuScene() = default;

    void update(InputState inputState, Router* router, GLFWwindow* window);
    void draw(double deltaTime);

private:
    // BACKGROUND
    AnimatedSprite *animation {};
    Sprite *background {};

    // BUTTONS
    Sprite *playButton {};
    Sprite *playButtonHover {};
    Sprite *settingsButton {};
    Sprite *settingsButtonHover {};
    Sprite *exitButton {};
    Sprite *exitButtonHover {};

    // STATE
    MenuState state {MenuState()};
};
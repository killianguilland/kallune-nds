#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/basic_mesh.hpp"

#include "graphics/elements/animatedSprite.hpp"
#include "input/scenes/settings_state.hpp"
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

class SettingsScene
{
public:
    SettingsScene();
    ~SettingsScene() = default;

    void update(InputState inputState, Router* router);
    void draw(double deltaTime);

private:
    // BACKGROUND
    AnimatedSprite *animation {};
    Sprite *background {};

    // BUTTONS
    Sprite *returnButton {};
    Sprite *returnButtonHover {};

    // STATE
    SettingsState state {SettingsState()};
};
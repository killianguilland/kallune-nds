#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/basic_mesh.hpp"

#include "input/scenes/pause_state.hpp"
#include "utils/scene.hpp"
#include "utils/router.hpp"


#include "graphics/elements/animatedSprite.hpp"

using namespace glbasimac;

/* OpenGL Engine */
extern GLBI_Engine GameEngine;
extern double mouseX, mouseY;
extern double x_world;
extern double y_world;
extern bool cursorClicked;
extern double cursorClickTime;

class PauseScene
{

public:
    PauseScene();
    ~PauseScene() = default;

    void update(InputState inputState, Router *router);

    void draw();

private:
    Sprite *texteSprite{};
    Sprite *returnButton{};
    Sprite *returnButtonHover{};
    Sprite *exitButton{};
    Sprite *exitButtonHover{};

    // STATE
    PauseState state {PauseState()};
};
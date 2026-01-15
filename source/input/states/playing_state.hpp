#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class PlayingState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::Playing;

    ~PlayingState() override = default;
    ButtonState pauseButton{ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};
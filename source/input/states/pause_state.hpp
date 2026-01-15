#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class PauseState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::Pause;

    ~PauseState() override = default;
    ButtonState returnButton{ButtonState::DEFAULT};
    ButtonState exitButton{ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};
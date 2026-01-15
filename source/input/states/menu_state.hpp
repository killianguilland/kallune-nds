#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class MenuState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::Menu;

    ~MenuState() override = default;
    ButtonState playButton{ButtonState::DEFAULT};
    ButtonState settingsButton{ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};
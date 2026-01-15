#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class EndState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::End;

    ~EndState() override = default;
    ButtonState exitButton{ButtonState::DEFAULT};

    void update(InputState state, Router* router);
};
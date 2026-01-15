#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class LoadingState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::Loading;

    ~LoadingState() override = default;
    void update(InputState state, Router* router);
};
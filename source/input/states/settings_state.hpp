#pragma once
#include "input/state.hpp"
#include "utils/router.hpp"
#include "utils/state.hpp"

class SettingsState : public StateInterface {
public:
    static constexpr Scene scene_type = Scene::Settings;

    ~SettingsState() override = default;
    ButtonState waterButton{DEFAULT};
    ButtonState sizeButton{DEFAULT};
    ButtonState typeButton{DEFAULT};
    ButtonState returnButton{DEFAULT};

    int waterValue{0};
    int sizeValue{0};
    int typeValue{0};

    void update(InputState state, Router* router);
};
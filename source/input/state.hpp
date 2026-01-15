#pragma once

#include "utils/router.hpp"
#include "utils/state.hpp"

class StateInterface {
public:
    virtual ~StateInterface() {}
    // Les méthodes que chaque scène DOIT implémenter
    virtual void update(InputState state, Router* router) = 0; // Rendu graphique
};
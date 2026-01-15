#pragma once
#include "input/input.hpp"

class SceneInterface {
public:
    // LE POINT CRUCIAL : le destructeur virtuel
    virtual ~SceneInterface() {}

    // Les méthodes que chaque scène DOIT implémenter
    virtual void draw(const Input& input) = 0; // Rendu graphique

    // Optionnel : une méthode pour le post-rendu ou les transitions
    virtual void postRender() = 0;
};
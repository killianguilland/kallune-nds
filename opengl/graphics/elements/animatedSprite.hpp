#pragma once

#include "sprite.hpp"

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite(const std::string& filename, float sizex, float sizez, int columns, int rows, int framerate);

    void setFrame(int frameIndex);
    void update(float deltaTime); // Advances frame based on time
    void setFrameRate(float fps);

private:
    int columns;
    int rows;
    int currentFrame;
    float frameRate;     // Frames per second
    float frameTime;     // Time per frame
    float elapsedTime;   // Accumulated time since last frame switch
};
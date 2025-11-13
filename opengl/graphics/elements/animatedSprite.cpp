#include "animatedSprite.hpp"

AnimatedSprite::AnimatedSprite(const std::string& filename, float sizex, float sizez, int columns, int rows, int framerate)
    : Sprite(filename, sizex, sizez), columns(columns), rows(rows), currentFrame(0),
      frameRate(10.0f), frameTime(1.0f / 10.0f), elapsedTime(0.0f) {
    setFrame(0); // Start with frame 0
    setFrameRate(framerate);
}

void AnimatedSprite::setFrame(int frameIndex) {
    currentFrame = frameIndex;

    int frame_x = frameIndex % columns;
    int frame_y = frameIndex / columns;

    float u_step = 1.0f / columns;
    float v_step = 1.0f / rows;

    float u0 = frame_x * u_step;
    float v0 = frame_y * v_step;
    float u1 = u0 + u_step;
    float v1 = v0 + v_step;

    // Flip vertically if needed
    // v0 = 1.0f - v0 - v_step;
    // v1 = v0 + v_step;

    float uvs_rect[8] = {
        u0, v1, // Bottom-left
        u0, v0, // Top-left
        u1, v1, // Bottom-right
        u1, v0  // Top-right
    };

	// mesh->addOneBuffer(2, 2, uvs_rect, "uvs", false);
    mesh->updateUVs(uvs_rect, 8); // Recreate the VAO to reflect the changes
}

void AnimatedSprite::update(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime;
        currentFrame = (currentFrame + 1) % (columns * rows);
        setFrame(currentFrame);
    }
}

void AnimatedSprite::setFrameRate(float fps) {
    frameRate = fps;
    frameTime = 1.0f / fps;
}

#pragma once

#include <memory>
#include <map>
#include <iostream>
#include "renderable.hpp"

class RenderQueue {
public:
    RenderQueue() = default;
    ~RenderQueue() = default;

    // Add a renderable object to the queue
    void add(const std::shared_ptr<Renderable>& renderable, int z_index = 0) {
        if (!renderable) {
            std::cout << "Attempted to replace a sprite in render queue" << std::endl;
            return;
        }
        renderables[z_index] = renderable;
    }

    // Clear the queue
    void clear() {
        renderables.clear();
    }

    void render() {
        // std::map already keeps keys sorted by z_index in ascending order
        for (const auto& [z_index, renderable] : renderables) {
            if (renderable) {
                renderable->draw();
            }
        }
    }

private:
    std::map<int, std::shared_ptr<Renderable>> renderables;
};
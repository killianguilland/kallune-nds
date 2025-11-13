#pragma once
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "glbasimac/glbi_texture.hpp"
#include "tools/basic_mesh.hpp"
#include <string>
#include <stb_image.h>
#include "renderable.hpp"

class Sprite : public Renderable {
    public:
        Sprite(const std::string& filename, float sizex = 1.0f, float sizez = 1.0f);
        ~Sprite();

        void draw() override;
        StandardMesh* getMesh();
        glbasimac::GLBI_Texture* getTexture();
    
    protected:
        StandardMesh* mesh;
        glbasimac::GLBI_Texture* texture {};
};
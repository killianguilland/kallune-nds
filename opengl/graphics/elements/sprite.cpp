#include "sprite.hpp"

Sprite::Sprite(const std::string& filename, float sizex, float sizez) {
	std::string path = "assets/images/" + filename;
	int x, y, n;
	unsigned char *imageData{stbi_load(path.c_str(), &x, &y, &n, 0)};

	std::cout << "Image loaded: " << filename << " (" << x << "x" << y << ", channels: " << n << ")" << std::endl;
    
    texture = new glbasimac::GLBI_Texture();
	texture->createTexture();
    texture->attachTexture();
	texture->loadImage(x, y, n, imageData);
    // std::cout << "Texture ID for " << filename << " : " << texture->id_in_GL << std::endl;
	texture->setParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture->setParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	mesh = new StandardMesh(4, GL_TRIANGLE_STRIP);
	float coord[8] = {-sizex/2, -sizez/2,
					  -sizex/2,  sizez/2,
					   sizex/2, -sizez/2,
					   sizex/2,  sizez/2};

	float uvs_rect[8] = {
		0.0f, 1.0f,  // Bottom-left
		0.0f, 0.0f,  // Top-left
		1.0f, 1.0f,  // Bottom-right
		1.0f, 0.0f   // Top-right
	};
	mesh->addOneBuffer(0,2,coord,"coordinates",true);
	mesh->addOneBuffer(2,2,uvs_rect,"uvs",true);

	mesh->createVAO();

    stbi_image_free((void *)imageData);
}

Sprite::~Sprite() {
	delete texture;
    delete mesh;
}

void Sprite::draw() {
    texture->attachTexture();
    mesh->draw();
    texture->detachTexture();
}

StandardMesh* Sprite::getMesh() {
    return mesh;
}
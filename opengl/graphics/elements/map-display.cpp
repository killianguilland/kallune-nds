#include "map-display.hpp"
#include <iomanip>

MapDisplay::MapDisplay()
{
	for (int i = 0; i < 114; ++i)
	{
		std::ostringstream ss;
		ss << "tileset/tile_" << std::setw(3) << std::setfill('0') << i << ".png";
		tileset.push_back(new Sprite(ss.str(), 1.0f, 1.0f));
	}

	const siv::PerlinNoise::seed_type seed = 123456u;

	const siv::PerlinNoise perlin{ seed };
	
	for (int y = 0; y < 100; ++y)
	{
		std::vector<double> noiseMapRow;
		for (int x = 0; x < 100; ++x)
		{
			const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 7);
			noiseMapRow.push_back(noise);
		}
		noiseMap.push_back(noiseMapRow);
	}
}

void MapDisplay::update(double deltaTime, Game game) {
	// Update the noise map based on the player's position
	int nx = 100 - game.getPlayerX();
	int ny = game.getPlayerY();
	if (nx >= 0 && nx < static_cast<int>(noiseMap.size()) &&
		ny >= 0 && ny < static_cast<int>(noiseMap[nx].size()))
	{
		noiseMap[nx][ny] -= 1.25 * deltaTime;
	}

	for (const auto &entity : game.getEntitiesInfo())
	{
		int nx = 100 - entity.x;
		int ny = entity.y;
		if (nx >= 0 && nx < static_cast<int>(noiseMap.size()) &&
			ny >= 0 && ny < static_cast<int>(noiseMap[nx].size()))
		{
			noiseMap[nx][ny] -= 1.25 * deltaTime;
		}
	}
}

void MapDisplay::draw(double deltaTime, Game game)
{
    int MAP_WIDTH = game.map.getWidth();
	int MAP_HEIGHT = game.map.getHeight();

	const std::vector<std::vector<MapType>> &carte = game.map.getMap();
	std::vector<std::vector<MapType>> carte_inverted;
	// invert rows
	for (size_t i = 0; i < 100; i++)
	{
		carte_inverted.push_back(carte[99 - i]);
	}

	int nbr_fleurs = 0;

    // float z_index = 0.0f;
	for (int layer = MAP_WIDTH + MAP_HEIGHT - 2; layer >= 0; --layer)
	{
		int start_x = std::max(0, layer - MAP_HEIGHT + 1);
		int end_x = std::min(MAP_WIDTH - 1, layer);

		for (int x = start_x; x <= end_x; ++x)
		{
			
			int y = layer - x;
			const double noise {noiseMap[y][x]};

			if (y < 0 || y >= MAP_HEIGHT)
				continue;

			int tileType;

			if (carte_inverted[y][x] == MapType::WATER)
			{
				const CornerType cornerType = getCornerType(carte_inverted, x, y);
				// Use a map to associate CornerType with tileType
				static const std::unordered_map<CornerType, int> cornerTileMap = {
					{TOP, 105},
					{BOTTOM, 108},
					{LEFT, 106},
					{RIGHT, 107},
					{TOP_LEFT, 109},
					{TOP_RIGHT, 112},
					{BOTTOM_LEFT, 111},
					{BOTTOM_RIGHT, 110},
					{SURROUNDED, 113},
					{NORMAL, 104}
				};

				tileType = cornerTileMap.at(cornerType);
			}
			else if (carte_inverted[y][x] == MapType::GRASS)
			{
				const std::vector<int> grassTiles = { 24, 24, 24, 24, 22, 22, 22, 22, 23, 23, 23, 23, 29, 30, 31, 32, 33, 34, 35, 36 };
				// Map noise [0,1] to grassTiles indices equally
				size_t numTiles = grassTiles.size();
				size_t idx = static_cast<size_t>(noise * numTiles);
				if (idx >= numTiles) idx = numTiles - 1;
				tileType = grassTiles[idx];
			}
			else if (carte_inverted[y][x] == MapType::SAND)
			{
				tileType = 66;
			}
			else if (carte_inverted[y][x] == MapType::FLOWER)
			{
				nbr_fleurs++;
				tileType = 4;
			}

			else if (carte_inverted[y][x] == MapType::WALL)
			{
				const CornerType cornerType = getCornerType(carte_inverted, x, y);
				// Use a map to associate CornerType with tileType
				static const std::unordered_map<CornerType, int> cornerTileMap = {
					{TOP, 5},
					{BOTTOM, 9},
					{LEFT, 9},
					{RIGHT, 9},
					{TOP_LEFT, 5},
					{TOP_RIGHT, 2},
					{BOTTOM_LEFT, 0},
					{BOTTOM_RIGHT, 4},
					{SURROUNDED, 8},
					{NORMAL, 9}
				};

				tileType = cornerTileMap.at(cornerType);
			}
			else if (carte_inverted[y][x] == MapType::SOLID_WALL)
			{
				const std::vector<int> solidTiles = { 11, 12, 13, 14, 15, 16 };
				// Map noise [0,1] to solidTiles indices equally
				size_t numTiles = solidTiles.size();
				size_t idx = static_cast<size_t>(noise * numTiles);
				if (idx >= numTiles) idx = numTiles - 1;
				tileType = solidTiles[idx];
			}
			else
			{
				tileType = 3;
			}

			float iso_x = (x - y) * 0.50f;
			float iso_y = (x + y) * 0.25f;

			GameEngine.mvMatrixStack.pushMatrix();
			GameEngine.mvMatrixStack.addTranslation(Vector3D(iso_x, iso_y, 0.0f));
			GameEngine.updateMvMatrix();

			tileset[tileType]->draw();

			int height {};
			if(noise > 0.85) {
				height = 4;
			} 
			else if (noise > 0.5) {
				height = 3;
			}
			else if (noise > 0.25) {
				height = 2;
			} 
			else {
				height = 1;
			}

			GameEngine.mvMatrixStack.popMatrix();
			if (carte_inverted[y][x] == MapType::SOLID_WALL || carte_inverted[y][x] == MapType::WALL)
			{
				for (int i = 0; i < height; ++i)
				{
					GameEngine.mvMatrixStack.pushMatrix();
					GameEngine.mvMatrixStack.addTranslation(Vector3D(iso_x, iso_y + 0.25f * (i + 1), 0.0f));
					GameEngine.updateMvMatrix();

					tileset[tileType]->draw();

					GameEngine.mvMatrixStack.popMatrix();
				}
			}
			else if (carte_inverted[y][x] == MapType::FLOWER)
			{
				GameEngine.mvMatrixStack.pushMatrix();
				GameEngine.mvMatrixStack.addTranslation(Vector3D(iso_x, iso_y + 0.25f, 0.0f));
				GameEngine.updateMvMatrix();

				if(noise > 0.75) {
					tileset[44]->draw();
				} 
				else if (noise > 0.5) {
					tileset[45]->draw();
				}
				else if (noise > 0.25) {
					tileset[46]->draw();
				} 
				else {
					tileset[47]->draw();
				}

				GameEngine.mvMatrixStack.popMatrix();
			}
		}
    }
}
#include "game_scene.hpp"
#include <tuple>
#include <stb_image.h>
#include "../elements/sprite.hpp"
#include "../elements/animatedSprite.hpp"
#include <iomanip>
#include <sstream>

GLBI_Engine GameEngine;

Sprite *pauseButton{};
Sprite *pauseButtonHover{};

GameScene::GameScene()
{
	renderQueue = RenderQueue();
	mapDisplay = MapDisplay();

	// Badger
	badger = new Character(1.2f, 1.2f);
	badger->addAnimation(BehaviorType::IDLE, Direction::NORTH, "critters/badger/critter_badger_SW_idle.png",  22, 1, 9);
	badger->addAnimation(BehaviorType::IDLE, Direction::SOUTH, "critters/badger/critter_badger_NE_idle.png",  22, 1, 9);
	badger->addAnimation(BehaviorType::IDLE, Direction::EAST, "critters/badger/critter_badger_SE_idle.png",  22, 1, 9);
	badger->addAnimation(BehaviorType::IDLE, Direction::WEST, "critters/badger/critter_badger_NW_idle.png",  22, 1, 9);
	badger->addAnimation(BehaviorType::MOVE, Direction::NORTH, "critters/badger/critter_badger_SW_walk.png",  9, 1, 15);
	badger->addAnimation(BehaviorType::MOVE, Direction::SOUTH, "critters/badger/critter_badger_NE_walk.png",  9, 1, 15);
	badger->addAnimation(BehaviorType::MOVE, Direction::EAST, "critters/badger/critter_badger_SE_walk.png",  9, 1, 15);
	badger->addAnimation(BehaviorType::MOVE, Direction::WEST, "critters/badger/critter_badger_NW_walk.png",  9, 1, 15);

	// Wolf
	wolf = new Character(2.0f, 2.0f);
	wolf->addAnimation(BehaviorType::IDLE, Direction::NORTH, "critters/wolf/wolf-idle-SW.png",  4, 1, 1);
	wolf->addAnimation(BehaviorType::IDLE, Direction::SOUTH, "critters/wolf/wolf-idle-NE.png",  4, 1, 1);
	wolf->addAnimation(BehaviorType::IDLE, Direction::EAST, "critters/wolf/wolf-idle-SE.png",  4, 1, 1);
	wolf->addAnimation(BehaviorType::IDLE, Direction::WEST, "critters/wolf/wolf-idle-NW.png",  4, 1, 1);
	wolf->addAnimation(BehaviorType::MOVE, Direction::NORTH, "critters/wolf/wolf-run-SW.png",  8, 1, 4);
	wolf->addAnimation(BehaviorType::MOVE, Direction::SOUTH, "critters/wolf/wolf-run-NE.png",  8, 1, 4);
	wolf->addAnimation(BehaviorType::MOVE, Direction::EAST, "critters/wolf/wolf-run-SE.png",  8, 1, 4);
	wolf->addAnimation(BehaviorType::MOVE, Direction::WEST, "critters/wolf/wolf-run-NW.png",  8, 1, 4);

	// Boar
	boar = new Character(1.0f, 0.8f);
	boar->addAnimation(BehaviorType::IDLE, Direction::NORTH, "critters/boar/critter_boar_SW_idle_strip.png",  7, 1, 1);
	boar->addAnimation(BehaviorType::IDLE, Direction::SOUTH, "critters/boar/critter_boar_NE_idle_strip.png",  7, 1, 1);
	boar->addAnimation(BehaviorType::IDLE, Direction::EAST, "critters/boar/critter_boar_SE_idle_strip.png",  7, 1, 1);
	boar->addAnimation(BehaviorType::IDLE, Direction::WEST, "critters/boar/critter_boar_NW_idle_strip.png",  7, 1, 1);
	boar->addAnimation(BehaviorType::MOVE, Direction::NORTH, "critters/boar/critter_boar_SW_run_strip.png",  4, 1, 8);
	boar->addAnimation(BehaviorType::MOVE, Direction::SOUTH, "critters/boar/critter_boar_NE_run_strip.png",  4, 1, 8);
	boar->addAnimation(BehaviorType::MOVE, Direction::EAST, "critters/boar/critter_boar_SE_run_strip.png",  4, 1, 8);
	boar->addAnimation(BehaviorType::MOVE, Direction::WEST, "critters/boar/critter_boar_NW_run_strip.png",  4, 1, 8);

	// Stag
	stag = new Character(1.0f, 1.4f);
	stag->addAnimation(BehaviorType::IDLE, Direction::NORTH, "critters/stag/critter_stag_SW_idle.png",  24, 1, 1);
	stag->addAnimation(BehaviorType::IDLE, Direction::SOUTH, "critters/stag/critter_stag_NE_idle.png",  24, 1, 1);
	stag->addAnimation(BehaviorType::IDLE, Direction::EAST, "critters/stag/critter_stag_SE_idle.png",  24, 1, 1);
	stag->addAnimation(BehaviorType::IDLE, Direction::WEST, "critters/stag/critter_stag_NW_idle.png",  24, 1, 1);
	stag->addAnimation(BehaviorType::MOVE, Direction::NORTH, "critters/stag/critter_stag_SW_run.png",  10, 1, 8);
	stag->addAnimation(BehaviorType::MOVE, Direction::SOUTH, "critters/stag/critter_stag_NE_run.png",  10, 1, 8);
	stag->addAnimation(BehaviorType::MOVE, Direction::EAST, "critters/stag/critter_stag_SE_run.png",  10, 1, 8);
	stag->addAnimation(BehaviorType::MOVE, Direction::WEST, "critters/stag/critter_stag_NW_run.png",  10, 1, 8);

	pauseButton = new Sprite("pause/pause-button.png", 0.99f, 0.99f);
	pauseButtonHover = new Sprite("pause/pause-button-hover.png", 0.99f, 0.99f);
}

void GameScene::update(double deltaTime, InputState inputState, Router* router, Game game) {
    state.update(inputState);

	mapDisplay.update(deltaTime, game);

    if(state.pauseButton == ButtonState::ACTIVE) {
        router->goTo(Scene::Pause);
    }

}


void GameScene::draw(double deltaTime, Game game)
{
	renderQueue.clear();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	GameEngine.mvMatrixStack.pushMatrix();
	// GameEngine.mvMatrixStack.addHomothety(Vector3D(0.1f, 0.1f, 0.1f));
	//std::cout << -game.getPlayerX() << "x" << game.getPlayerY() << "y" << std::endl;
	float player_x = game.getPlayerX();
	float player_y = game.getPlayerY();
	float iso_x_offset = (-player_x-player_y) * 0.50f;
	float iso_y_offset = (-player_y + player_x) * 0.25f;
	// std::cout << game.getPlayerX() << "x" << game.getPlayerY() << "y" << std::endl;
	GameEngine.mvMatrixStack.addTranslation(Vector3D(50.0f, -25.0f, 0.0f));
	GameEngine.mvMatrixStack.addTranslation(Vector3D(iso_x_offset, iso_y_offset, 0.0f));

	// Draw entities

	GameEngine.mvMatrixStack.pushMatrix();
	//GameEngine.mvMatrixStack.addHomothety(Vector3D(0.4f, 0.4f, 0.4f));

	mapDisplay.draw(deltaTime, game);

	for (const auto &entity : game.getEntitiesInfo())
	{
		GameEngine.mvMatrixStack.pushMatrix();
		float posY = entity.y ;
		float posX = entity.x - 100;

		float iso_x_entities = (posX + posY) * 0.50f;
		float iso_y_entities = (posY - posX) * 0.25f;
		
		// std::cout << posX << " x " << posY << "   -   " << iso_x_entities << " x " << iso_y_entities << std::endl;

		GameEngine.mvMatrixStack.addTranslation(Vector3D(iso_x_entities, iso_y_entities, 0.0f));
		GameEngine.updateMvMatrix();

		if (entity.type == EntityType::BOAR)
		{
			boar->draw(deltaTime, entity.behavior == BehaviorType::IDLE ? BehaviorType::IDLE : BehaviorType::MOVE, entity.direction);
		}
		else if (entity.type == EntityType::DEER)
		{
			stag->draw(deltaTime, BehaviorType::MOVE, entity.direction);
		}
		else if (entity.type == EntityType::WOLF)
		{
			wolf->draw(deltaTime, entity.behavior == BehaviorType::IDLE ? BehaviorType::IDLE : BehaviorType::MOVE, entity.direction);
		}
		GameEngine.mvMatrixStack.popMatrix();
	}
	

	GameEngine.mvMatrixStack.popMatrix();
	GameEngine.mvMatrixStack.popMatrix();

	// Draw player
	GameEngine.mvMatrixStack.pushMatrix();
	// GameEngine.mvMatrixStack.addHomothety(Vector3D(0.5f, 0.5f, 0.5f));
	GameEngine.mvMatrixStack.addTranslation(Vector3D(0.0f, 0.0f, 0.0f));
	// GameEngine.mvMatrixStack.addTranslation(Vector3D(-1.5f, -1.5f, 0.0f));
	GameEngine.updateMvMatrix();
	// badger->update(deltaTime);
	// badger->draw();
	badger->draw(deltaTime, game.getPlayerBehavior(), game.getPlayerDirection());
	GameEngine.mvMatrixStack.popMatrix();

	// Draw pause button
	GameEngine.mvMatrixStack.pushMatrix();
	GameEngine.mvMatrixStack.addTranslation(Vector3D(8.0f - 1.0f, 4.5f - 1.0f, 0.0f));
	GameEngine.updateMvMatrix();
	switch(state.pauseButton) {
		case ButtonState::HOVER:
			pauseButtonHover->draw();
			break;
		default:
			pauseButton->draw();
			break;
	}
	GameEngine.mvMatrixStack.popMatrix();

	// std::cout << "Number of flowers: " << nbr_fleurs << std::endl;
}
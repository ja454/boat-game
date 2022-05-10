#include "Game.h"

World::World(bool multiPlayer, Graphics& gfx)
{
	this->gfx.reset(&gfx);

	score = 0;

	spriteObject[BACKGROUND] = std::unique_ptr<Sprite>(new Sprite(1, this->gfx->pDevice));
	spriteObject[PLAYER_1] = std::unique_ptr<Sprite>(new Sprite(2, this->gfx->pDevice));

	spriteObject[COIN_1] = std::unique_ptr<Sprite>(new Sprite(3, this->gfx->pDevice));
	spriteObject[COIN_2] = std::unique_ptr<Sprite>(new Sprite(3, this->gfx->pDevice));
	spriteObject[COIN_3] = std::unique_ptr<Sprite>(new Sprite(3, this->gfx->pDevice));
	spriteObject[COIN_4] = std::unique_ptr<Sprite>(new Sprite(3, this->gfx->pDevice));
	spriteObject[COIN_5] = std::unique_ptr<Sprite>(new Sprite(3, this->gfx->pDevice));

	if (multiPlayer == MULTIPLAYER) {
		spriteObject[PLAYER_2] = std::unique_ptr<Sprite>(new Sprite(2, this->gfx->pDevice));
	}

	quadrant = TOPLEFT;
}

World::~World()
{
	gfx.release();
}

void World::input()
{
	float throttle = spriteObject[PLAYER_1]->throttle, rotation = spriteObject[PLAYER_1]->rotation, v = 0.1, dt = 0.5;
	float x = spriteObject[PLAYER_1]->position.x, y = spriteObject[PLAYER_1]->position.y;

	if (KEY('I')) { throttle += 1; }
	if (KEY('K')) { throttle -= 1; }
	if (KEY('J')) { rotation -= 0.025; }
	if (KEY('L')) { rotation += 0.025; }
	if (KEY('Q')) { spriteObject[PLAYER_2].reset(); }
	if (KEY('F')) { fireTorpedo(); }

	if (throttle > 100) { throttle = 100; }
	else if (throttle < 0) { throttle = 0; }

	spriteObject[PLAYER_1]->throttle = throttle;

	x += v * cos(rotation) * throttle;
	y += v * sin(rotation) * throttle;

	if (x > 1200) { x = 1200; }
	else if (x < 0) { x = 0; }

	if (y > 800) { y = 800; }
	else if (y < 0) { y = 0; }

	/*
	//switch(quadrant) {
	//
	//case TOPLEFT:

	//	if (y > 800) {
	//		y = 0;
	//		quadrant = BOTTOMLEFT;
	//	}
	//	else if (y < 0){
	//		y = 0;
	//	}

	//	if (x > 1200) {
	//		x = 0;
	//		quadrant = TOPRIGHT;
	//	}
	//	else if (x < 0) {
	//		x = 0;
	//	}

	//	backGround->imageDimensions.left = 0;
	//	backGround->imageDimensions.top = 0;
	//	backGround->imageDimensions.right = 1022/2;
	//	backGround->imageDimensions.bottom = 703/2;
	//
	//case TOPRIGHT:

	//	if (y > 800) {
	//		y = 0;
	//		quadrant = BOTTOMRIGHT;
	//	}
	//	else if (y < 0) {
	//		y = 0;
	//	}

	//	if (x > 1200) {
	//		x = 1200;
	//	}
	//	else if (x < 0) {
	//		x = 1200;
	//		quadrant = TOPLEFT;
	//	}

	//	backGround->imageDimensions.left = 1022/2;
	//	backGround->imageDimensions.top = 0;
	//	backGround->imageDimensions.right = 1022;
	//	backGround->imageDimensions.bottom = 703/2;

	//case BOTTOMLEFT:
	//	if (y > 800) {
	//		y = 800;
	//		quadrant = BOTTOMLEFT;
	//	}
	//	else if (y < 0){
	//		y = 800;
	//		quadrant = TOPLEFT;
	//	}

	//	if (x > 1200) {
	//		x = 0;
	//		quadrant = BOTTOMRIGHT;
	//	}
	//	else if (x < 0) {
	//		x = 0;
	//	}

	//	backGround->imageDimensions.left = 0;
	//	backGround->imageDimensions.top = 703/2;
	//	backGround->imageDimensions.right = 1022/2;
	//	backGround->imageDimensions.bottom = 703;

	//case BOTTOMRIGHT:

	//	backGround->imageDimensions.left = 703/2;
	//	backGround->imageDimensions.top = 1022/2;
	//	backGround->imageDimensions.right = 1022;
	//	backGround->imageDimensions.bottom = 703;
	//
	//}
	*/

	islandCollision(x, y);
	coinCollision(x, y);

	spriteObject[PLAYER_1]->SetRot(rotation);
	spriteObject[PLAYER_1]->SetPos(x, y);
}

void World::update()
{
	for (auto& x : spriteObject) {
		if (x != nullptr)
			x->Draw(gfx->pContext);
	}
}

void World::coinCollision(float& x, float& y)
{
	for (int i = COIN_1; i <= COIN_5; i++) {
		if (spriteObject[i] == nullptr) { continue; }

		if (ObjectIslandCol(spriteObject[i]->position.x, spriteObject[i]->position.y)) {
			std::pair<float, float> pos = randomPosition();
			spriteObject[i]->SetPos(pos.first, pos.second);
		}

		float distanceX = x - spriteObject[i]->position.x,
			distanceY = y - spriteObject[i]->position.y,
			minDistance = 50.0,
			distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

		if (distance <= minDistance) {
			score++;
			//If there is a collision with a coin then the coin is again randomly placed on the map.
			std::pair<float, float> pos = randomPosition();
			spriteObject[i]->SetPos(pos.first, pos.second);
		}
	}
	return;
}

void World::fireTorpedo()
{
	if (spriteObject[TORPEDO_MAIN] == nullptr && ammo > 0) {
		ammo--;

		spriteObject[TORPEDO_MAIN] = std::unique_ptr<Sprite>(new Sprite(4, gfx->pDevice));
		spriteObject[TORPEDO_MAIN]->SetPos(spriteObject[PLAYER_1]->position.x, spriteObject[PLAYER_1]->position.y);
		spriteObject[TORPEDO_MAIN]->SetRot(spriteObject[PLAYER_1]->rotation);
	}

	return;
}

void World::torpedoMovement()
{
	if (spriteObject[TORPEDO_MAIN] != nullptr) {
		float rotation = spriteObject[TORPEDO_MAIN]->rotation;
		spriteObject[TORPEDO_MAIN]->position.x += 10 * cos(rotation);
		spriteObject[TORPEDO_MAIN]->position.y += 10 * sin(rotation);
		float x = spriteObject[TORPEDO_MAIN]->position.x;
		float y = spriteObject[TORPEDO_MAIN]->position.y;

		if (x < 0 || x > 1200 || y < 0 || y > 800 || ObjectIslandCol(x, y)) {
			spriteObject[TORPEDO_MAIN].release();
		}
	}

	return;
}

void World::torpedoCollision()
{

}

void World::islandCollision(float& x, float& y)
{
	for (auto island : islandLocation) {
		float distanceX = x - island[0] * scaleX,
			distanceY = y - island[1] * scaleY,
			minDistance = island[2],
			distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

		if (distance <= (minDistance)) {
			float unitX = (distanceX) / (minDistance);
			float unitY = (distanceY) / (minDistance);
			x = island[0] * scaleX + (minDistance + 5.0) * unitX;
			y = island[1] * scaleY + (minDistance + 5.0) * unitY;
			return;
		}
	}
	return;
}

bool World::ObjectIslandCol(float x, float y)
{
	for (auto island : islandLocation) {
		float distanceX = x - island[0] * scaleX,
			distanceY = y - island[1] * scaleY,
			minDistance = island[2],
			distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

		if (distance <= (minDistance)) { return TRUE; }
	}
	return FALSE;
}

std::pair<float, float> randomPosition()
{
	//Outputs a random pair that will be used as the x and y coordinate.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(1.0, 1180.0);
	std::uniform_real_distribution<float> dist2(1.0, 780.0);

	return std::pair<int, int>(dist1(mt), dist2(mt));
}

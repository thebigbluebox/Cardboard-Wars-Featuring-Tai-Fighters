#include "Enemy.h"



Enemy::Enemy(void)
{
	position = Vector3();
	color = { 1, 1, 1 };
	size = 1;
	shape = Shape::Cube;
}

Enemy::Enemy(Vector3 pos)
{
	position = pos;
	color = { 1, 1, 1 };
	size = 1;
	shape = Shape::Cube;
}

EnemyHandler::EnemyHandler(void)
{
	lastUpdate = 0;
	spawnInterval = 200; // ms
}

void EnemyHandler::updateBullets(void)
{
	for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
	{
		if (bulletArray[i].speed > 0)
			bulletArray[i].location = bulletArray[i].location.add(bulletArray[i].direction);

		glPushMatrix();
		glTranslatef(bulletArray[i].location.x, bulletArray[i].location.y, bulletArray[i].location.z);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();
	}
}

void EnemyHandler::spawnBullet(void)
{
	bulletArray[0].location = { playerPos.x, playerPos.y, playerPos.z };
	bulletArray[0].speed = 1;
}

void EnemyHandler::update(Vector3 playerPos, float deltaTime)
{
	this->playerPos = playerPos;
	lastUpdate += deltaTime;

	//deltaTime = deltaTime / 50; // scaling the speed down
	if (list.size() < 10 || lastUpdate > spawnInterval) {
		lastUpdate = 0;
		//Enemy p = Enemy({ playerPos.x + randFloat(-10, 10), playerPos.y + randFloat(-10, 10), playerPos.z - 10 });
		Enemy p = Enemy({ playerPos.x + randFloat(-1, 1), playerPos.y + randFloat(-1, 1), playerPos.z - 10 });
		p.color = { randFloat(0, 1), randFloat(0, 1), randFloat(0, 1) };
		list.push_back(p);
		//printf("%f %f %f \n", p.position.x, p.position.y, p.position.z);
	}

	for (auto it = list.begin(); it != list.end();) {
		// nothing to do

		for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
		{
			if (bulletArray[i].location.x - bulletArray[i].hitbox < it->position.x
				&& bulletArray[i].location.x + bulletArray[i].hitbox > it->position.x
				&& bulletArray[i].location.y - bulletArray[i].hitbox < it->position.y
				&& bulletArray[i].location.y + bulletArray[i].hitbox > it->position.y
				&& bulletArray[i].location.z - bulletArray[i].hitbox < it->position.z
				&& bulletArray[i].location.z + bulletArray[i].hitbox > it->position.z)
				std::cout << "EXPLOSION";
		}

		// delete enemies behind the player
		if (it->position.z > playerPos.z) {
			it = list.erase(it);
		}
		else {
			++it;
		}
		
	}
}
#include "Enemy.h"

Enemy::Enemy(void)
{
	position = Vector3();
	color = { 1, 1, 1 };
	size = 1;
	shape = Shape::Sphere;
}

Enemy::Enemy(Vector3 pos)
{
	position = pos;
	color = { 1, 1, 1 };
	size = 1;
	shape = Shape::Sphere;
}

EnemyHandler::EnemyHandler(void)
{
	lastUpdate = 0;
	spawnInterval = 200; // ms
}

void EnemyHandler::update(Vector3 playerPos, float deltaTime)
{
	lastUpdate += deltaTime;

	//deltaTime = deltaTime / 50; // scaling the speed down
	if (list.size() < 10 || lastUpdate > spawnInterval) {
		lastUpdate = 0;
		Enemy p = Enemy({ playerPos.x + randFloat(-10, 10), playerPos.y + randFloat(-10, 10), playerPos.z - 10 });
		p.color = { randFloat(0, 1), randFloat(0, 1), randFloat(0, 1) };
		list.push_back(p);
		printf("%f %f %f \n", p.position.x, p.position.y, p.position.z);
	}

	for (auto it = list.begin(); it != list.end();) {
		// nothing to do
		// delete enemies behind the player
		if (it->position.z > playerPos.z) {
			it = list.erase(it);
		}
		else {
			++it;
		}
	}
}
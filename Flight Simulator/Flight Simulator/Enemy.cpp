#include "Enemy.h"

//this needs to be global (doesn't work otherwise)
ParticleSystem particleSystem;

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

void EnemyHandler::spawnBullet(Vector3 location, Vector3 direction)
{
	bulletArray[0].location = location;
	bulletArray[0].direction = direction;
	bulletArray[0].speed = 1;
}

void EnemyHandler::update(Vector3 playerPos, float deltaTime)
{
	
	this->playerPos = playerPos;
	lastUpdate += deltaTime;

	//deltaTime = deltaTime / 50; // scaling the speed down
	if (list.size() < 10 || lastUpdate > spawnInterval) {
		lastUpdate = 0;
		Enemy p = Enemy({ playerPos.x + randFloat(-10, 10), playerPos.y + randFloat(-10, 10), playerPos.z - 10 });
		p.color = { randFloat(0, 1), randFloat(0, 1), randFloat(0, 1) };
		list.push_back(p);
		//printf("%f %f %f \n", p.position.x, p.position.y, p.position.z);
	}

	for (auto it = list.begin(); it != list.end();) {
		// nothing to do
		bool eraseMe = false; //whether to erase the current enemy list element
		//check if enemy has hit player
		if (playerPos.x - 1 < it->position.x
			&& playerPos.x + 1 > it->position.x
			&& playerPos.y - 1 < it->position.y
			&& playerPos.y + 1 > it->position.y
			&& playerPos.z - 2 < it->position.z
			&& playerPos.z + 1 > it->position.z)
		{
			std::cout << "\nYou have been hit";
			eraseMe = true;
		}
		//check if bullet has hit enemy
		for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
		{
			if (bulletArray[i].location.x - bulletArray[i].hitbox < it->position.x
				&& bulletArray[i].location.x + bulletArray[i].hitbox > it->position.x
				&& bulletArray[i].location.y - bulletArray[i].hitbox < it->position.y
				&& bulletArray[i].location.y + bulletArray[i].hitbox > it->position.y
				&& bulletArray[i].location.z - bulletArray[i].hitbox < it->position.z
				&& bulletArray[i].location.z + bulletArray[i].hitbox > it->position.z)
			{
				particleSystem.setPosition(it->position.x, it->position.y, it->position.z);
				for (int i = 0; i < 50; i++)
					particleSystem.spawnParticle();
				eraseMe = true;
				
				bulletArray[i].location = playerPos;
				bulletArray[i].speed = 0;
			}
		}

		// delete enemies behind the player
		if (it->position.z > playerPos.z || eraseMe) {
			it = list.erase(it);
		}
		else {
			++it;
		}
		
	}
	//particleSystem.spawnParticle();
	particleSystem.updateParticles();
	
}

void EnemyHandler::drawEnemies(void)
{
	for (auto it = list.begin(); it != list.end(); ++it) {
		glPushMatrix();
		glColor3fv(it->color.v);
		glTranslatef(it->position.x, it->position.y, it->position.z);
		glutSolidCube(it->size);
		glPopMatrix();
	}
	particleSystem.drawParticles();
}
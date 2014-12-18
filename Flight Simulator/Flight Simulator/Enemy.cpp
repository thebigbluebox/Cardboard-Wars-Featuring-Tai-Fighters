#include "Enemy.h"
#include "TextureLoader.h"
#include "main.h"

//this needs to be global (doesn't work otherwise)
ParticleSystem particleSystem;

Enemy::Enemy(void)
{
	position = Vector3();
	color = { 0.5f, 0.5f, 0.5f };
	size = 1;
	shape = Shape::Cube;
}

Enemy::Enemy(Vector3 pos)
{
	position = pos;
	color = { 0.5f, 0.5f, 0.5f };
	size = 1;
	shape = Shape::Cube;
}

EnemyHandler::EnemyHandler(void)
{
	lastUpdate = 0;
	spawnInterval = 200; // ms
	ai = ONE;
	spawnEnemies = true;
}

void EnemyHandler::updateBullets(void)
{
	

	for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
	{
		//stop bullets that went too far
		if (bulletArray[i].location.z < playerPos.z - 30)
		{
			//bulletArray[i].speed = 0;
			//bulletArray[i].location.z = playerPos.z;
		}

		

		//move bullets that are in motion
		if (bulletArray[i].speed > 0)
			bulletArray[i].location = bulletArray[i].location.add(bulletArray[i].direction);

		//draw bullets
		glPushMatrix();
		glTranslatef(bulletArray[i].location.x, bulletArray[i].location.y, bulletArray[i].location.z);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();
	}

	//reload
	if (gameInfo.currentAmmo < 0)
	{
		for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray) - 1; i++)
		{
			bulletArray[i].speed = 0;
		}
		gameInfo.currentAmmo = sizeof(bulletArray) / sizeof(*bulletArray);
	}
}

void EnemyHandler::spawnBullet(Vector3 location, Vector3 direction)
{
	for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
	{
		if (bulletArray[i].speed == 0 )
		{
			bulletArray[i].location = location;
			bulletArray[i].direction = direction;
			bulletArray[i].speed = 1;
			gameInfo.currentAmmo -= 1;
			break;
		}
	}
	/*bulletArray[0].location = location;
	bulletArray[0].direction = direction;
	bulletArray[0].speed = 1;*/
}

void EnemyHandler::update(Vector3 playerPos, float deltaTime)
{
	
	this->playerPos = playerPos;
	lastUpdate += deltaTime;

	//every time the player position is divisible by 10 the ai updates
	if ((int)playerPos.z % 10 == 0 && (int)playerPos.z != lastAIUpdate)
	{
		lastAIUpdate = (int)playerPos.z;
		std::cout << "\nai update";
		//update the ai
		switch (ai)
		{
		case ONE:
			ai = TWO;
			break;
		case TWO:
			ai = THREE;
			break;
		case THREE:
			ai = FOUR;
			break;
		case FOUR:
			ai = ONE;
			break;
		default:
			ai = FOUR;
			break;
		}
	}
		

	//deltaTime = deltaTime / 50; // scaling the speed down
	if (list.size() < 10 || lastUpdate > spawnInterval && spawnEnemies) {
		lastUpdate = 0;
		Enemy p = Enemy({ playerPos.x + randFloat(-10, 10), playerPos.y + randFloat(-10, 10), playerPos.z - 20 });
		//p.color = { randFloat(0, 1), randFloat(0, 1), randFloat(0, 1) }; // randomize color
		list.push_back(p);
	}

	for (auto it = list.begin(); it != list.end();) {
		//bahave based on the ai
		if (ai == ONE)
		{
			it->position.x += 0.02 * sin(playerPos.z);
			it->position.y -= 0.02 * sin(playerPos.z);
		}
		if (ai == TWO)
		{
			if (it->position.x < playerPos.x)
				it->position.x += 0.01;
			if (it->position.x > playerPos.x)
				it->position.x -= 0.01;
		}
		if (ai == THREE)
		{
			if (it->position.x < playerPos.x + 400)
			{
				it->position.x += 0.02;
			}
			it->position.y += 0.04 * sin(it->position.x);
		}
		if (ai == FOUR)
		{
			if (it->rotation.z <= 359)
			{
				it->rotation.z += 5;
				spawnEnemies = false;
			}
			else
			{
				ai = ONE;
				spawnEnemies = true;
			}
		}
		//it->position.x += sin(playerPos.z);
		bool eraseMe = false; //whether to erase the current enemy list element
		//check if enemy has hit player
		if (playerPos.x - 1 < it->position.x
			&& playerPos.x + 1 > it->position.x
			&& playerPos.y - 1 < it->position.y
			&& playerPos.y + 1 > it->position.y
			&& playerPos.z - 2 < it->position.z
			&& playerPos.z + 1 > it->position.z)
		{
			gameInfo.lives -= 1;
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
				gameInfo.score += 1;
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
		Vector3 origin = { 0, 0, 0 };
		glPushMatrix();
		
		glTranslatef(it->position.x, it->position.y, it->position.z);

		glRotatef(it->rotation.z, 0, 0, 1);
		enemyModel(origin, it->color);
		glPopMatrix();
	}
	particleSystem.drawParticles();
}
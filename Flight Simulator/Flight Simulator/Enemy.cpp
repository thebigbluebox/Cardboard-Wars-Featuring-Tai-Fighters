#include "Enemy.h"
#include "TextureLoader.h"

//This is used for explosions
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
	//setup the bullet information
	bullet.location = pos;
	bullet.hitbox = 1;
	bullet.direction.x = 0;
	bullet.direction.y = 0;
	bullet.direction.z = 1;
	bullet.speed = 0;
	//pick a random ai to spawn with if the score is high enough
	if (gameInfo.score > 10)
	{
		if ((rand() % 20 + 1)> 1)
		{
			switch (rand() % gameInfo.level + 1)
			{
			case 1:
				ai = ONE;
				break;
			case 2:
				ai = TWO;
				break;
			case 3:
				ai = THREE;
				break;
			case 4:
				ai = FOUR;
				break;
			default:
				ai = ONE;
				break;
			}
		}
		else
		{
			ai = HEALTH;
			bullet.location.z = 50;
		}
	}
	if (gameInfo.level == 4)
		ai = FOUR;
}

EnemyHandler::EnemyHandler(void)
{
	lastUpdate = 0;
	spawnInterval = 400; // ms
	spawnEnemies = true;
}

/*Updates the player bullets*/
void EnemyHandler::updateBullets(void)
{
	for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
	{
		//check when bullets go too far. (unused)
		if (bulletArray[i].location.z < playerPos.z - 30)
		{
		}

		//move bullets that are in motion
		if (bulletArray[i].speed > 0)
			bulletArray[i].location = bulletArray[i].location.add(bulletArray[i].direction);

		//draw bullets
		glPushMatrix();
		glTranslatef(bulletArray[i].location.x, bulletArray[i].location.y, bulletArray[i].location.z);
		glColor3f(randFloat(0.2, 1), randFloat(0, 0.1), randFloat(0.2, 1));
		glutSolidSphere(1, 20, 20);
		glPopMatrix();
	}
}

/*Spawn player bullets*/
void EnemyHandler::spawnBullet(Vector3 location, Vector3 direction)
{
	for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray); i++)
	{
		//reload
		if (gameInfo.currentAmmo <= 0)
		{
			for (int i = 0; i < sizeof(bulletArray) / sizeof(*bulletArray) - 1; i++)
			{
				bulletArray[i].speed = 0;
				bulletArray[i].location.z *= -1;
			}
			gameInfo.currentAmmo = sizeof(bulletArray) / sizeof(*bulletArray);
			break;
		}

		//shoot
		else if (gameInfo.currentAmmo > 0)
		{
			bulletArray[i].location = location;
			bulletArray[i].direction = direction;
			bulletArray[i].speed = 1;
			gameInfo.currentAmmo -= 1;
			break;
		}
	}
}

/*Update enemies*/
void EnemyHandler::update(Vector3 playerPos, float deltaTime)
{
	this->playerPos = playerPos;
	lastUpdate += deltaTime;

	//every time the player position is divisible by a set amount the ai updates
	if ((int)playerPos.z % 15 == 0 && (int)playerPos.z != lastAIUpdate)
	{
		lastAIUpdate = (int)playerPos.z;
	}
		
	//when conditions are met spawn an enemy
	if (list.size() < 10 || lastUpdate > spawnInterval && spawnEnemies) {
		lastUpdate = 0;
		Enemy p = Enemy({ playerPos.x + randFloat(-10, 10), playerPos.y + randFloat(-10, 10), playerPos.z - 20 });
		list.push_back(p);
	}

	//iterate through enemy list
	for (auto it = list.begin(); it != list.end();) 
	{
		bool eraseMe = false; //whether to erase the current enemy list element

		//if the game is over delete the current enemy
		if (gameInfo.gameOver == true)
		{
			eraseMe = true;
		}

		//update enemy bullet location
		if (it->bullet.location.z > playerPos.z + 100 && !(it->ai == Enemy::HEALTH))
			it->bullet.location.z = it->position.z;
		it->bullet.location.x = it->position.x;
		it->bullet.location.y = it->position.y;
		if (!(it->ai == Enemy::HEALTH))
		{
			it->bullet.location.z += 0.8 * it->bullet.direction.z;
		}

		//move enemies based on their ai
		if (it->ai == Enemy::ONE)
		{
			it->position.x += 0.02 * sin(playerPos.z) + 0.01 * cos(playerPos.z);
			it->position.y -= 0.02 * sin(playerPos.z);
		}
		if (it->ai == Enemy::TWO)
		{
			if (it->position.x < playerPos.x)
				it->position.x += 0.01;
			if (it->position.x > playerPos.x)
				it->position.x -= 0.01;
		}
		if (it->ai == Enemy::THREE)
		{
			it->position.x += 0.06 * cos(it->position.z - playerPos.z);
			it->position.y += 0.06 * sin(it->position.x);
		}
		if (it->ai == Enemy::FOUR)
		{
			if (it->rotation.z + 5 <= 360)
			{
				it->rotation.z += 5;
			}
			else
			{
				//home in on player
				it->position.x += 0.02 * (playerPos.x - it->position.x);
				it->position.y += 0.02 * (playerPos.y - it->position.y);
			}
		}
		
		//check if enemy or health pack has bumped player
		if (playerPos.x - 1 < it->position.x
			&& playerPos.x + 1 > it->position.x
			&& playerPos.y - 1 < it->position.y
			&& playerPos.y + 1 > it->position.y
			&& playerPos.z - 2 < it->position.z
			&& playerPos.z + 1 > it->position.z)
		{
			eraseMe = true;
			if (!(it->ai == Enemy::HEALTH))
			{
				gameInfo.lives -= 1;
			}
			else
			{
				gameInfo.lives += 1;
			}
		}

		//check if enemy has shot player
		if (playerPos.x - 1 < it->bullet.location.x
			&& playerPos.x + 1 > it->bullet.location.x
			&& playerPos.y - 1 < it->bullet.location.y
			&& playerPos.y + 1 > it->bullet.location.y
			&& playerPos.z - 2 < it->bullet.location.z
			&& playerPos.z + 1 > it->bullet.location.z)
		{
			gameInfo.lives -= 1;
			it->bullet.location.z += 10; //move the bullet past the player to ensure it doesnt hit multiple times
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
				//spawn an explosion
				particleSystem.setPosition(it->position.x, it->position.y, it->position.z);
				for (int i = 0; i < 50; i++)
					particleSystem.spawnParticle();

				//delete current enemy
				eraseMe = true;
				
				//reset bullet
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
	//updae explosions
	particleSystem.updateParticles(); 
}

void EnemyHandler::drawEnemies(void)
{
	//iterate through enemy list
	for (auto it = list.begin(); it != list.end(); ++it) {
		Vector3 origin = { 0, 0, 0 };
		glPushMatrix();
		
		glTranslatef(it->position.x, it->position.y, it->position.z);
		glRotatef(it->rotation.z, 0, 0, 1);

		//color enemy based on the ai
		if (it->ai == Enemy::ONE)
			it->color = { randFloat(0, 0.4), randFloat(0, 0.4), 1 };
		if (it->ai == Enemy::TWO)
			it->color = { randFloat(0, 0.4), 1, randFloat(0, 0.4) };
		if (it->ai == Enemy::THREE)
			it->color = { randFloat(0.3, 0.4), randFloat(0.6, 0.9), randFloat(0.6, 0.7) };
		if (it->ai == Enemy::FOUR)
			it->color = { 1, randFloat(0, 0.4), randFloat(0, 0.4) };
		
		//check what to draw depending on ai
		if (!(it->ai == Enemy::HEALTH))
		{
			enemyModel(origin, it->color);
		}
		else
		{
			healthPack(origin);
		}
		glPopMatrix();

		//draw enemy bullet
		if (it->bullet.location.z < playerPos.z)
		{
			glPushMatrix();
			glColor3f(randFloat(0, 0.2), randFloat(0.3, 0.8), randFloat(0, 0.2));
			glTranslatef(it->bullet.location.x, it->bullet.location.y, it->bullet.location.z);
			glScalef(1, 1, 8);
			glutSolidCube(0.2);
			glPopMatrix();
		}
	}
	//draw the explosions
	particleSystem.drawParticles();
}
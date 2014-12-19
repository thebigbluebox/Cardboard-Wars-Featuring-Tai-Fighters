#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "stdafx.h"
#include "ParticleSystem.h"

struct _bullet {
	Vector3 location = { 0, 0, 0 };
	Vector3 direction = { 0, 0, 0 };
	float speed = 0;
	int hitbox = 1;
	int duration = 0;
	int maxDuration = 10;
};
typedef _bullet bullet;

class Enemy
{
public:
	enum Shape { Sphere, Cube, Teapot };
	Vector3 position;
	//Vector3 direction;
	Vector3 rotation;
	Vector3 color;
	float size;
	//float spin;
	Shape shape;
	Enemy(void);
	Enemy(Vector3 pos);
	bullet bullet;

	enum AI { ZERO, ONE, TWO, THREE, FOUR, FIVE, HEALTH }; //the numbers are different AI types 
	AI ai;
};


class EnemyHandler
{

	
public:
	std::vector<Enemy> list;
	
	//typedef std::list<Enemy>::iterator iter;
	//iter begin();
	//iter end();
	EnemyHandler(void);
	void update(Vector3 playerPos, float deltaTime);
	void clear(void);
	int lastUpdate;
	int spawnInterval;
	void updateBullets(void);
	void spawnBullet(Vector3 location, Vector3 direction);
	void drawEnemies(void);

private:
	Vector3 playerPos; //the location of the player
	bullet bulletArray[6]; //the player's bullets
	float lastAIUpdate; //the last time the AI changed
	bool spawnEnemies; //whether enemies should spawn

};

#endif

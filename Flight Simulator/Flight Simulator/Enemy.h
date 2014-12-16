#ifndef __ENEMY_H__
#define __ENEMY_H__
#include "stdafx.h"

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
};


class EnemyHandler
{
private:
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
};

#endif
#ifndef __MAIN_H__
#define __MAIN_H__

typedef struct {
	int score;
	int lives;
	int lastShotTime;
	int ammo;
	int currentAmmo;
	bool gameOver;
	bool leftCannon;
	double anglex;
	double angley;
	int level;
	bool shipLight;
} GameInfo;

extern GameInfo gameInfo;

extern GameInfo getGameInfo(void);

#endif
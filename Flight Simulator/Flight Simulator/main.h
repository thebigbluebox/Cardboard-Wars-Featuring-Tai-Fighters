#ifndef __MAIN_H__
#define __MAIN_H__

typedef struct {
	int score;
	int lives;
} GameInfo;

extern GameInfo gameInfo;

extern GameInfo getGameInfo(void);

// .cpp

#endif
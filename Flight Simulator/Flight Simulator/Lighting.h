#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include "stdafx.h"

typedef struct {
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess;
} Material;

void setMaterial(Material material);

#endif
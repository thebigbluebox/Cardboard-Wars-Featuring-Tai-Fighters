#include "Lighting.h"
#include "stdafx.h"

void setMaterial(Material material) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
}
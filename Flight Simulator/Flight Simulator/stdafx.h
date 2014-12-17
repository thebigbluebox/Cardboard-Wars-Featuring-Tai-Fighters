#ifndef __STDAFX_H__
#define __STDAFX_H__

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <gl/glut.h>
#endif

// C libraries
#include <stdio.h>
#include <time.h>
#include <math.h>

// C++ libraries
#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

// User libraries
#include "calculations.h"
#include "Vector3.h"
#include "Enemy.h"

#endif 
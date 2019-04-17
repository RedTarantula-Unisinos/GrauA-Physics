#pragma once
#include "Render.h"

#include <Box2D/Box2D.h>
#include <GLFW/glfw3.h>
#include <list>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>

struct BoxDefinition
{
	int w = 1;
	int h = 1;
	double d = 10.0;
	double f = 1.0;
	double r = 1.0;
};


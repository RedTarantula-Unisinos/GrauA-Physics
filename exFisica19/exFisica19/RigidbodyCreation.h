#include "Render.h"
#include "JStructs.h"

#include <list>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>

#pragma once


using namespace std;
class RigidbodyCreation
{
public:
	RigidbodyCreation();
	~RigidbodyCreation();

	b2Body * GetLastBodyCreated();

	b2Body * CreateBoxAtPosition(b2World* world, double posX = 0, double posY = 0, int width = 2, int height = 2, double density = 10.0, double friction = 0.5, double resistution = 0.5,bool addToList = false);
	b2Body * CreateCircleAtPosition(b2World* world, double posX, double posY, int radius, double density, double friction, double resistution, bool addToList = false);
	b2Body * CreateLineAtPosition(b2World* world, double posX, double posY, int posX2, int posY2, double density, double friction, double resistution, bool addToList = false);

	void SetBoxWidth(int w) { boxDef.w = w; };
	void SetBoxHeight(int h) { boxDef.h = h; };
	void SetBoxDensity(double d) { boxDef.d = d; };
	void SetBoxFriction(double f) { boxDef.f = f; };
	void SetBoxResistution(double r) { boxDef.r = r; };

	void AddBodyToList(b2Body*body) { createdBodies.push_front(body); };

	BoxDefinition GetBoxDefinition() { return boxDef; };

	void RandomizeBox(b2World* world, double mouseX, double mouseY);
	void RandomizeBall(b2World* world, double mouseX, double mouseY);
	void RandomizeLine(b2World* world, double mouseX, double mouseY);

	void PrintBoxDefinition();

	void DeleteAllBodies() { createdBodies.clear(); };
	void DeleteLastBody() { createdBodies.pop_front(); };

private:
	float32 timesteps;
	int32 velocityIterations, positionIterations;
	float xMin = -40.0f, xMax = 40.0f, yMin = -40.0f, yMax = 40.0f;

	BoxDefinition boxDef;
	list<b2Body*> createdBodies;
};


#include "Render.h"
#include "JStructs.h"
#include "MatVet.h"
#include <list>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>

#pragma once
class Spaceship
{
public:
	Spaceship();
	~Spaceship();

	void SpawnShip(b2World* world, double _x, double _y); // Creates the spaceship
	void DrainFuel(int amount) { fuel -= amount; };

	int GetShipFuel();
	float GetAngle();
	
	void DestroyShip();

	b2Body * GetBody() { return body; }

	void ShipDecelerate();
	void ShipMoveUp();
	void ShipMoveDown();
	void ShipRotateClockwise();
	void ShipRotateAntiClockwise();

	b2Vec2 GetGlobalPoint(b2Vec2 globalpos) { body->GetWorldPoint(globalpos) ;};

	void SetForceMagnitude(float magnitude) { forceMagnitude = magnitude; };
	void SetForceRotationCompensation(float compensation) { forceRotationCompensation = compensation; };

private:
	double x, y;
	float forceMagnitude, forceRotationCompensation;
	int fuel;
	bool alive;
	b2Body* body;
};


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
class Spaceship
{
public:
	Spaceship();
	~Spaceship();

	void SpawnShip(b2World* world, double _x, double _y); // Creates the spaceship
	void DrainFuel(int amount) { fuel -= amount; };

	int GetShipFuel();
	float GetAngle()
	{		
		float a = body->GetAngle();
		std::cout << "Got Angle: " << a << std::endl;
		return a;
	};
	
	void DestroyShip();

	b2Body * GetBody() { return body; }

	void ShipDecelerate() { body->SetAngularVelocity(body->GetAngularVelocity()*.9); }

private:
	double x, y;
	int fuel;
	bool alive;
	b2Body* body;
};


#include <Box2D/Box2D.h>
#include <iostream>
#include <cmath>
#include <string>

#pragma once
class Spaceship
{
public:
	Spaceship();
	~Spaceship();

	void SpawnShip(b2World* world, double _x, double _y); // Creates the spaceship
	void DrainFuel(int amount);

	int GetShipFuel() { return fuel; };
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

	b2Vec2 CalculaComponentesDoVetor(float magnitude, float angulo);

	float GrausParaRadianos(float angulo) {return angulo*b2_pi / 180;};
	float RadianosParaGraus(float angle) {return angle * 180 / b2_pi;};

	void CheckShipWithPlatform(int multiplier);
	float GetSpeed();

	double FixAngle(double ang);

	void Die() { if (!dead) { PrintWarning("You died"); } dead = true; };
	bool GetDead() { return dead; };

	void PrintShipStats();
	void PrintWarning(std::string warning);

private:
	double x, y;
	float forceMagnitude, forceRotationCompensation;
	int fuel,maxfuel;
	bool dead;
	bool fuelWarning50, fuelWarning25, fuelWarning0;
	b2Body* body;
};


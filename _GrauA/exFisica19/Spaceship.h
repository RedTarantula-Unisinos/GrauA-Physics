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
	void DrainFuel(int amount); // Drains fuel

	int GetShipFuel() { return fuel; }; // Returns the current ammount of fuel available
	float GetAngle(); // Returns the ship's rotation
	
	void DestroyShip(); // Not used yet

	b2Body * GetBody() { return body; } // Returns the ship's body

	void ShipDecelerate(); // SPACE
	void ShipMoveUp(); // W
	void ShipMoveDown(); // S
	void ShipRotateClockwise(); // A
	void ShipRotateAntiClockwise(); // D

	b2Vec2 GetGlobalPoint(b2Vec2 pos) { body->GetWorldPoint(pos) ;};

	void SetForceMagnitude(float magnitude) { forceMagnitude = magnitude; }; // Changes the impulse's magnitude
	void SetForceRotationCompensation(float compensation) { forceRotationCompensation = compensation; }; // Changes the impulse's magnitude for rotation

	b2Vec2 VectorComponent(float magnitude, float angulo); // Imported from MatVet.h
	float ToRad(float angulo) {return angulo*b2_pi / 180;}; // Imported from MatVet.h
	float ToDegrees(float angle) {return angle * 180 / b2_pi;}; // Imported from MatVet.h

	void CheckShipWithPlatform(int multiplier); // Checks the ship's conditions when in contact with a platform
	float GetSpeed(); // Returns the ship's speed

	double FixAngle(double ang); // Keeps the angle in values between 0 and 360 for convenience

	void Die() { if (!dead) { PrintWarning("You died"); } dead = true; }; // Kills the ship
	bool GetDead() { return dead; }; // Returns if the ship's dead or not

	void PrintShipStats(); // Prints the ship's speed, angle and fuel on the console
	void PrintWarning(std::string warning); // Prints warnings on the console

private:
	float forceMagnitude, forceRotationCompensation;
	int fuel, maxfuel;
	bool dead;
	bool fuelWarning75, fuelWarning50, fuelWarning25, fuelWarning0;
	b2Body* body;
};


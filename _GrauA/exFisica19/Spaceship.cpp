#include "Spaceship.h"



Spaceship::Spaceship()
{
}

Spaceship::~Spaceship()
{
}

void Spaceship::SpawnShip(b2World* world, double _x, double _y) // Init the ship's body, fixture and variables
{
	double x = _x;
	double y = _y;

	// Initializes the ship's variables
	fuel = 2000;
	maxfuel = 2000;
	dead = false;
	fuelWarning0 = false;
	fuelWarning25 = false;
	fuelWarning50 = false;
	fuelWarning75 = false;


	b2Body *box;
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	bodyDef.type = b2_dynamicBody;
	b2PolygonShape shape;
	shape.SetAsBox(2, 4);
	b2FixtureDef fix;
	fix.shape = &shape;
	fix.density = 1.0;
	fix.friction = 0.8;
	fix.restitution = 0.5;
	box = world->CreateBody(&bodyDef);
	box->CreateFixture(&fix);
	body = box;

	body->SetUserData(this);
}

void Spaceship::DrainFuel(int amount) // Drains the ship's fuel
{
	fuel -= amount;
	
	if (fuel <= 0 && !fuelWarning0) // Warns if fuel reaches 0%
	{
		PrintWarning("0% - You've used all your fuel");
		fuelWarning0 = true;
	}
	else if (fuel < maxfuel*.25 && !fuelWarning25) // Warns if fuel reaches 25%
	{
		PrintWarning("25% - You've used three quarters of your fuel");
		fuelWarning25 = true;
	}
	else if (fuel < maxfuel*.5 && !fuelWarning50) // Warns if fuel reaches 50%
	{
		PrintWarning("50% - You've used half of your fuel");
		fuelWarning50 = true;
	}
	else if (fuel < maxfuel*.75 && !fuelWarning75) // Warns if fuel reaches 50%
	{
		PrintWarning("75% - You've used a quarter of your fuel");
		fuelWarning75 = true;
	}

}


float Spaceship::GetAngle() // Returns the ship's angle
{
	return ToDegrees(body->GetAngle());;
};

void Spaceship::DestroyShip() // Unused
{
}

void Spaceship::ShipDecelerate() // Control SPACE
{
	if (!GetDead()) {
		b2Vec2 velocityLinear = body->GetLinearVelocity();
		velocityLinear.x *= .8; velocityLinear.y *= .8;
		body->SetLinearVelocity(velocityLinear); // Decreases linear velocity

		float32 velocityAngular = body->GetAngularVelocity();
		velocityAngular *= .9;
		body->SetAngularVelocity(velocityAngular); // Decreases angular velocity

		DrainFuel(2); // Drains fuel
	}
}

void Spaceship::ShipMoveUp() // Control W
{
	if (!GetDead()) {
		b2Vec2 localP = b2Vec2(0, 0); // Center
		b2Vec2 globalP = body->GetWorldPoint(localP);
		b2Vec2 force = VectorComponent(forceMagnitude, GetAngle() + 90);
		body->ApplyForce(force, globalP, true);
		DrainFuel(1); // Drains fuel
	}
}

void Spaceship::ShipMoveDown() // Control S
{
	if (!GetDead()) {
		b2Vec2 localP = b2Vec2(0, 0); // Center
		b2Vec2 globalP = body->GetWorldPoint(localP);
		b2Vec2 force = VectorComponent(forceMagnitude, GetAngle() + 270);
		body->ApplyForce(force, globalP, true);
		DrainFuel(1); // Drains fuel
	}
}

void Spaceship::ShipRotateClockwise() // Control D
{
	if (!GetDead()) {
		b2Vec2 localP = b2Vec2(1, 2);
		b2Vec2 globalP = body->GetWorldPoint(localP);
		b2Vec2 force = VectorComponent(forceMagnitude*forceRotationCompensation, GetAngle());
		body->ApplyForce(force, globalP, true);
		DrainFuel(1);
	}
}

void Spaceship::ShipRotateAntiClockwise() // Control A
{
	if (!GetDead()) {
		b2Vec2 localP = b2Vec2(-1, 2);
		b2Vec2 globalP = body->GetWorldPoint(localP);
		b2Vec2 force = VectorComponent(forceMagnitude*forceRotationCompensation, GetAngle() + 180);
		body->ApplyForce(force, globalP, true);
		DrainFuel(1);
	}
}

b2Vec2 Spaceship::VectorComponent(float magnitude, float angulo) // Imported from MatVet.h
{
	float v = magnitude;
	float angulorad = ToRad(angulo);
	float vx = v*cos(angulorad);
	float vy = v*sin(angulorad);
	b2Vec2 vec(vx, vy);
	return vec;
}

void Spaceship::CheckShipWithPlatform(int multiplier) // Checks the ship's conditions when in contact with a platform
{

	if (!GetDead()) {
		float angleToleration = 30;
		float speedToleration = 3.5f;

		PrintShipStats();

		if (GetSpeed() >= speedToleration) // Checks for the ship's speed at the moment of the contact
		{
			std::cout << "Too much speed" << std::endl;
			Die();
		}
		else
		{
			std::cout << "Good speed" << std::endl;

			if (FixAngle(GetAngle()) < 0 + angleToleration || FixAngle(GetAngle()) > 360 - angleToleration) // Checks for the ship's angle at the moment of the contact
			{
				std::cout << "Good angle" << std::endl;
				std::cout << FixAngle(GetAngle()) << std::endl;
				std::cout << "YOU GOT IT! Platforms multiplier: " << multiplier << std::endl;
			}
			else
			{
				std::cout << "Bad angle" << std::endl;
				std::cout << FixAngle(GetAngle()) << std::endl;
				Die();
			}
		}
	}
}

float Spaceship::GetSpeed() // Returns the ship's speed
{
	float32 x = body->GetLinearVelocity().x;
	float32 y = body->GetLinearVelocity().y;
	float speed = sqrt(pow(x, 2) + pow(y, 2));
	return speed;

}

double Spaceship::FixAngle(double ang) // Keeps the angle's value between 0 and 360
{
	
		const double width = 360 - 0;   // 
		const double offsetValue = ang - 0;   // value relative to 0

		return (offsetValue - (floor(offsetValue / width) * width)) + 0;
	
}

void Spaceship::PrintShipStats() // Prints stats on the console -> By pressing I
{
	std::cout
		<< "====================" << std::endl
		<< "=Your Ship's Status=" << std::endl
		<< "====================" << std::endl
		<< "=Speed: " << GetSpeed() << std::endl
		<< "=Angle: " << FixAngle(GetAngle()) << std::endl
		<< "=Fuel: " << GetShipFuel() << std::endl
		<< "====================" << std::endl;
}

void Spaceship::PrintWarning(std::string warning) // Prints warnings on the console
{

	std::cout << "(!) WARNING (!) - " << warning << std::endl;


}


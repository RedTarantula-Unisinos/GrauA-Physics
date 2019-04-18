#include "Spaceship.h"



Spaceship::Spaceship()
{
}


Spaceship::~Spaceship()
{
}

void Spaceship::SpawnShip(b2World* world, double _x, double _y)
{
	x = _x;
	y = _y;

	fuel = 1000;
	alive = true;


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

}

int Spaceship::GetShipFuel()
{
	return 0;
}

void Spaceship::DestroyShip()
{
}

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

float Spaceship::GetAngle()
{
	return RadianosParaGraus(body->GetAngle());;
};

void Spaceship::DestroyShip()
{
}

void Spaceship::ShipDecelerate()
{
	b2Vec2 velocityLinear = body->GetLinearVelocity();
	velocityLinear.x *= .9; velocityLinear.y *= .9;
	body->SetLinearVelocity(velocityLinear);

	float32 velocityAngular = body->GetAngularVelocity();
	velocityAngular *= .9;
	body->SetAngularVelocity(velocityAngular);
}

void Spaceship::ShipMoveUp() // W
{
	b2Vec2 localP = b2Vec2(0, 0);
	b2Vec2 globalP = body->GetWorldPoint(localP);
	b2Vec2 force = CalculaComponentesDoVetor(forceMagnitude, GetAngle() + 90);
	body->ApplyForce(force, globalP, true);

}

void Spaceship::ShipMoveDown() // S
{
	b2Vec2 localP = b2Vec2(0, 0);
	b2Vec2 globalP = body->GetWorldPoint(localP);
	b2Vec2 force = CalculaComponentesDoVetor(forceMagnitude, GetAngle() + 270);
	body->ApplyForce(force, globalP, true);
}

void Spaceship::ShipRotateClockwise() // D
{
	b2Vec2 localP = b2Vec2(1, 2);
	b2Vec2 globalP = body->GetWorldPoint(localP);
	b2Vec2 force = CalculaComponentesDoVetor(forceMagnitude*forceRotationCompensation, GetAngle());
	body->ApplyForce(force, globalP, true);
}

void Spaceship::ShipRotateAntiClockwise() // A
{
	b2Vec2 localP = b2Vec2(-1, 2);
	b2Vec2 globalP = body->GetWorldPoint(localP);
	b2Vec2 force = CalculaComponentesDoVetor(forceMagnitude*forceRotationCompensation, GetAngle() + 180);
	body->ApplyForce(force, globalP, true);
}

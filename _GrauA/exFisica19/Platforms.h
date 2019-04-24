#include <list>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <Box2D/Box2D.h>
#include <vector>


#pragma once
class Platforms
{
public:
	Platforms() {};
	~Platforms() {};
	b2Body * SpawnPlatform(b2World * world, double _x, double _y, double _length, int _multiplier)
	{
		x = _x - length / 2;
		y = _y;
		length = _length;

		b2Body *line;


		b2BodyDef bd;
		line = world->CreateBody(&bd);
		bd.type = b2_dynamicBody;

		b2EdgeShape shape;

		shape.Set(b2Vec2(x, y), b2Vec2(x+length, y));

		b2FixtureDef fix;
		fix.shape = &shape;
		fix.density = 1;

		line->CreateFixture(&shape,1);
		body = line;
		body->SetUserData(this);

		multiplier = _multiplier;

		return body;
	};

	double GetXmin() { return x; }
	double GetY() { return y; }
	double GetXmax() { return x+length; }
	double GetLength() { return length; }

	b2Body* GetBody() { return body; };

	void startContact() { contacting = true; }
	
	int GetMultiplier() { return multiplier; };

private:
	double x, y;
	double length;
	b2Body * body;
	bool contacting;
	int multiplier;

};


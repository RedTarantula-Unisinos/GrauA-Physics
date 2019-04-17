#include "RigidbodyCreation.h"



RigidbodyCreation::RigidbodyCreation()
{
}


RigidbodyCreation::~RigidbodyCreation()
{
}

b2Body * RigidbodyCreation::GetLastBodyCreated()
{

	b2Body * bb = createdBodies.front();

	return bb;
}

b2Body *RigidbodyCreation::CreateBoxAtPosition(b2World* world, double posX, double posY, int width, int height, double density, double friction, double resistution, bool addToList)
{
	std::cout << "trying to create box at " << posX << "x" << posY << std::endl;

	b2Body *box;


	b2BodyDef bodyDef;
	bodyDef.position.Set(posX, posY);
	bodyDef.type = b2_dynamicBody;


	b2PolygonShape shape;
	shape.SetAsBox(width, height);


	b2FixtureDef fix;
	fix.shape = &shape;

	fix.density = 10.0;
	fix.friction = 0.5;
	fix.restitution = 0.5;

	//Por fim, criamos o corpo...
	box = world->CreateBody(&bodyDef);
	//... e criamos a fixture do corpo 	
	box->CreateFixture(&fix);

	if (addToList) {AddBodyToList(box);	}

	return box;
}
b2Body *RigidbodyCreation::CreateCircleAtPosition(b2World* world, double posX, double posY, int radius, double density, double friction, double resistution, bool addToList)
{
	std::cout << "trying to create circle at " << posX << "x" << posY << std::endl;

	b2Body *circle;


	b2BodyDef bodyDef;
	bodyDef.position.Set(posX, posY);
	bodyDef.type = b2_dynamicBody;

	b2CircleShape shape;
	shape.m_radius = radius;




	b2FixtureDef fix;
	fix.shape = &shape;

	fix.density = 10.0;
	fix.friction = 0.5;
	fix.restitution = 0.5;

	//Por fim, criamos o corpo...
	circle = world->CreateBody(&bodyDef);
	//... e criamos a fixture do corpo 	
	circle->CreateFixture(&fix);

	if (addToList) { AddBodyToList(circle); }

	return circle;
}


b2Body *RigidbodyCreation::CreateLineAtPosition(b2World* world, double posX, double posY, int posX2, int posY2, double density, double friction, double resistution, bool addToList)
{

	std::cout << "trying to create line at " << posX << "x" << posY << std::endl;

	b2Body *line;


	b2BodyDef bd;
	line = world->CreateBody(&bd);
	bd.type = b2_dynamicBody;

	b2EdgeShape shape;

	shape.Set(b2Vec2(posX, posY), b2Vec2(posX2, posY2));

	b2FixtureDef fix;
	fix.shape = &shape;
	//Setamos outras propriedades da fixture
	fix.density = density;
	fix.friction = friction;
	fix.restitution = resistution;

	line->CreateFixture(&shape, density);

	if (addToList) { AddBodyToList(line); }

	return line;
}
void RigidbodyCreation::PrintBoxDefinition()
{
	std::cout << "=====================" << std::endl;
	std::cout << "=  BOX DEFINITIONS  =" << std::endl;
	std::cout << "=====================" << std::endl;
	std::cout << "= Size: " << boxDef.w << "x" << boxDef.h << std::endl;
	std::cout << "= Density: " << boxDef.d << std::endl;
	std::cout << "= Friction: " << boxDef.f << std::endl;
	std::cout << "= Resistution: " << boxDef.r << std::endl;
	std::cout << "---------------------" << std::endl << std::endl;

}


void RigidbodyCreation::RandomizeBox(b2World* world, double mouseX, double mouseY)
{
	std::cout << "trying to create random box" << std::endl;

	int rand_size = 1 + rand() % 12;
	double rand_density = 1 + rand() % 10;
	double rand_friction = (1 + rand() % 10) / 10;
	double rand_resistution = (1 + rand() % 10) / 10;

	createdBodies.push_front(CreateBoxAtPosition(world, mouseX, mouseY, rand_size, rand_size, rand_density, rand_friction, rand_resistution));
}

void RigidbodyCreation::RandomizeBall(b2World* world, double mouseX, double mouseY)
{
	std::cout << "trying to create randoom ball" << std::endl;

	int rand_radius = 1 + rand() % 12;
	double rand_density = 1 + rand() % 10;
	double rand_friction = (1 + rand() % 10) / 10;
	double rand_resistution = (1 + rand() % 10) / 10;

	createdBodies.push_front(CreateCircleAtPosition(world, mouseX, mouseY, rand_radius, rand_density, rand_friction, rand_resistution));
}

void RigidbodyCreation::RandomizeLine(b2World* world, double mouseX, double mouseY)
{
	std::cout << "trying to create random line" << std::endl;

	double distance = 2 + rand() % 30;

	double rand_x = rand() % 30 - distance;
	double rand_y = rand() % 30 - distance;

	double rand_density = 1 + rand() % 10;
	double rand_friction = (1 + rand() % 10) / 10;
	double rand_resistution = (1 + rand() % 10) / 10;

	createdBodies.push_front(CreateLineAtPosition(world, mouseX, mouseY, rand_x, rand_y, rand_density, rand_friction, rand_resistution));
}
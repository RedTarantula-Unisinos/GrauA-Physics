#include "JStructs.h"
#include "RigidbodyCreation.h"
#include "MatVet.h"
#include "Spaceship.h"
#include "Manager.h"
#include "Platforms.h"

using namespace std;

Manager manager;

Spaceship playerShip;
Platforms platform1;

RigidbodyCreation rbcreate;
DebugDraw renderer;
b2World *world;
b2Body* ground,*wall1,*wall2,*ceiling;

float scalingFactor = 6.0f;
double xMPos = 0, yMPos = 0;
int wind_width = 640;
int wind_height = 480;

int changingMode = 0;
double ex4Res = 0;
bool ex5CreatedRamp = false;
double ex5Friction = 0;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	0 - Width and Height
	1- Width
	2- Height
	3- Density
	4- Friction
	5 - Resistution
	*/

	double mX = 0;
	double mY = 0;

	b2Vec2 force;
	b2Body * shipBody = playerShip.GetBody();

	b2Vec2 localP, globalP;

	const float forceMagnitude = 800;
	const float magnitudeDivide = 1.2;

	float angle = RadianosParaGraus(playerShip.GetAngle());

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_REPEAT) // Applies force upward
	{
		localP = b2Vec2(0, 0);
		globalP = playerShip.GetBody()->GetWorldPoint(localP);
		force = CalculaComponentesDoVetor(forceMagnitude, angle+90);
		playerShip.GetBody()->ApplyForce(force, globalP, true);
		std::cout << "Pulse Angle: " << angle+90 << std::endl;
	}

	if (key == GLFW_KEY_A && action == GLFW_REPEAT) // Applies force leftward
	{
		localP = b2Vec2(-1, 2);
		globalP = playerShip.GetBody()->GetWorldPoint(localP);
		force = CalculaComponentesDoVetor(forceMagnitude/magnitudeDivide, angle+180);
		playerShip.GetBody()->ApplyForce(force, globalP, true);
		std::cout << "Pulse Angle: " << angle+180 << std::endl;
	}

	if (key == GLFW_KEY_S && action == GLFW_REPEAT) // Applies force downward
	{
		localP = b2Vec2(0, 0);
		globalP = playerShip.GetBody()->GetWorldPoint(localP);
		force = CalculaComponentesDoVetor(forceMagnitude, angle+270);
		playerShip.GetBody()->ApplyForce(force, globalP, true);
		std::cout << "Pulse Angle: " << angle+270 << std::endl;
	}

	if (key == GLFW_KEY_D && action == GLFW_REPEAT) // Applies force rightward
	{
		localP = b2Vec2(1, 2);
		globalP = playerShip.GetBody()->GetWorldPoint(localP);
		force = CalculaComponentesDoVetor(forceMagnitude/magnitudeDivide, angle);
		playerShip.GetBody()->ApplyForce(force, globalP, true);
		std::cout << "Pulse Angle: " << angle << std::endl;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
	{
		playerShip.ShipDecelerate();
	}

}

void InitBox2D()
{

	b2Vec2 gravity(0.0f, -1.62f);

	world = new b2World(gravity);

	manager.SetVelocityIt(10);
	manager.SetPositionIt(8);
	manager.SetTimeSteps(1.0f / 60.0f); // 60 FPS

	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	wall1 = world->CreateBody(&bd);
	wall2 = world->CreateBody(&bd);
	ceiling = world->CreateBody(&bd);

	b2EdgeShape shape;

	shape.Set(b2Vec2(-50, -39.5), b2Vec2(50, -39.5));
	ground->CreateFixture(&shape, 1.0);
	shape.Set(b2Vec2(-50, 39.5), b2Vec2(50, 39.5));
	ceiling->CreateFixture(&shape, 1.0);
	shape.Set(b2Vec2(-50, -39.5), b2Vec2(-50, 39.5));
	wall1->CreateFixture(&shape, 1.0);
	shape.Set(b2Vec2(50, -39.5), b2Vec2(50, 39.5));
	wall2->CreateFixture(&shape, 1.0);

}

void InitGameStuff()
{
	playerShip.SpawnShip(world, 0, 0); // Spawns the spaceship at the center
	platform1.SpawnPlatform(world, 20, 20, 5);
}

void RunBox2D()
{
	world->Step(manager.GetTimeSteps(), manager.GetVelocityIt(), manager.GetPositionIt());
	world->ClearForces();
}

void DrawFixture(b2Fixture* fixture, b2Color color)
{
	const b2Transform& xf = fixture->GetBody()->GetTransform();
	switch (fixture->GetType())
	{
	case b2Shape::e_circle:
	{
		b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

		b2Vec2 center = b2Mul(xf, circle->m_p);
		float32 radius = circle->m_radius;

		renderer.DrawCircle(center, radius, color);
	}
	break;

	case b2Shape::e_polygon:
	{
		b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
		int32 vertexCount = poly->m_count;
		b2Assert(vertexCount <= b2_maxPolygonVertices);
		b2Vec2 vertices[b2_maxPolygonVertices];

		for (int32 i = 0; i < vertexCount; ++i)
		{
			vertices[i] = b2Mul(xf, poly->m_vertices[i]);
		}

		renderer.DrawPolygon(vertices, vertexCount, color);
	}

	break;
	case b2Shape::e_edge:
	{
		b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
		int32 vertexCount;

		b2Vec2 vertices[b2_maxPolygonVertices];
		int i = 0;

		if (edge->m_hasVertex0)
		{
			vertices[i] = b2Mul(xf, edge->m_vertex0);
			i++;
		}
		vertices[i] = b2Mul(xf, edge->m_vertex1); i++;
		vertices[i] = b2Mul(xf, edge->m_vertex2); i++;
		if (edge->m_hasVertex3)
		{
			vertices[i] = b2Mul(xf, edge->m_vertex3);
			i++;
		}

		vertexCount = i;
		renderer.DrawPolygon(vertices, vertexCount, color);
	}

	break;

	}

}

void DrawBody(b2Body *b, b2Color color)
{
	b2Fixture *f;
	for (f = b->GetFixtureList(); f; f = f->GetNext())
	{
		DrawFixture(f, color);
	}
}

void Render()
{
	b2Body *b;
	glColor3f(1, 0, 0);
	glPointSize(5);
	glLineWidth(3);

	b2Color color;
	color.r = 1.0;
	color.g = 0.0;
	color.b = 0.0;

	//PERCORRE A LISTA DE CORPOS RÍGIDOS DO MUNDO E CHAMA A ROTINA DE DESENHO PARA A LISTA DE FIXTURES DE CADA UM
	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		DrawBody(b, color);
	}
}

int main(void)
{
	srand(time(0));

	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(wind_width, wind_height, "Moon Lander", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);

	glfwSetKeyCallback(window, key_callback);


	InitBox2D(); // Initiates Box2D
	InitGameStuff(); // Setups the game's scene

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetCursorPos(window,&xMPos,&yMPos);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (width >= height)
		{
			ratio = width / (float)height;
			gluOrtho2D(manager.GetXMin()*ratio, manager.GetXMax()*ratio, manager.GetYMin(), manager.GetYMax());
		}
		else
		{
			ratio = height / (float)width;
			gluOrtho2D(manager.GetXMin(), manager.GetXMax(), manager.GetYMin()*ratio, manager.GetYMax()*ratio);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		RunBox2D();
		Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}


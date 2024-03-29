#include "JStructs.h"
#include "RigidbodyCreation.h"
#include "Manager.h"
#include <GL/glu.h>
#include "Shenanigans.h"
#include "ContactListener.h"

using namespace std;

Manager manager;

Spaceship playerShip;
Platforms platform1, platform2, platform3;

RigidbodyCreation rbcreate;
DebugDraw renderer;
b2World *world;
b2Body* ground,*wall1,*wall2,*ceiling;

float scalingFactor = 6.0f;
double xMPos = 0, yMPos = 0;
int wind_width = 640;
int wind_height = 480;



static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	float angle = RadianosParaGraus(playerShip.GetAngle());

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_W && action == GLFW_REPEAT) // Applies force upward
	{
		playerShip.ShipMoveUp();
	}

	if (key == GLFW_KEY_A && action == GLFW_REPEAT) // Applies force leftward
	{
		playerShip.ShipRotateAntiClockwise();
	}

	if (key == GLFW_KEY_S && action == GLFW_REPEAT) // Applies force downward
	{
		playerShip.ShipMoveDown();
	}

	if (key == GLFW_KEY_D && action == GLFW_REPEAT) // Applies force rightward
	{
		playerShip.ShipRotateClockwise();
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT) // Decelerates
	{
		playerShip.ShipDecelerate();
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) // Prints ship's info to console
	{
		playerShip.PrintShipStats();
	}

}

void InitBox2D()
{
	b2Vec2 gravity(0.0f, -1.62f);

	world = new b2World(gravity);

	world->SetContactListener(new ContactListener());

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
	playerShip.SetForceMagnitude(800);
	playerShip.SetForceRotationCompensation(.5);
	platform1.SpawnPlatform(world, 20, -39, 4, 5); // Spawns a platform
	platform2.SpawnPlatform(world, -10, -39, 7, 2); // Spawns a platform
	platform3.SpawnPlatform(world, -35, -39, 10, 1); // Spawns a platform
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

	b2Color colorPlatforms;
	colorPlatforms.r = 0.0;
	colorPlatforms.g = 0.5;
	colorPlatforms.b = 0.0;

	b2Color colorShip;
	colorShip.r = 1.0;
	colorShip.g = 0.0;
	colorShip.b = 0.0;

	b2Color colorScene;
	colorScene.r = 0.2;
	colorScene.g = 0.2;
	colorScene.b = 0.2;

	DrawBody(platform1.GetBody(), colorPlatforms);
	DrawBody(platform2.GetBody(), colorPlatforms);
	DrawBody(platform3.GetBody(), colorPlatforms);

	DrawBody(playerShip.GetBody(), colorShip);

	DrawBody(wall1, colorScene);
	DrawBody(wall2, colorScene);
	DrawBody(ceiling, colorScene);
	DrawBody(ground, colorScene);

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


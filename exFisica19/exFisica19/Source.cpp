#include <cstdlib>
#include <iostream>
#include "Render.h" //é a Render que está incluindo a glfw!
#include <ctime>
#include <list>
#include <string>

using namespace std;

static void cursorPosCallback(GLFWwindow* window, double posX, double posY);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

double xMPos = 0, yMPos = 0;
int wind_width = 640;
int wind_height = 480;

struct BoxDefinition
{
	int w = 1;
	int h = 1;
	double d = 10.0;
	double f = 1.0;
	double r = 1.0;
};


class GameManager
{
public:
	GameManager() {};
	~GameManager() {};

	float32 GetTimeSteps() { return timesteps; };
	void SetTimeSteps(float32 _timesteps) { timesteps = _timesteps; };

	int32 GetVelocityIt() { return velocityIterations; };
	void SetVelocityIt(int32 _vi) { velocityIterations = _vi; };
	int32 GetPositionIt() { return positionIterations; };
	void SetPositionIt(int32 _pi) { positionIterations = _pi; };

	float GetXMin() { return xMin; };
	void SetXMin(float _n) { xMin = _n; };

	float GetXMax() { return xMax; };
	void SetXMax(float _n) { xMax = _n; };

	float GetYMin() { return yMin; };
	void SetYMin(float _n) { yMin = _n; };

	float GetYMax() { return yMax; };
	void SetYMax(float _n) { yMax = _n; };

	b2Body * CreateBoxAtPosition(double posX = 0, double posY = 0, int width = 2, int height = 2, double density = 10.0, double friction = 0.5, double resistution = 0.5);

	void SetBoxWidth(int w) { boxDef.w = w; };
	void SetBoxHeight(int h) { boxDef.h = h; };
	void SetBoxDensity(double d) { boxDef.d = d; };
	void SetBoxFriction(double f) { boxDef.f = f; };
	void SetBoxResistution(double r) { boxDef.r = r; };

	BoxDefinition GetBoxDefinition() { return boxDef; }

	void PrintBoxDefinition();
private:
	float32 timesteps;
	int32 velocityIterations, positionIterations;
	float xMin = -40.0f, xMax = 40.0f, yMin = -40.0f, yMax = 40.0f;

	BoxDefinition boxDef;
};



GameManager manager;

b2World *world;

//Alguns corpos rígidos
b2Body *box1, *box2;
b2Body* ground;

float scalingFactor = 6.0f;

list<b2Body*> createdBodies;

//Objeto para a classe que faz o desenho das formas de colisão dos corpos rígidos
DebugDraw renderer;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

int changingMode = 0;
//Callback de teclado - PADRÃO DA GLFW - alterar conforme desejar (teclas de controle do programa)
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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_KP_ADD || key == GLFW_KEY_EQUAL) && action == GLFW_PRESS) {

		BoxDefinition boxDef = manager.GetBoxDefinition();

		int current_boxWidth = boxDef.w;
		int current_boxHeight = boxDef.h;
		double current_boxDensity = boxDef.d;
		double current_boxFriction = boxDef.f;
		double current_boxResistution = boxDef.r;

		switch (changingMode)
		{
		case 0: manager.SetBoxWidth(current_boxWidth + 1); manager.SetBoxHeight(current_boxHeight + 1); break;
		case 1: manager.SetBoxWidth(current_boxWidth + 1); break;
		case 2: manager.SetBoxHeight(current_boxHeight + 1); break;
		case 3: manager.SetBoxDensity(current_boxDensity + 5); break;
		case 4: manager.SetBoxFriction(current_boxFriction + 1); break;
		case 5: manager.SetBoxResistution(current_boxResistution + 1); break;
		default:
			break;
		}
		manager.PrintBoxDefinition();

	}
	if ((key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) && action == GLFW_PRESS) {

		BoxDefinition boxDef = manager.GetBoxDefinition();

		int current_boxWidth = boxDef.w;
		int current_boxHeight = boxDef.h;
		double current_boxDensity = boxDef.d;
		double current_boxFriction = boxDef.f;
		double current_boxResistution = boxDef.r;

		switch (changingMode)
		{
		case 0: if (current_boxWidth > 1 && current_boxHeight > 1) { manager.SetBoxWidth(current_boxWidth - 1); manager.SetBoxHeight(current_boxHeight + 1); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		case 1: if (current_boxWidth>1) {	manager.SetBoxWidth(current_boxWidth - 1); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		case 2: if (current_boxHeight>1) {	manager.SetBoxHeight(current_boxHeight - 1); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		case 3: if (current_boxDensity > 5) {	manager.SetBoxDensity(current_boxDensity - 5); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		case 4: if (current_boxFriction > 1) { manager.SetBoxFriction(current_boxFriction - 1); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		case 5: if (current_boxResistution > 1) {	manager.SetBoxResistution(current_boxResistution - 1); }else {std::cout << "(!) Reached the minimum value" << std::endl;} break;
		default:
			break;
		}
		manager.PrintBoxDefinition();

	}
	if ((key == GLFW_KEY_0 || key == GLFW_KEY_KP_0) && action == GLFW_PRESS)
	{
		if (changingMode == 5)
		{
			changingMode = 0;
		}
		else
		{
			changingMode++;
		}

		switch (changingMode)
		{
		case 0: std::cout << "(!) Now modifying: width and height" << std::endl; break;
		case 1: std::cout << "(!) Now modifying: width" << std::endl; break;
		case 2: std::cout << "(!) Now modifying: height" << std::endl; break;
		case 3: std::cout << "(!) Now modifying: density" << std::endl; break;
		case 4: std::cout << "(!) Now modifying: friction" << std::endl; break;
		case 5: std::cout << "(!) Now modifying: resistution" << std::endl; break;
		default:
			break;
		}
	}
}




void InitBox2D()
{

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -9.8f);

	// Inicializa a biblioteca Box2D
	world = new b2World(gravity);

	// Define os parâmetro para a simulação
	manager.SetVelocityIt(10);
	manager.SetPositionIt(8);
	manager.SetTimeSteps(1.0f / 60.0f); // 60 FPS


	box1 = manager.CreateBoxAtPosition(-10, 10,2,2);


	//Cria o chão
	//criação do corpo rígido
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	//Forma do chão: edge
	b2EdgeShape shape;
	shape.Set(b2Vec2(-39.5, -39.5), b2Vec2(39.5, -39.5));
	ground->CreateFixture(&shape, 1.0);

}


void RunBox2D()
{
	world->Step(manager.GetTimeSteps(), manager.GetVelocityIt(), manager.GetPositionIt());
	world->ClearForces();
}


void PrintBodies()
{
	b2Body *b;
	float ang;
	b2Vec2 pos;
	//PERCORRE A LISTA DE CORPOS RÍGIDOS DO MUNDO
	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		pos = b->GetPosition();
		ang = b->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", pos.x, pos.y, ang);
	}
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

// Programa Principal 
int main(void)
{
	srand(time(0));

	//Inicialização da janela da aplicação
	GLFWwindow* window;

	//Setando a callback de erro
	glfwSetErrorCallback(error_callback);

	//Inicializando a lib
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Criando a janela
	window = glfwCreateWindow(wind_width, wind_height, "HELLO GLFW!! BYE BYE GLUT!!!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Colocando a janela criada como sendo o contexto atual
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Setando a callback de teclado
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);

	glfwSetCursorEnterCallback(window, cursorEnterCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glfwSetKeyCallback(window, key_callback);


	InitBox2D();


	while (!glfwWindowShouldClose(window)) //loop da aplicação :)
	{
		float ratio;
		int width, height;

		//aqui recupera o tamanho atual da janela, para correção do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		//setando a viewport da OpenGL, para ocupar toda a janela da aplicação
		glViewport(0, 0, width, height);

		// Limpa a janela de visualização com a cor branca
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetCursorPos(window,&xMPos,&yMPos);

		//Setando a matriz de projeção, para definir o Ortho2D (câmera ortográfica 2D), respeitando o aspect ratio
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

		//Setando a matriz de modelo, para mandar desenhar as primitivas
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Mandando simular e desenhar 	
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

b2Body *GameManager::CreateBoxAtPosition(double posX, double posY, int width, int height, double density, double friction, double resistution)
{
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

	return box;
}

void GameManager::PrintBoxDefinition()
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

void cursorPosCallback(GLFWwindow * window, double posX, double posY)
{

	//std::cout << posX << " - " << posY << std::endl;

}

void cursorEnterCallback(GLFWwindow * window, int entered)
{
	/*if (entered)
	{
		std::cout << "Entered Window" << std::endl;
	}
	else
	{
		std::cout << "Left Window" << std::endl;
	}*/

}

void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	double mX = 0;
	double mY = 0;

	BoxDefinition boxDef = manager.GetBoxDefinition();

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Left button pressed " << std::endl;
		glfwGetCursorPos(window, &mX, &mY);

		double spawnXPos = (mX - (wind_width / 2)) ;
		double spawnYPos = (mY - (wind_height / 2)) ;

		createdBodies.push_front(manager.CreateBoxAtPosition(spawnXPos / scalingFactor, -spawnYPos / scalingFactor, boxDef.w, boxDef.h, boxDef.d, boxDef.f, boxDef.r));
	}


}

void scrollCallback(GLFWwindow * window, double xOffset, double yOffset)
{
	/*std::cout << xOffset << ":" << yOffset << std::endl;*/


}

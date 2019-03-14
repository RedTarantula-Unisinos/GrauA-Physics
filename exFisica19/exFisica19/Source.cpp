#include <cstdlib>
#include <iostream>
#include "Render.h" //é a Render que está incluindo a glfw!
#include <ctime>
#include <list>
#include <string>

using namespace std;

static void glfwSetCursorPosCallback(GLFWwindow* window, double posX, double posY);
void cursorEnterCallback(GLFWwindow* window, int entered);

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

	b2Body *CreateBoxAtMouse(float mouseX, float mouseY);
	b2Body *CreateBoxAtPosition(double posX, double posY);
private:
	float32 timesteps;
	int32 velocityIterations, positionIterations;
	float xMin = -40.0f, xMax = 40.0f, yMin = -40.0f, yMax = 40.0f;
};



GameManager manager;

b2World *world;

//Alguns corpos rígidos
b2Body *box1, *box2;
b2Body* ground;

list<b2Body*> createdBodies;

//Objeto para a classe que faz o desenho das formas de colisão dos corpos rígidos
DebugDraw renderer;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//Callback de teclado - PADRÃO DA GLFW - alterar conforme desejar (teclas de controle do programa)
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		std::cout << "Pressed B" << endl;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		createdBodies.push_front(manager.CreateBoxAtMouse(xpos, ypos));
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


	box1 = manager.CreateBoxAtPosition(-10, 10);


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
	window = glfwCreateWindow(640, 480, "HELLO GLFW!! BYE BYE GLUT!!!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Colocando a janela criada como sendo o contexto atual
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Setando a callback de teclado
	glfwSetCursorPosCallback(window, cursorposition);
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

b2Body *GameManager::CreateBoxAtMouse(float mouseX, float mouseY)
{
	std::cout << "In CreateBoxAtMouse function\nMouseX: " + to_string(mouseX) + " - " + to_string(mouseY) << std::endl;
	b2Body *box;

	double xpos, ypos;
	//getting cursor position

	//Primeiro, criamos a definição do corpo
	b2BodyDef bodyDef;
	bodyDef.position.Set(mouseX, mouseY);
	bodyDef.type = b2_dynamicBody;

	//Estamos usando uma forma de poligono, que pode ter até 8 vértices
	b2PolygonShape shape;
	shape.SetAsBox(5, 5);

	//Depois, criamos uma fixture que vai conter a forma do corpo
	b2FixtureDef fix;
	fix.shape = &shape;
	//Setamos outras propriedades da fixture
	fix.density = 10.0;
	fix.friction = 0.5;
	fix.restitution = 0.5;

	//Por fim, criamos o corpo...
	box = world->CreateBody(&bodyDef);
	//... e criamos a fixture do corpo 	
	box->CreateFixture(&fix);

	return box;

}

b2Body *GameManager::CreateBoxAtPosition(double posX, double posY)
{
	b2Body *box;

	//Primeiro, criamos a definição do corpo
	b2BodyDef bodyDef;
	bodyDef.position.Set(posX, posY);
	bodyDef.type = b2_dynamicBody;

	//Estamos usando uma forma de poligono, que pode ter até 8 vértices
	b2PolygonShape shape;
	shape.SetAsBox(5, 5);

	//Depois, criamos uma fixture que vai conter a forma do corpo
	b2FixtureDef fix;
	fix.shape = &shape;
	//Setamos outras propriedades da fixture
	fix.density = 10.0;
	fix.friction = 0.5;
	fix.restitution = 0.5;

	//Por fim, criamos o corpo...
	box = world->CreateBody(&bodyDef);
	//... e criamos a fixture do corpo 	
	box->CreateFixture(&fix);

	return box;
}

void glfwSetCursorPosCallback(GLFWwindow * window, double posX, double posY)
{



}

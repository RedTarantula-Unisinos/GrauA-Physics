//*****************************************************
//
// Projeto OpenGL2 + GLFW + Box2D
//
// Finalmente, removendo a GLUT da hist�ria :D
//
// Adaptado do projeto com GLUT por Rossana B. Queiroz 08/2011
// �ltima modifica��o: 05/09/2016
//*****************************************************

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Render.h" //� a Render que est� incluindo a glfw!
#include "MatVet.h" //Para decompor vetor, transformar grau em radianos.. sugest�o: usar a glm!

using namespace std;

//Algumas globais para interface e simula��o
int32 framePeriod = 16;
int32 mainWindow;
float scaleFactor;
int altura = 450, largura = 450;
float32 timeStep;
int32 velocityIterations;
int32 positionIterations;
int tx, ty, tw, th;
b2Vec2 viewCenter(0.0f, 0.0f);
float32 viewZoom = 1.0f;

// O objeto World serve para armazenar os dados da simula��o --> MUNDO F�SICO DA BOX2D
b2World *world;

//Alguns corpos r�gidos
//Objetos ch�o, paredes e teto
b2Body* chao;
b2Body* paredeEsquerda;
b2Body* paredeDireita;
b2Body* teto;
//Objeto caixa
b2Body *caixa;

//Objeto para a classe que faz o desenho das formas de colis�o dos corpos r�gidos
DebugDraw renderer;


//Callback de erro 
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//Callback de teclado
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		b2Vec2 vetorForca;
		vetorForca = CalculaComponentesDoVetor(10000, 45);
		
		// Descomentar estes blocos separadamente (e recomentar os anteriores)
		//----------------------------------------------------
		//Para aplicar a for�a no centro de massa do objeto
		caixa->ApplyForceToCenter(vetorForca,true);

		//----------------------------------------------------
		//Para aplicar um impulso linear (quantidade de movimento gerada pela aplica��o de uma for�a)...
		//caixa->ApplyLinearImpulse(vetorForca,caixa->GetWorldCenter());

		//----------------------------------------------------
		//Para aplicar for�as em outro ponto do objeto...
		//b2Vec2 pontoLocal, pontoGlobal;
		////Pega a posicao do ponto no mundo, dado um ponto fornecido em coordenadas locais (0,0) � o centro
		//pontoLocal.x = -3.0;
		//pontoLocal.y = 4.0;
		//pontoGlobal = caixa->GetWorldPoint(pontoLocal);
		//caixa->ApplyForce(vetorForca, pontoGlobal, true);
		

		//Para aplicar um impulso linear...
		//caixa->ApplyLinearImpulse(vetorForca,pontoGlobal);

		//b2Vec2 vetorVelocidade;
		//vetorVelocidade = CalculaComponentesDoVetor(30,45);
		//caixa->SetLinearVelocity(vetorVelocidade);
		
	}
}

//Rotina para a cria��o de um objeto caixa parametrizado
b2Body *criaCaixa(float posX, float posY, float altura, float largura, float massa, float coefatrito, float coefrestituicao)
{
	//Cria o novo objeto (objeto novoObjeto)
	b2Body *objeto;

	//1� passo: cria��o da defini��o do corpo (b2BodyDef)
	b2BodyDef b;
	b.position.Set(posX, posY);
	b.type = b2_dynamicBody;
	//2� passo: cria��o do corpo pelo mundo (mundo cria corpo)
	objeto = world->CreateBody(&b);
	//3� passo: cria��o da defini��o da forma (b2PolygonShape, b2CircleShape ou b2EdgeShape)
	b2PolygonShape caixa;
	caixa.SetAsBox(largura / 2, altura / 2);
	//4� passo: cria��o da defini��o da fixture (b2FixtureDef)
	//N�o esquecer de associar a forma com a fixture!
	b2FixtureDef f;
	f.shape = &caixa;
	f.density = massa / (altura*largura);
	f.friction = coefatrito;
	f.restitution = coefrestituicao;
	//5� passo: cria��o da fixture pelo corpo (objeto cria fixture)
	objeto->CreateFixture(&f);

	return objeto;
}


//Fun��o de inicializa��o da Box2D
void InitBox2D()
{

	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -9.8f);

	// Inicializa a biblioteca Box2D
	world = new b2World(gravity);

	// Define os par�metro para a simula��o
	// Quanto maior, mais preciso, por�m, mais lento
	velocityIterations = 6;
	positionIterations = 2;
	timeStep = 1.0f / 60.0f; //60 Hz

							 //Cria o ch�o
	b2BodyDef bd;
	chao = world->CreateBody(&bd);
	//Forma do ch�o: edge
	b2EdgeShape shape;
	shape.Set(b2Vec2(-39.5, -39.5), b2Vec2(39.5, -39.5));
	chao->CreateFixture(&shape, 0.0);

	//Cria o teto
	teto = world->CreateBody(&bd);
	shape.Set(b2Vec2(-39.5, 39.5), b2Vec2(39.5, 39.5));
	teto->CreateFixture(&shape, 0.0);

	//Cria a parede esquerda
	paredeEsquerda = world->CreateBody(&bd);
	shape.Set(b2Vec2(-39.5, 39.5), b2Vec2(-39.5, -39.5));
	paredeEsquerda->CreateFixture(&shape, 0.0);

	//Cria a parede direita
	paredeDireita = world->CreateBody(&bd);
	shape.Set(b2Vec2(39.5, 39.5), b2Vec2(39.5, -39.5));
	paredeDireita->CreateFixture(&shape, 0.0);


	//Cria uma caixa na posicao (0,0), com 8m de altura, 6m de largura, 3kg, coeficiente de atrito e restitui��o 0.5
	caixa = criaCaixa(0.0, 0.0, 8.0, 6.0, 3.0, 0.5, 0.5);

}


// Fun��o de Execu��o da Simula��o
void RunBox2D()
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();
}

// Fun��o que imprime todos os objetos  
void PrintBodies()
{
	b2Body *b;
	float ang;
	b2Vec2 pos;
	//PERCORRE A LISTA DE CORPOS R�GIDOS DO MUNDO
	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		pos = b->GetPosition();
		ang = b->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", pos.x, pos.y, ang);
	}
}

//Fun��o que chama as rotinas de desenho para cada fixture de um corpo r�gido, de acordo com o tipo da forma de colis�o dela e 
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

//Para chamar a rotina de desenho das fixtures de um corpo
void DrawBody(b2Body *b, b2Color color)
{
	//desenha s� a primeira fixture. Pra desenhar todas, tem que percorrer a lista
	DrawFixture(b->GetFixtureList(), color);
}




//Rotina de render (n�o � mais callback)
void Render()
{
	b2Body *b;
	glColor3f(1, 0, 0);
	glPointSize(5);

	b2Color color; color.r = 1.0; color.g = 0.0; color.b = 0.0;

	//PERCORRE A LISTA DE CORPOS R�GIDOS DO MUNDO E CHAMA A ROTINA DE DESENHO PARA A LISTA DE FIXTURES DE CADA UM
	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		DrawBody(b, color);
	}
}

// Programa Principal 
int main(void)
{
	srand(time(0));

	//Inicializa��o da janela da aplica��o
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
	glfwSetKeyCallback(window, key_callback);


	InitBox2D();


	while (!glfwWindowShouldClose(window)) //loop da aplica��o :)
	{
		float ratio;
		int width, height;

		//aqui recupera o tamanho atual da janela, para corre��o do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		//setando a viewport da OpenGL, para ocupar toda a janela da aplica��o
		glViewport(0, 0, width, height);

		// Limpa a janela de visualiza��o com a cor branca
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Setando a matriz de proje��o, para definir o Ortho (c�mera ortogr�fica 2D)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-40.0f*ratio, 40.0f*ratio, -40.0f, 40.0f);
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

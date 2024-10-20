/*
Semestre 2024-1
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transforomación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- ,Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float cartelPosY = 0.0f; // Posición vertical del cartel
float cartelRotation = 0.05f; // Rotación del cartel
float cartelSpeed = 0.05f; // Velocidad del movimiento

float dadoSpeed = 3.0f;
float barcoSpeed = 1.0f; // Velocidad del barco
float limit = 125.0f; // Límite del movimiento

float barcoPosX = 125.0f;
float barcoPosZ = 125.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture MonopolyTexture;
Texture dado10;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model arco;
Model cartel;
Model rueda;
Model barco;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,    1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,    0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,     1.0f, 1.0f,     0.0f, -1.0f, 0.0f
	};


	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}

void crearDado10() {
	unsigned int dado10_indices[] = {
		// ARRIBA, Cara 1
		0, 1, 2,
		2, 3, 0,
		//ARRIBA, cara 2
		4, 5, 6,
		6, 7, 4,
		//ARRIBA, cara 3
		8, 9, 10,
		10, 11, 8,
		//ARRIBA, cara 4
		12, 13, 14,
		14, 15, 12,
		//ARRIBA, cara 5
		16, 17, 18,
		18, 19, 16,
		//ABAJO, cara 6 
		20, 21, 22,
		22, 23, 20,
		//ABAJO, cara 7
		24, 25, 26,
		26, 27, 24,
		//ABAJO, cara 8
		28, 29, 30,
		30, 31, 28,
		//ABAJO, cara9
		32, 33, 34,
		34, 35, 32,
		//ABAJO, cara 10
		36, 37, 38,
		38, 39, 36
	};
	GLfloat dado10_vertices[] = {
		// CARA 1
		//x		y		z		S		    T			NX		NY		NZ
		-0.5f, +0.0f,  0.5f,	0.0566f,    0.2969f,	0.0f,	0.0f,	-1.0f,	//0
		0.0f, -0.25f,   0.7f,	0.0215f,	0.1856f,	0.0f,	0.0f,	-1.0f,	//1
		0.5f, +0.0f,   0.5f,	0.1504f,	0.0996f,	0.0f,	0.0f,	-1.0f,	//2
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	0.0f,	0.0f,	-1.0f,	//3
		// CARA 2
		//x		y		z		S		    T
		0.5f, +0.0f,   0.5f,    0.3145f,    0.8906f,	-1.0f,	0.0f,	0.0f,
		1.f,  -0.25f,  0.1f,	0.1875f,    0.8047f,	-1.0f,	0.0f,	0.0f,
		0.9f, +0.0f,   -0.5f,	0.2266f,	0.6895f,	-1.0f,	0.0f,	0.0f,
		0.0f,  0.75f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	0.0f,	0.0f,

		//CARA 3
		-0.5f, +0.0f,  0.5f,	0.4629f,	0.0234f,	0.0f,	0.0f,	1.0f,
		-1.f,  -0.25f, 0.1f,    0.2793f,	0.0117f,	0.0f,	0.0f,	1.0f,
		-0.9f, +0.0f,  -0.5f,	0.1504f,	0.0996f,	0.0f,	0.0f,	1.0f,
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	0.0f,	0.0f,	1.0f,

		// CARA 4
		//x		y		z		S			T
		0.9f, +0.0f,   -0.5f,	0.9141f,	0.8535f,	1.0f,	0.0f,	0.0f,
		0.6f, -0.35f,  -1.15f,	0.8184f,	0.9551f,	1.0f,	0.0f,	0.0f,
		0.0f,  0.0f,  -1.25f,	0.6328f,	0.9648f,	1.0f,	0.0f,	0.0f,
		0.0f,  0.75f,  -0.25f,	0.5957f,	0.7324f,	1.0f,	0.0f,	0.0f,

		// CARA 5
		//x		y		z		S			T
		-0.9f, +0.0f,   -0.5f,	0.2598f,	0.4648f,	0.0f,	1.0f,	0.0f,
		-0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	0.0f,	1.0f,	0.0f,
		0.0f,  0.0f,  -1.25f,	0.5957f,	0.4668f,	0.0f,	1.0f,	0.0f,
		0.0f,  0.75f,  -0.25f,	0.4277f,	0.2539f,	0.0f,	1.0f,	0.0f,

		//CARA 6
		//x		y		z		S			T
		0.0f, -0.25f,   0.7f,   0.3164f,	0.8906f,	-1.0f,	0.0f,	0.0f,
		0.5f, +0.0f,    0.5f,	0.4473f,	0.9785f,	-1.0f,	0.0f,	0.0f,
		1.f,  -0.25f,   0.1f,	0.6309f,	0.9629f,	-1.0f,	0.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	0.0f,	0.0f,

		//CARA 7
		0.0f, -0.25f,   0.7f,   0.2598f,	0.4648f,	-1.0f,	0.0f,	0.0f,
		-0.5f, +0.0f,   0.5f,	0.0918f,    0.4141f,	-1.0f,	0.0f,	0.0f,
		-1.f,  -0.25f,   0.1f,	0.0625f,	0.3027f,	-1.0f,	0.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.4277f,	0.2539f,	-1.0f,	0.0f,	0.0f,

		//CARA 8
		1.f,  -0.25f,  0.1f,    0.2266f,	0.6895f,	-1.0f,	0.0f,	0.0f,
		0.9f, +0.0f,   -0.5f,	0.2617f,	0.5762f,	-1.0f,	0.0f,	0.0f,
		0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	-1.0f,	0.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	0.0f,	0.0f,

		//CARA 9
		-1.f,  -0.25f,  0.1f,   0.4668f,	0.0273f,	-1.0f,	0.0f,	0.0f,
		-0.9f, +0.0f,   -0.5f,	0.6523f,	0.0371f,	-1.0f,	0.0f,	0.0f,
		-0.6f, -0.35f,  -1.15f,	0.75f,		0.1328f,	-1.0f,	0.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.4277f,	0.2539f,	-1.0f,	0.0f,	0.0f,

		//CARA 10
		0.6f, -0.35f,  -1.15f,  0.7598f,	0.5234f,	-1.0f,	0.0f,	0.0f,
		0.0f,  0.0f,  -1.25f,	0.5957f,	0.4668f,	-1.0f,	0.0f,	0.0f,
		-0.6f, -0.35f,  -1.15f,	0.4297f,	0.5195f,	-1.0f,	0.0f,	0.0f,
		0.0f,  -1.25f,  -0.25f,	0.5957f,	0.7324f,	-1.0f,	0.0f,	0.0f,

	};

	Mesh* caras10 = new Mesh();
	caras10->CreateMesh(dado10_vertices, dado10_indices, 320, 60);
	meshList.push_back(caras10);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	crearDado10();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	MonopolyTexture = Texture("Textures/monopoly.tga");
	MonopolyTexture.LoadTextureA();
	dado10 = Texture("Textures/dado10.tga");
	dado10.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	barco = Model();
	barco.LoadModel("Models/12219_boat_v2_L2.obj");
	rueda = Model();
	rueda.LoadModel("Models/rueda.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movCoche = 0.0f;
	movOffset = 0.03f;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	float movdadoY = 15.0f;
	int state = 0;
	int stateDado = 4;
	int rotacion = 0;
	float velocidadRotacion = glm::radians(30.0f);
	float rota = 0;
	float rotaX = 0;
	float rotaY = 0;
	int tiempoDado = 0;
	float rotaZ = 0;
	int estado = 0; // Estado inicial
	float duracion = 5.0f; // Duración en segundos
	float tiempoActual = 0.0f;
	int aleatorio = 0;
	int aleatorioZ = 0;
	int aleatorioY = 0;
	bool rotaDado = true;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		rota += 3 * deltaTime;

		switch (state) {
		case 0: // Mover a la derecha
			barcoPosX += barcoSpeed * deltaTime;
			if (barcoPosX >= limit) {
				barcoPosX = limit; // Ajusta la posición al límite
				rotacion = -90;
				state = 1; // Cambia al siguiente estado
			}
			break;
		case 1: // Mover hacia arriba
			barcoPosZ -= barcoSpeed * deltaTime;
			if (barcoPosZ <= -limit) {
				barcoPosZ = -limit;
				rotacion = 0;
				state = 2; // Cambia al siguiente estado
			}
			break;
		case 2: // Mover a la izquierda
			barcoPosX -= barcoSpeed * deltaTime;
			if (barcoPosX <= -limit) {
				barcoPosX = -limit;
				rotacion = 90;
				state = 3; // Cambia al siguiente estado
			}
			break;
		case 3: // Mover hacia abajo
			barcoPosZ += barcoSpeed * deltaTime;
			if (barcoPosZ >= limit) {
				barcoPosZ = limit;
				rotacion = 180;
				state = 0; // Regresa al estado inicial
			}
			break;
		default:
			break;
		}


		switch (stateDado) {
			case 0:
				movdadoY = 20.0f; // Asigna el valor
				stateDado = 1; // Cambia al siguiente estado
				printf("Estado 0");
				break;

			case 1:
				movdadoY -= dadoSpeed * deltaTime;
				if (movdadoY <= 2) {
					movdadoY = 2; // Ajusta la posición al límite
					stateDado = 2; // Cambia al siguiente estado
				}
				break;

			case 2:
				rotaX = static_cast<float>(rand()) / RAND_MAX * 360.0f;
				rotaY = static_cast<float>(rand()) / RAND_MAX * 360.0f;
				rotaZ = static_cast<float>(rand()) / RAND_MAX * 360.0f;
				tiempoDado += dadoSpeed * deltaTime;
				if (tiempoDado >= 2) {
					stateDado = 3;
				}
				break;
			case 3:
				printf("Estado 3");
				rotaX = 0.0f; // Resetea la rotación
				rotaY = 0.0f;
				rotaZ = 0.0f;
				tiempoDado = 0;
				aleatorio = rand() % 10 + 1;
				stateDado = 5;
				// Puede agregar lógica adicional aquí si es necesario
				break;
			case 4: 
				stateDado = static_cast<int>(mainWindow.getStateDado());
				break;
			default:
				break;
		}
		if (rotaDado == true) {
			if (aleatorio == 1) {
				aleatorioZ = 50;
				aleatorioY = 80;
				rotaDado = false;
			};
			if (aleatorio == 2) {
				aleatorioZ = 50;
				aleatorioY = 10;
				rotaDado = false;
			};
			if (aleatorio == 3) {
				aleatorioZ = 50;
				aleatorioY = 150;
				rotaDado = false;
			};
			if (aleatorio == 4) {
				aleatorioZ = 50;
				aleatorioY = 290;
				rotaDado = false;
			};
			if (aleatorio == 5) {
				aleatorioZ = 50;
				aleatorioY = 220;
				rotaDado = false;
			};
			if (aleatorio == 6) {
				aleatorioZ = 110;
				aleatorioY = 45;
				rotaDado = false;
			};
			if (aleatorio == 7) {
				aleatorioZ = 110;
				aleatorioY = 120;
				rotaDado = false;
			};
			if (aleatorio == 8) {
				aleatorioZ = 110;
				aleatorioY = 0;
				rotaDado = false;
			};
			if (aleatorio == 9) {
				aleatorioZ = 110;
				aleatorioY = 190;
				rotaDado = false;
			};
			if (aleatorio == 10) {
				aleatorioZ = 110;
				aleatorioY = 260;
				rotaDado = false;
			};
		}




		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		MonopolyTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		// Barco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(barcoPosX, 0.0f, barcoPosZ));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		
		//model = glm::rotate(model, rotacion * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		barco.RenderModel();

		// Rueda del lado positivo
		model = modelaux; // Restaurar la matriz de modelo del barco
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, { 15.0f, -4.0f, 8.0f }); // Mover la rueda a la posición correcta
		model = glm::rotate(model, rota * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar alrededor del eje Z
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda.RenderModel();

		// Rueda del lado negativo
		model = modelaux; // Restaurar la matriz de modelo del barco
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, { 15.0f, -4.0f, -8.0f }); // Mover la rueda a la posición correcta
		model = glm::rotate(model, rota * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar alrededor del eje Z
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rueda.RenderModel();



		//Dado 10 caras
		glm::mat4 modelDado = glm::mat4(1.0); // Nueva matriz para el dado
		modelDado = glm::translate(modelDado, glm::vec3(-1.5f, movdadoY, -2.0f));
		modelDado = glm::rotate(modelDado, aleatorioZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelDado = glm::rotate(modelDado, aleatorioY * toRadians, glm::vec3(0.0f, 1.0f, .0f));
		modelDado = glm::rotate(modelDado, rotaX * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelDado = glm::rotate(modelDado, rotaY * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelDado = glm::rotate(modelDado, rotaZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelDado = glm::scale(modelDado, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelDado));
		dado10.UseTexture();
		meshList[4]->RenderMesh();


		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
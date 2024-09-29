/*
Práctica 6: Texturizado
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
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture dado10;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Carro;
Model Capo;
Model Cristal;
Model Llanta;
Model Rin;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
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
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.25f,	0.33f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,    0.25f,	0.66f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	0.66f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.5f,  0.33f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.75f,	0.33f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.75f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,    0.5f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.75f, 0.33f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,  0.33f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,   0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.75f,  0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.50f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.75f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.75f,	0.33f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.50f,	0.33f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.50f,  0.66f,      0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.75f,	0.66f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.75f,  1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.50f,  1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

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


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
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
	dadoTexture = Texture("Textures/dado_edit.tga");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	dado10 = Texture("Textures/dado10.tga");
	dado10.LoadTextureA();


	
	
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dado_M = Model();
	Dado_M.LoadModel("Models/dadoanimales.obj");

	//Modelo del Carro
	Carro = Model();
	Carro.LoadModel("Models/carro.obj");

	Capo = Model();
	Capo.LoadModel("Models/capo.obj");

	Llanta = Model();
	Llanta.LoadModel("Models/llanta.obj");

	Rin = Model();
	Rin.LoadModel("Models/rin.obj");

	Cristal = Model();
	Cristal.LoadModel("Models/cristal.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();
		/*Reporte de práctica :
		Ejercicio 1: Crear un dado de 10 caras y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y
		texturizar el logo de la Facultad de ingeniería en el cofre de su propio modelo de coche
		*/

		//Dado de 10 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado10.UseTexture();
		meshList[5]->RenderMesh();

		//Carro textrurizado
		model = glm::mat4(1.0);
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro.RenderModel();
		model = modelaux;

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { 1.8f, 0.4f, 0.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Capo.RenderModel();


		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cristal.RenderModel();

		//Llantas
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { 3.0f, -1.0f, -2.0f });
		model = glm::rotate(model, glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { 3.0f, -1.0f, -2.0f });
		model = glm::rotate(model, glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rin.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { 3.0f, -1.0f, 2.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { 3.0f, -1.0f, 2.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rin.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { -2.8f, -1.0f, -1.8f });
		model = glm::rotate(model, glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { -2.8f, -1.0f, -1.8f });
		model = glm::rotate(model, glm::radians(180.0f), { 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rin.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { -2.8f, -1.0f, 1.8f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta.RenderModel();

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = modelaux;
		model = glm::translate(model, { -2.8f, -1.0f, 1.8f });
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rin.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/
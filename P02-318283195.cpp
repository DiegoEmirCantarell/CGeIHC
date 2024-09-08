//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderRojo = "shaders/shader_rojo.vert";
static const char* vShaderAzul = "shaders/shader_azul.vert";
static const char* vShaderVerde = "shaders/shader_verde.vert";
static const char* vShaderCafe = "shaders/shader_cafe.vert";
static const char* vShaderVerdeObscuro = "shaders/shader_verde_obscuro.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letraE[] = {	
		//X		Y	 Z		 R	 G		B
		-8.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-8.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-8.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, 1.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -1.5f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-7.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -4.0f, 0.0f, 0.1f, 0.5f, 0.5f,
		-6.0f, -6.0f, 0.0f, 0.1f, 0.5f, 0.5f,
	};

	GLfloat vertices_letraC[] = {
		//X		Y	 Z		R	G		B
		6.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, 4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.9f, 0.8f, 0.3f,
		8.0f, -6.0f, 0.0f, 0.9f, 0.8f, 0.3f,
	};
	
	GLfloat vertices_letraL[] = {
		//X		Y	 Z		R	G		B
		6.0f, 6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, 6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		6.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, -4.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		7.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		8.0f, -4.0f, 0.0f, 0.0f, 0.8f, 0.3f,
		8.0f, -6.0f, 0.0f, 0.0f, 0.8f, 0.3f,
	};
	MeshColor *letra_e = new MeshColor();
	letra_e->CreateMeshColor(vertices_letraE,150);
	meshColorList.push_back(letra_e);
	
	MeshColor* letra_c = new MeshColor();
	letra_c->CreateMeshColor(vertices_letraC, 150);
	meshColorList.push_back(letra_c);

	MeshColor* letra_l = new MeshColor();
	letra_l->CreateMeshColor(vertices_letraL, 150);
	meshColorList.push_back(letra_l);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	auto ShaderRojo = new Shader();
	ShaderRojo->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderList.push_back(*ShaderRojo);

	auto ShaderAzul = new Shader();
	ShaderAzul->CreateFromFiles(vShaderAzul, fShaderColor);
	shaderList.push_back(*ShaderAzul);

	auto ShaderVerde = new Shader();
	ShaderVerde->CreateFromFiles(vShaderVerde, fShaderColor);
	shaderList.push_back(*ShaderVerde);

	auto ShaderVerdeO = new Shader();
	ShaderVerdeO->CreateFromFiles(vShaderVerdeObscuro, fShaderColor);
	shaderList.push_back(*ShaderVerdeO);

	auto ShaderCafe = new Shader();
	ShaderCafe->CreateFromFiles(vShaderCafe, fShaderColor);
	shaderList.push_back(*ShaderCafe);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.05f,0.05f,0.05f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//Letras
		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, 0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.3f, 0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.6f, 0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();


		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//

		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.15f, -5.0f));
		model = glm::scale(model, glm::vec3(0.65f, 0.85f, 0.65f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Tejado
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.52f, -4.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Puerta central
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.35f, 0.25f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Ventana izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.17f, 0.05f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Ventana derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.17f, 0.05f, -4.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Pino izquierdo
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, -0.55f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.7f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.8f, 0.45f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Pino Derecho
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, -0.55f, -5.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.8f, 0.45f));
		glUniformMatrix4fv((GLint)uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv((GLint)uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/
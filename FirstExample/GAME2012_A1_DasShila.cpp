//***************************************************************************
// GAME2012 - Assignment1.cpp by Shila Das - 101141958
//
// Assignment 1 submission.
//
// Description:
// draw square and transform to transformation: rotation 45 degree based on z axis, and scale to sqrt(1/2*1/2+1/2*1/2)=0.707
//
//*****************************************************************************

using namespace std;

#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <array>
#include <iostream>


#define FPS 1
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint Buffers[NumBuffers];

const GLuint NumVertices = 20;
//const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2];
std::array<glm::vec2, NumVertices> vertices1;

int no_of_square = 15;
GLuint vao, points_vbo, colours_vbo, modelID;

GLfloat points[] = {
	-0.9f,  0.9f,  0.0f,
	0.9f,  0.9f,  0.0f,
	0.9f, -0.9f,  0.0f,
	-0.9f, -0.9f,  0.0f
};


GLfloat colours[] = {
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f
};
//transformation: rotation 45 degree based on z axis, and scale to sqrt(1/2*1/2+1/2*1/2)=0.707
float scale = 0.707f, angle = 45.0f;
void timer(int);



void init(void)
{
	srand((unsigned)time(NULL));
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "transform.vert" },
		{ GL_FRAGMENT_SHADER, "square.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	modelID = glGetUniformLocation(program, "model");

	//Generating two buffers, one is used to store the coordinates of the vertices
	//The other one is not used. Just wanted to show that we can allocate as many as buffers, some of which might left unused.
	vao = 0;//1
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	points_vbo = 0;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	colours_vbo = 0;
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	timer(0);
	glClearColor(0.4, 0.4, 0.4, 1.0); 
}

//---------------------------------------------------------------------
//
// transformModel  (Create the transformation matrices)
//
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
}
void recolorSquare(void)
{
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;

	float colours[] = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW); // Why can we do this without binding again?
}
//---------------------------------------------------------------------
//
// display
//
//Calculation
/*
* a+b=x, x=the length of outer square
* tanA=a/b, A is the angle we give
* a=x-b, b=x-a
* tanA=(x-b)/b
* b=x/(1+tanA)
* Or another formula
* cosA=b/c
* c=b/cosA
* c=(x/(1+tanA))/cosA
* 
*/
void
display(void)
{
	float x = 19.5f,rot=0.0f;
	glClear(GL_COLOR_BUFFER_BIT);
	
	int i = 0;

	do
	{
		glBindVertexArray(i);
		//Transform each of the squares correctly (here only need to scale and rotate them).
		x = (x / (1+glm::tan(glm::radians(angle)))/glm::cos(glm::radians(angle)))  ;
		//transformObject(pow(scale, i), Z_AXIS, angle * i, glm::vec3(0.0f, 0.0f, 0.0f));
		transformObject(x, Z_AXIS, rot-=angle, glm::vec3(0.0f, 0.0f, 0.0f));
		//Apply a different color for each of the squares.
		recolorSquare();

		glDrawArrays(GL_LINE_LOOP, 0, 4);
		i++;
	} while (i < no_of_square+1);


	glFlush();
}
void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0); // 60 FPS or 16.67ms.
}
void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	
	std::cout << "How many number of squares to be rendered: ";
	std::cin >> no_of_square;

	std::cout << "Specify the angle of rotation that the squares are rotated by: ";
	std::cin >> angle;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutCreateWindow("DAS, Shila, 101141958");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	//glutIdleFunc(idle);

	glutMainLoop();
	
	

}

#include <iostream>
#include <time.h>
#include <cmath>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL\GL.h>

#include "cOcean.h"

#pragma warning(disable: 4305)
#define GLEW_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#define BITMAP_ID 0x4D42
#define pi acos(-1.0)

using namespace std;

const int WIDTH = 600, HEIGHT = 600;
const float stp = 8.0f;
const int boxnum = 4;

float dx = 0.0f, dz = 0.0f, dy = 0.0f;
float center[] = { 0.0f, 0.0f, 0.0f };
float eye[] = { 0.0f, 20.0f, 30.0f };
float deye[] = { 0.0f, -20.0f, -30.0f };
float eyelen = -30.0f;
float shipPos[] = { 0.0f, 0.0f, 0.0f };
float boxPos[20][3];
float movPos[20][3];

glm::vec3 eyev;
float alpha = 0.0f, beta = 0.0f, gamma = 0.0f,
pitch = 0.0f, yaw = 0.0f, roll = 0.0f,
x = 0.0f, y = 0.0f, z = 30.0f;

int times = 0;
bool bPersp = false;
bool bAnim = false;
bool bWire = false;
bool hasGao = false;
bool preBox[20];

int wHeight = 0;
int wWidth = 0;
float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0
clock_t tstart, tend;
float tdue;
float s_speed = 0.0;
float s_cord = 0.0;
float _yew = 0.0;

unsigned int ttem = 0;

cOcean ocean(64, 0.0001f, vector2(0.0f, 64.0f), 64, false);

glm::mat4 Projection = glm::perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.05f, 2000.0f);
glm::mat4 View = glm::mat4(1.0f);
glm::mat4 Model = glm::mat4(1.0f);
glm::vec3 light_position;
float getHeight(float x, float z);


unsigned int texture1[3];

void drawboom(int mm);
void stopPicking();

void idle()
{
	glutPostRedisplay();
}

float wmat_diffuse[] = { 0.0, 0.0, 0.0 };
float wmat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
float mat_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };

float degreeToRadians(float dgr)
{
	float ret = dgr*3.145926575 / 180.0;
	return ret;
}


void reeye()
{
	for (int i = 0; i < 3; i++)
		center[i] = eye[i] + deye[i];
}

void reebox()
{
	shipPos[0] = eye[0];
	shipPos[1] = getHeight(eye[0], eye[2] - 30.0) - 3.3;
	shipPos[2] = eye[2] - 30.0f;
	for (int i = 0; i <3; i++)
	{

		if (i % 2 == 0)
			movPos[i][0] += 0.5f;
		else
			movPos[i][0] -= 0.4f;
		if (i % 3 == 1)
			movPos[i][2] += 0.5f;
		else  if (i % 3 == 2)
			movPos[i][2] -= 0.4f;
		else
			movPos[i][2] -= 0.3f;
		if (movPos[i][0] > 500)	movPos[i][0] = -500.0f;
		if (movPos[i][0] < -500)	movPos[i][0] = 500.0f;
		if (movPos[i][2] > 500)	movPos[i][2] = -500.0f;
		if (movPos[i][2] < -500)	movPos[i][2] = 500.0f;
	}
	//eye[0], getHeight(eye[0], eye[2] - 30.0) - 3.3, eye[2] - 30.0f
	for (int i = 3; i < boxnum; i++)
	{
		float ddx = 0.0f, ddy = 0.0f, ddz = 0.0f;
		float d1 = 0.0f, d2 = 0.0f, d3 = 0.0f;
		d1 = eye[0] - (boxPos[i][0] + movPos[i][0]);
		d2 = getHeight(eye[0], eye[2] - 30.0) - 3.6 - (boxPos[i][1] + movPos[i][1]);
		d3 = eye[2] - 30.0f - (boxPos[i][2] + movPos[i][2]);
		if (d1 + d2 + d3 < 0.005f)
			continue;
		movPos[i][0] += 2.0f * (d1 / (d1 + d2 + d3));
		movPos[i][1] += 2.0f * (d2 / (d1 + d2 + d3));
		movPos[i][2] += 2.0f * (d3 / (d1 + d2 + d3));
		//cout << movPos[i][0] << " " << movPos[i][1] << " " << movPos[i][2] << endl;
	}

}

glm::mat4 getViewMatrix(glm::vec3 eye, float pitch, float yaw)
{
	// If the pitch and yaw angles are in degrees,
	// they need to be converted to radians. Here
	// I assume the values are already converted to radians.
	float cosPitch = cos(degreeToRadians(pitch));
	float sinPitch = sin(degreeToRadians(pitch));
	float cosYaw = cos(degreeToRadians(yaw));
	float sinYaw = sin(degreeToRadians(yaw));

	glm::vec3 xaxis(cosYaw, 0, -sinYaw);
	glm::vec3 yaxis(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
	glm::vec3 zaxis(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);
	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	float matrix[16] = {
		xaxis.x, yaxis.x, zaxis.x, 0, //column 1
		xaxis.y, yaxis.y, zaxis.y, 0, //column 2
		xaxis.z, yaxis.z, zaxis.z, 0, //column 3
		-glm::dot(xaxis, eye), -glm::dot(yaxis, eye),
		-glm::dot(zaxis, eye), 1  //column 4
	};
	return glm::make_mat4(matrix);
}

void key(unsigned char k, int x, int y)
{
	dx = View[3][0]; dz = View[3][2];// dy = View[3][1];
	bool ok = false;
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }
	case ' ': {s_speed = 0.0; s_cord = 0.0;  ok = true; yaw = 0.0;
		deye[0] = eyelen*sin(degreeToRadians(yaw));
		deye[2] = eyelen*cos(degreeToRadians(yaw));
		reeye(); break; }
	case 'o': {bWire = !bWire;  ok = true; break; }
	case 'a': {
		yaw = 0; pitch = 0;
		eyev.x -= stp;
		eye[0] -= stp;
		center[0] -= stp;
		ok = true;  break; }
	case 'd': {
		yaw = 0; pitch = 0;
		eyev.x += stp;
		eye[0] += stp;
		center[0] += stp;
		ok = true; break; }

	case 'w': {
		yaw = 0; pitch = 0;
		eyev.z -= stp;
		eye[2] -= stp;
		center[2] -= stp;
		ok = true; break; }
	case 's': {
		yaw = 0; pitch = 0;
		eyev.z += stp;
		eye[2] += stp;
		center[2] += stp;
		ok = true; break; }

	case'z':{
		pitch += 10;
		eye[1] += 1;
		center[1] += 1;
		break;
	}
	case'c':{
		pitch -= 10;
		eye[1] -= 1;
		center[1] -= 1;
		break;
	}

	case 'g': {hasGao = !hasGao; break; }

	case 'j':{yaw += 10.0;

		deye[0] = eyelen*sin(degreeToRadians(yaw));
		deye[2] = eyelen*cos(degreeToRadians(yaw));
		reeye();
		break; }
	case 'l':{yaw -= 10.0;
		deye[0] = eyelen*sin(degreeToRadians(yaw));
		deye[2] = eyelen*cos(degreeToRadians(yaw));
		reeye();
		break; }
	case 'r':{
		memset(preBox, 1, sizeof(preBox));
		for (int i = 0; i < boxnum; i++)
			movPos[i][0] = 0.0f, movPos[i][2] = 0.0f, movPos[i][1] = 0.0f;
		yaw = 0;
		eye[0] = 0.0f;
		eye[2] = 30.0f;
		deye[0] = eyelen*sin(degreeToRadians(yaw));
		deye[2] = eyelen*cos(degreeToRadians(yaw));
		eyev.x = eye[0]; eyev.z = eye[2];
		reeye();

		break;
	}
	case '1':{
		light_position.x -= 10.0f;
		break; }
	case '2':{
		light_position.x += 10.0f;
		break; }
	case '3':{
		light_position.y -= 100.0f;
		break; }
	case '4':{
		light_position.y += 100.0f;
		break; }
	case '5':{
		light_position.z -= 100.0f;
		break; }
	case '6':{
		light_position.z += 100.0f;
		break; }
	default:  {}
	}
	//cout << center[0] << " " << center[1] << " " << center[2] << endl;
	//	cout << eyev.x<< " " <<eyev.y<<" "<<eyev.z<< endl;
	//	cout << yaw << endl;
	//	cout << endl;

}
void circulate()
{
	if (abs(View[3][2]) > 930)
		View[3][2] = 32;
	if (abs(View[3][0]) > 600)
		View[3][0] = 32;
}


void draw_1()
{
	reebox();
	GLfloat low_shininess[] = { 100.0 };
	GLfloat no_shininess[] = { 0.0, 0.0, 0.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_specular[] = { 0.0, 0.0, 0.0 };
	//	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	//glTranslatef(center[0],getHeight(center[0],center[2])-3.3,center[2]);
	for (int i = 0; i < boxnum; i++)
	{
		if (preBox[i + 1])
		{
			//cout << i << " " << endl;;
			if ((i) % 3 == 0)	glColor3f(0.0, 1.0, 0.0);
			if ((i) % 3 == 1)	glColor3f(1.0, 0.0, 0.0);
			if ((i) % 3 == 2)	glColor3f(0.0, 0.0, 1.0);
			if (!preBox[i + 1])	glColor3f(0.0, 0.0, 0.0);

			glPushMatrix();
			glPushName(i + 1);
			glTranslatef(boxPos[i][0] + movPos[i][0],
				getHeight(boxPos[i][0] + movPos[i][0],
				boxPos[i][2] + movPos[i][2]) + movPos[i][1], boxPos[i][2] + movPos[i][2]);
			glutSolidCube(2.0);
			glPopName();
			glPopMatrix();
		}
	}
	glDisable(GL_COLOR_MATERIAL);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;
	if (!bPersp) {
		gluPerspective(45.0f, whRatio, 0.05f, 400.0f);
		//glFrustum(-6, 6, -6, 6, 0.05f,400.0f);
	}
	else {
		glOrtho(-6, 6, -6, 6, -500, 500);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

float getHeight(float x, float z)
{
	int nn = 0;
	float summ = 0.0f;
	float minn = 2.0f*ocean.length / ocean.N;
	while (x > 32 || x < -32)
	{
		if (x>32)	x -= 32;
		else  x += 32;
	}
	while (z > 32 || z < -32)
	{
		if (z>32)	z -= 32;
		else  z += 32;
	}
	for (int m_prime = 0; m_prime < ocean.Nplus1; m_prime++) {
		for (int n_prime = 0; n_prime < ocean.Nplus1; n_prime++) {
			int index = m_prime *ocean.Nplus1 + n_prime;
			float temx = ocean.vertices[index].ox; float dx = (temx - x);
			float temz = ocean.vertices[index].oz; float dz = (temz - z);
			float dis = sqrt(dx*dx + dz*dz);
			if (dis>minn)
				continue;
			nn++;
			summ += ocean.vertices[index].y;
		}
	}
	float ret = summ / nn;
	return ret;
}

void Draw_Triangle() // This function draws a triangle with RGB colors
{

	glPushName(0);
	GLfloat low_shininess[] = { 200.0 };
	GLfloat no_shininess[] = { 0.0, 0.0, 0.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_specular[] = { 0.0, 0.0, 0.0 };

	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);

	glPushMatrix();
	glTranslatef(eye[0], getHeight(eye[0], eye[2] - 30.0) - 3.6, eye[2] - 30.0f);
	glRotatef(90, 0, 1, 0);
	//		glTranslatef(0, -0.3f, 0.0f);
	//	glTranslatef(0, 0, 0.0f);
	glScalef(15.0f, 15.0f, 40.0f);

	glPopMatrix();
	glDisable(GL_TEXTURE);

	glPopName();

	glDisable(GL_BLEND);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	float whRatio = (GLfloat)WIDTH / (GLfloat)HEIGHT;
}

void draw_background()
{
}

void redraw()
{

	glClearColor(0.05f, 0.05f, 0.45f, 1.0f);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat blu[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { light_position.x, light_position.y, light_position.z };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	tend = clock();
	View = getViewMatrix(eyev, 0, yaw);
	ocean.render(tend / 160.0, light_position, Projection, View, Model, true, bWire);
	glUseProgram(0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blu);
	glEnable(GL_LIGHT0);

	center[1] = getHeight(center[0], center[2]) + 2.5f;
	reeye();
	gluLookAt(
		eye[0], eye[1], eye[2],
		center[0], 5.0f, center[2],
		0.0, 1.0, 0.0);

	Draw_Triangle();
	glDisable(GL_BLEND);
	draw_1();
	draw_background();
	glDisable(GL_LIGHTING);
	glutSwapBuffers();

}


int init()
{
	glGenTextures(4, texture1);

	tstart = clock();
	ocean.afterGenerate();
	reeye();
	//View = glm::mat4(1.0f);
	//View = glm::rotate(View, pitch, glm::vec3(-1.0f, 0.0f, 0.0f));
	//View = glm::rotate(View, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	//View = glm::translate(View, glm::vec3(x, -40, z));
	eyev.x = eye[0]; eyev.y = eye[1]; eyev.z = eye[2];
	View = glm::mat4(1.0f);
	View = getViewMatrix(eyev, pitch, yaw);
	light_position = glm::vec3(200.0f, 500.0f, -2000.0f);

	return 0;

}

int main(int argc, char *argv[])
{
	glewInit();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	int windowHandle = glutCreateWindow("Wave");
	if (init())
	{
		return 1;
	}
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;

}
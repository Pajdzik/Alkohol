// Alkohol.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Alkohol.h"

using namespace std;
using namespace glm;

void initGLEW(void);
void initGLUT(int*, char**);
void initOpenGL(float, int, int);
void initModels(void);
void setupShaders(void);

void cleanModels(void);
void cleanShaders(void);

void changeSize(int, int);
void displayFrame(void);
void drawObject(void);
void nextFrame(void);

void keyDown(unsigned char, int, int);
void mouseMove(int x, int y);



void displayFrame(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);			// wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	observer.set(shaderProgram);
	
	drawObject();
	
	glutSwapBuffers();	
}

void drawObject(void) {
	
	model->draw(shaderProgram);
}

void nextFrame(void) {
	// procedura uruchamiana okresowo. Robi animacjê.
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;

	lastTime = actTime;
	model->angle += speed * interval / 1000.0;
	if (model->angle > 360) model->angle = 0.0f;

	glutPostRedisplay();
}

void keyDown(unsigned char c, int x, int y) {
	observer.move(c);
}

void mouseMove(int x, int y) {
	const float step = 0.003;

	if (x < 100) {			// lewa strona ekranu
		observer.mouseStep.first = -step;
	} else if (x > 500) {	// prawa strona ekranu
		observer.mouseStep.first = step;
	} else {
		observer.mouseStep.first = 0.0f;
	}

	if (y < 100) {			// góra ekranu
		observer.mouseStep.second = step;
	} else if (y > 500) {	// dó³ ekrau
		observer.mouseStep.second = -step;
	} else {
		observer.mouseStep.second = 0.0f;
	}
}

void changeSize(int width, int height) {
	glViewport(0, 0, width, height);						// ustawienie wymiarow przestrzeni okna
	observer.setupProjection(observer.fieldOfView, width, height);	//dostosowanie macierzy rzutowania do nowego wymiaru okna
}

void setupShaders(void) {
	shaderProgram = new ShaderProgram("shaders/vertexShader.txt", NULL, "shaders/fragmentShader.txt");
	shaderProgram->use();
}

void cleanShaders(void) {
	delete shaderProgram;
}

void initGLEW(void) {
	GLenum result = glewInit();

	if (GLEW_OK != result) {
		fprintf(stderr,"%s\n",glewGetErrorString(result));
		exit(1);
	}
}

void initGLUT(int *argc, char** argv) {
	//Procedura inicjuj¹ca biblotekê glut
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowPosition(windowPositionX,windowPositionY);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("Wirtualna galeria alkoholi");
	
	glutReshapeFunc(changeSize);	// procedura obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame);	// procedura obs³uguj¹ca odœwierzanie okna
	glutIdleFunc(nextFrame);		// procedura wywo³uj¹ca najczêœciaj jak siê da (animacja)

	glutKeyboardFunc(keyDown);
	glutPassiveMotionFunc(mouseMove);
}

void initOpenGL(float angle, int width, int height) {
	observer.setupProjection(angle, width, height);
	setupShaders();
	glEnable(GL_DEPTH_TEST);
}

void initModels(void) {
	model = new Model(shaderProgram, "becherovka", vec3(0.0f, 0.0f, 0.0f));
}

void cleanModels(void) {
	delete model;
}

int main(int argc, char* argv[]) {
	initGLUT(&argc, argv);
	initGLEW();
	initOpenGL(observer.fieldOfView, windowWidth, windowHeight);
	initModels();
	
	glutMainLoop();

	cleanShaders();
	cleanModels();

	return 0;
}


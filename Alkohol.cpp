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
	models[0]->draw(shaderProgram);
	models[1]->draw(shaderProgram);
	
}

void nextFrame(void) {
	// procedura uruchamiana okresowo. Robi animacjê.
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime - lastTime;

	lastTime = actTime;
	models[0]->angle += speed * interval / 1000.0;

	glutPostRedisplay();
}

void keyDown(unsigned char c, int x, int y) {
	const float speed = 0.01f;

	switch (c) {
	case 'a':	// lewo
	case 'A':
		observer.strafe(-speed);
		break;

	case 'd':	// prawo
	case 'D':
		observer.strafe(speed);
		break;

	case 's':	// ty³
	case 'S':
		observer.move(-speed);
		break;

	case 'w':	// przód
	case 'W':
		observer.move(speed);
		break;
	}
}

void mouseMove(int x, int y) {
	const unsigned char strip = 180;

	if (x < strip) {			// lewa strona ekranu
		observer.mouseStep.first = (float) -(windowWidth - x) / 10000;
	} else if (x > (windowWidth - strip)) {	// prawa strona ekranu
		observer.mouseStep.first = (float) x / 10000;
	} else {
		observer.mouseStep.first = 0.0f;
	}

	if (y < strip) {			// góra ekranu
		observer.mouseStep.second = (float) (windowHeight - y) / 10000;
	} else if (y > (windowHeight - strip)) {	// dó³ ekrau
		observer.mouseStep.second = (float) -y / 10000;
	} else {
		observer.mouseStep.second = 0.0f;
	}
}

void mouseEntry(int state) {
	observer.mouseStep.first = 0.0f;
	observer.mouseStep.second = 0.0f;
}

void mouse(int button, int state, int x, int y) {
	if (button == 3) {
		if (observer.fieldOfView > 0)	observer.setupProjection(--observer.fieldOfView, windowWidth, windowHeight);
	} else if (button == 4) {
		if (observer.fieldOfView < 180) observer.setupProjection(++observer.fieldOfView, windowWidth, windowHeight);
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
	
	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Wirtualna galeria alkoholi");
	
	glutReshapeFunc(changeSize);	// procedura obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame);	// procedura obs³uguj¹ca odœwierzanie okna
	glutIdleFunc(nextFrame);		// procedura wywo³uj¹ca najczêœciaj jak siê da (animacja)

	glutKeyboardFunc(keyDown);
	glutPassiveMotionFunc(mouseMove);
	glutEntryFunc(mouseEntry);
	glutMouseFunc(mouse);
}

void initOpenGL(float angle, int width, int height) {
	observer.setupProjection(angle, width, height);
	setupShaders();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
}

void initModels(void) {
	models[0] = new Model(shaderProgram, "chivasregal", vec3(0.0f, 0.0f, 0.0f));
	models[1] = new Model(shaderProgram, "pernod", vec3(-10.0f, 0.0f, 0.0f));
}

void cleanModels(void) {
	for (int i = 0; i < 18; i++) {
		delete models[i];
	}
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


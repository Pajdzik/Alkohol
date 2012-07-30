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
void quit(void);

void changeSize(int, int);
void displayFrame(void);
void drawObject(void);
void nextFrame(void);

void keyDown(unsigned char, int, int);
void mouseMove(int x, int y);



void displayFrame(void) {
	glClearColor(0.9, 0.9, 0.9, 1.0);			// wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	observer.set(shaderProgram);
	
	drawObject();
	
	glutSwapBuffers();	
}

void drawObject(void) {
	for (int i = 0; i < bottleCount; i++) {
		bottles[i]->draw(shaderProgram);
	}

	table->draw(shaderProgram);
}

void nextFrame(void) {
	observer.move();

	glutPostRedisplay();
}

void pick(int sx, int sy) {

}

void keyDown(unsigned char key, int x, int y) {
	const float speed = 0.03f;

	switch (key) {
	case 'a':	// lewo
	case 'A':
		observer.strafeStep = -speed;
		break;

	case 'd':	// prawo
	case 'D':
		observer.strafeStep = +speed;
		break;

	case 's':	// ty³
	case 'S':
		observer.moveStep = -speed;
		break;

	case 'w':	// przód
	case 'W':
		observer.moveStep = speed;
		break;

	case 27:	// escape
		quit();
		exit(0);
		break;
	}
}

void keyUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':	// lewo
	case 'A':
		observer.strafeStep = 0.0f;
		break;

	case 'd':	// prawo
	case 'D':
		observer.strafeStep = 0.0f;
		break;

	case 's':	// ty³
	case 'S':
		observer.moveStep = 0.0f;
		break;

	case 'w':	// przód
	case 'W':
		observer.moveStep = 0.0f;
		break;
	}
}

void keySpecialUp(int key, int x, int y) {
	switch (key) {
	
	}
}

void mouseMove(int x, int y) {
	observer.mouse.first = x;
	observer.mouse.second = y;
}

void mouse(int button, int state, int x, int y) {
	if (button = GLUT_LEFT_BUTTON) {
		pick(x, y);
	} else if (button == 3) {		// rolka
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
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Wirtualna galeria alkoholi");
	
	glutReshapeFunc(changeSize);	// procedura obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame);	// procedura obs³uguj¹ca odœwierzanie okna
	glutIdleFunc(nextFrame);		// procedura wywo³uj¹ca najczêœciaj jak siê da (animacja)

	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(keySpecialUp);

	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouse);

	glutWarpPointer(windowWidth / 2, windowHeight / 2);
}

void initOpenGL(float angle, int width, int height) {
	observer.setupProjection(angle, width, height);
	setupShaders();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
}

void initModels(void) {
	printf("Loading models... \n\n");

	const float radius = 70.0f;
	const float alfa = 10.0f; // 360.0 / (bottleCount / 2 - 2);

	float angle = 0.0f;

	bottles[absolut]		= new Bottle(shaderProgram, new AlcInfo("absolut", "wódka", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));				angle += alfa;
	bottles[baileys]		= new Bottle(shaderProgram, new AlcInfo("baileys", "likier", 17), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[becherovka]		= new Bottle(shaderProgram, new AlcInfo("becherovka", "likier", 38), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[beefeater]		= new Bottle(shaderProgram, new AlcInfo("beefeater", "gin", 47), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));				angle += alfa;
	bottles[chivasregal]	= new Bottle(shaderProgram, new AlcInfo("chivasregal", "whisky", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));		angle += alfa;
	bottles[cointreau]		= new Bottle(shaderProgram, new AlcInfo("cointreau", "likier", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[curacao]		= new Bottle(shaderProgram, new AlcInfo("curacao", "likier", 30), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[havanaclub]		= new Bottle(shaderProgram, new AlcInfo("havanaclub", "rum", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[jackdaniels]	= new Bottle(shaderProgram, new AlcInfo("jackdaniels", "whiskey", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));		angle += alfa;
	bottles[jagermeister]	= new Bottle(shaderProgram, new AlcInfo("jagermeister", "likier", 35), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));		angle += alfa;
	bottles[jameson]		= new Bottle(shaderProgram, new AlcInfo("jameson", "whiskey", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[kahlua]			= new Bottle(shaderProgram, new AlcInfo("kahlua", "likier", 20), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));				angle += alfa;
	bottles[malibu]			= new Bottle(shaderProgram, new AlcInfo("malibu", "likier", 21), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));				angle += alfa;
	bottles[martell]		= new Bottle(shaderProgram, new AlcInfo("martell", "koniak", 40), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[molinari]		= new Bottle(shaderProgram, new AlcInfo("molinari", "likier", 42), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[olmeca]			= new Bottle(shaderProgram, new AlcInfo("olmeca", "tequila", 38), vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[pernod]			= new Bottle(shaderProgram, new AlcInfo("pernod", "likier", 40),	vec3(radius * cos(angle), 0.0f, radius * sin(angle)));			angle += alfa;
	bottles[ramazzotti]		= new Bottle(shaderProgram, new AlcInfo("ramazzotti", "likier", 30),	vec3(radius * cos(angle), 0.0f, radius * sin(angle)));

	table = new Model(shaderProgram, "untitled",  vec3(0, 0, 30));
}

void cleanModels(void) {
	for (int i = 0; i < 18; i++) {
		delete bottles[i];
	}
}

void quit(void) {
	cleanShaders();
	cleanModels();
}

int main(int argc, char* argv[]) {
	initGLUT(&argc, argv);
	initGLEW();
	initOpenGL(observer.fieldOfView, windowWidth, windowHeight);
	initModels();
	
	glutMainLoop();

	quit();

	return 0;
}


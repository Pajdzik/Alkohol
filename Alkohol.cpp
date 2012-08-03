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

void keyUp(unsigned char, int, int);
void keyDown(unsigned char, int, int);
void mouseMove(int x, int y);



void displayFrame(void) {
	glClearColor(0.2, 0.2, 0.2, 1.0);			// wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	observer.set(shaderProgram);
	
	drawObject();
	
	glutSwapBuffers();	
}

void drawObject(void) {
	barrel->draw(shaderProgram);
	chair->draw(shaderProgram);
	counter->draw(shaderProgram);
	room->draw(shaderProgram);
	ground->draw(shaderProgram);
	table->draw(shaderProgram);
	table2->draw(shaderProgram);
	desk->draw(shaderProgram);

	for (int i = 0; i < bottleCount; i++) {
		bottles[i]->draw(shaderProgram);
	}


	
}

void nextFrame(void) {
	observer.move();

	glutPostRedisplay();
}

void pick(int sx, int sy) {
	//GLdouble	modelMatrix[16],
	//			projMatrix[16];
	//GLint viewport[4];
	//GLdouble nearv[3], farv[3];

	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	//glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	//gluUnProject(sx, viewport[1] + viewport[3] - sy, 0, modelMatrix, projMatrix, viewport, &nearv[0], &nearv[1], &nearv[2]);
	//gluUnProject(sx, viewport[1] + viewport[3] - sy, 1, modelMatrix, projMatrix, viewport, &farv[0], &farv[1], &farv[2]);

	//printf("%f %f %f\t\t%f %f %f \n", nearv[0], nearv[1], nearv[2], farv[0], farv[1], farv[2]);

	//if(nearv[2] == farv[2])		return;

	//GLfloat t = (nearv[2] - 1) / (nearv[2] - farv[2]);

	//// so here are the desired (x, y) coordinates
	//GLfloat x = nearv[0] + (farv[0] - nearv[0]) * t,
	//		y = nearv[1] + (farv[1] - nearv[1]) * t;

	//printf("%f %f\n", x, y);


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

void mouseEntry(int state) {
	if (state == GLUT_LEFT) {
		observer.focused = false;
	} else {
		observer.focused = true;
	}
}

void mouseMove(int x, int y) {
	observer.mouse.first = x;
	observer.mouse.second = y;
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
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
		fprintf(stderr,"%s\n", glewGetErrorString(result));
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
	glutEntryFunc(mouseEntry);

	//glutWarpPointer(windowWidth / 2, windowHeight / 2);
}

void initOpenGL(float angle, int width, int height) {
	observer.setupProjection(angle, width, height);
	setupShaders();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);

	glEnable(GL_MULTISAMPLE);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
}

void initModels(void) {
	printf("Loading models... \n\n");

	bottles[absolut]		= new Bottle(shaderProgram, new AlcInfo("absolut", "wódka", 40),		vec3(-60, -80, -300));
	bottles[baileys]		= new Bottle(shaderProgram, new AlcInfo("baileys", "likier", 17),		vec3(-80, -81, -300));
	bottles[becherovka]		= new Bottle(shaderProgram, new AlcInfo("becherovka", "likier", 38),	vec3(-100, -84, -300));
	bottles[beefeater]		= new Bottle(shaderProgram, new AlcInfo("beefeater", "gin", 47),		vec3(-120, -80, -300));
	bottles[chivasregal]	= new Bottle(shaderProgram, new AlcInfo("chivasregal", "whisky", 40),	vec3(-140, -80, -300));
	bottles[cointreau]		= new Bottle(shaderProgram, new AlcInfo("cointreau", "likier", 40),		vec3(-160, -82, -300));
	bottles[curacao]		= new Bottle(shaderProgram, new AlcInfo("curacao", "likier", 30),		vec3(-180, -85, -300));
	bottles[havanaclub]		= new Bottle(shaderProgram, new AlcInfo("havanaclub", "rum", 40),		vec3(-200, -78, -300));
	bottles[jackdaniels]	= new Bottle(shaderProgram, new AlcInfo("jackdaniels", "whiskey", 40),	vec3(-220, -80, -300));
	bottles[jagermeister]	= new Bottle(shaderProgram, new AlcInfo("jagermeister", "likier", 35),	vec3(-240, -83, -300));
	bottles[jameson]		= new Bottle(shaderProgram, new AlcInfo("jameson", "whiskey", 40),		vec3(-260, -80, -300));
	bottles[kahlua]			= new Bottle(shaderProgram, new AlcInfo("kahlua", "likier", 20),		vec3(-280, -78, -300));
	bottles[malibu]			= new Bottle(shaderProgram, new AlcInfo("malibu", "likier", 21),		vec3(-300, -76, -300));
	bottles[martell]		= new Bottle(shaderProgram, new AlcInfo("martell", "koniak", 40),		vec3(-320, -79, -300));
	bottles[molinari]		= new Bottle(shaderProgram, new AlcInfo("molinari", "likier", 42),		vec3(-340, -73, -300));
	bottles[olmeca]			= new Bottle(shaderProgram, new AlcInfo("olmeca", "tequila", 38),		vec3(-360, -80, -300));
	bottles[pernod]			= new Bottle(shaderProgram, new AlcInfo("pernod", "likier", 40),		vec3(-380, -80, -300));
	bottles[ramazzotti]		= new Bottle(shaderProgram, new AlcInfo("ramazzotti", "likier", 30),	vec3(-400, -82, -300));

	barrel	= new Model(shaderProgram, "barrel", vec3(380, -140, 270));
	chair	= new Model(shaderProgram, "chair", vec3(-330, -120, 70));
	counter = new Model(shaderProgram, "counter", vec3(-420, -210, 200));
	table	= new Model(shaderProgram, "table2", vec3(290, -150, -220));
	table2	= new Model(shaderProgram, "table3", vec3(290, -150, -30));
	ground	= new Model(shaderProgram, "floor", vec3(0, -210, 0));
	desk	= new Model(shaderProgram, "table", vec3(-530, 0, -31));
	room	= new Model(shaderProgram, "room",  vec3(0, 0, 0));
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


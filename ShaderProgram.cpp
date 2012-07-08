#include "StdAfx.h"
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(char* vertexShaderFile, char* geometryShaderFile, char* fragmentShaderFile) {
	printf("Loading vertex shader.\n");
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFile);

	printf("Loading fragment shader.\n");
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	if (geometryShaderFile != NULL) {
		printf("Loading geometry shader.\n");
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryShaderFile);
	} else {
		geometryShader = NULL;
	}

	shaderProgram = glCreateProgram();

	// pod��czenie shader�w
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (geometryShader != NULL)		glAttachShader(shaderProgram, geometryShader);

	glLinkProgram(shaderProgram);

	// pobranie log�w b��d�w linkowania i wy�wietlenie ich
	int infoLogLength = 0,
		charsWritten = 0;
	char *infoLog = 0;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1) {
		infoLog = new char[infoLogLength];
		glGetProgramInfoLog(shaderProgram, infoLogLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	printf("Shader program created!\n\n");
}


ShaderProgram::~ShaderProgram(void) {
	// od��czenie shader�w
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	if (geometryShader != NULL)		glDetachShader(shaderProgram, geometryShader);

	// usu� shadery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShader != NULL)		glDeleteShader(geometryShader);

	glDeleteProgram(shaderProgram);
}


char* ShaderProgram::readFile(char* fileName) {
	int fileSize = 0;
	char *result = 0;
	FILE *file = 0;

	file = fopen(fileName, "r");
	fseek(file, 0, SEEK_END);

	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	result = new char[fileSize + 1];
	fread(result, 1, fileSize, file);
	result[fileSize] = 0;
	
	fclose(file);

	return result;
}

GLuint ShaderProgram::loadShader(GLenum shaderType, char* fileName) {
	GLuint shader = glCreateShader(shaderType);			// wygeneruj uchwyt
	const GLchar* shaderSource = readFile(fileName);	// wczytanie pliku
	glShaderSource(shader, 1, &shaderSource, NULL);		// powi�zanie �r�d�a z uchwytem shadera
	glCompileShader(shader);							// kompilacja

	delete[] shaderSource;		// usuni�cie �r�d�a z pami�ci

	// pobranie logu b��d�w kompilacji i wy�wietlenie ich
	int infoLogLength = 0,
		charsWritten = 0;
	char *infoLog = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1) {
		infoLog =  new char[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, &charsWritten, infoLog);

		printf("%s", infoLog);
		delete []infoLog;
	}

	return shader;
}

void ShaderProgram::use() {
	// w��cz u�ywanie programu cieniuj�cego reprezentowanego przez aktualny obiekt
	glUseProgram(shaderProgram);
}

GLuint ShaderProgram::getUniformLocation(char* variableName) {
	//Pobierz numer slotu odpowiadaj�cego zmiennej jednorodnej o nazwie variableName
	return glGetUniformLocation(shaderProgram, variableName);
}

GLuint ShaderProgram::getAttribLocation(char* variableName) {
	//Pobierz numer slotu odpowiadaj�cego atrybutowi o nazwie variableName
	return glGetAttribLocation(shaderProgram, variableName);
}
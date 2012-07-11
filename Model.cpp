#include "StdAfx.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "lib/SOIL/SOIL.h"

using namespace std;
using namespace glm;

Model::Model(ShaderProgram *shaderProgram, string name, vec3 position) {
	bool b = false;

	string modelPath = "models/";
	modelPath.append(name);
	modelPath.append(".obj");

	printf("Loading %s\n",  name.c_str());

	printf("\t Model.....");
	b = loadOBJ(modelPath.c_str(), vertices, uvs, normals);

	if (b == true)	printf(" done!\n");
	else			printf(" ERROR!\n\n\n\n");
	

	string texturePath = "textures/tga/";
	texturePath.append(name);
	//texturePath.append(".jpg");
	texturePath.append(".tga");

	printf("\t Texture...");
	b = loadTexture("textures/tga/dupa.png");

	if (b == true)	printf(" done!\n\n");
	else			printf(" ERROR!\n\n\n\n");


	setupVBO();
	setupVAO(shaderProgram);

	angle = 10.0f;

	this->position = position;
}

Model::~Model(void) {
	// VBO
	glDeleteBuffers(1, &verticesBuffer);
	glDeleteBuffers(1, &uvsBuffer);
	glDeleteBuffers(1, &normalsBuffer);

	glDeleteVertexArrays(1, &vao);		// VAO
}

void Model::draw(ShaderProgram *shaderProgram) {
	modelMatrix = translate(mat4(1.0f), position); 
	modelMatrix = rotate(modelMatrix, angle, vec3(0.5f, 1.0f, 0.0f)); 
	

	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, value_ptr(modelMatrix));
	glUniform1i(shaderProgram->getUniformLocation("textureMap"), 0);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
}

void Model::setupVAO(ShaderProgram *shaderProgram) {
	//Procedura tworz¹ca VAO - "obiekt" OpenGL wi¹¿¹cy numery slotów atrybutów z buforami VBO

	//Pobierz numery slotów poszczególnych atrybutów
	GLuint vertexLocation	= shaderProgram->getAttribLocation("vertex");		// "vertex" odnosi siê do deklaracji "in vec4 vertex;" w vertex shaderze
	GLuint normalLocation	= shaderProgram->getAttribLocation("normal");		// "normal" odnosi siê do deklaracji "in vec4 normal;" w vertex shaderze
	GLuint textureLocation	= shaderProgram->getAttribLocation("texture");

	//Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	glGenVertexArrays(1,&vao);

	//Uaktywnij nowo utworzony VAO
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(vertexLocation); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej vertexLocation (atrybut "vertex")
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu vertexLocation maj¹ byæ brane z aktywnego VBO
	
	glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(textureLocation); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej colorLocation (atrybut "color")
	glVertexAttribPointer(textureLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu colorLocation maj¹ byæ brane z aktywnego VBO

	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(normalLocation); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej normalLocation (atrybut "normal")
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu normalLocation maj¹ byæ brane z aktywnego VBO

	glBindVertexArray(0);
}

void Model::setupVBO(void) {
	//Procedura tworz¹ca bufory VBO zawieraj¹ce dane z tablic opisuj¹cych rysowany obiekt.

	//Wspó³rzêdne wierzcho³ków
	glGenBuffers(1, &verticesBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê wierzcho³ków
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW); //wgraj tablicê wierzcho³ków do VBO
	
	//Wektory normalne wierzcho³ków
	glGenBuffers(1, &normalsBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê wektorów normalnych
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW); //wgraj tablicê wektorów normalnych do VBO
	
	//Kolory wierzcho³ków
	glGenBuffers(1, &uvsBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê kolorów
	glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW); //wgraj tablicê kolorów do VBO
}

bool Model::loadOBJ(const char *path, vector<vec3> &out_vertices, vector<vec2> &out_uvs, vector<vec3> &out_normals)
{
	//printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		fprintf(stderr, "Impossible to open the file ! Are you in the right path?\n");
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				fprintf(stderr, "File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}

	return true;
}

bool Model::loadTexture(const char *path) {
	GLuint textureID = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (textureID == NULL) {
		return false;
	} else {
		glBindTexture( GL_TEXTURE_2D, textureID );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glBindTexture( GL_TEXTURE_2D, 0 );
		 
		texture = textureID;
		return true;
	}
}

#include "StdAfx.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "cube.h"

using namespace std;
using namespace glm;

Model::Model(ShaderProgram *shaderProgram, const char *modelPath, const char* texturePath) {
	bool b = false;

	printf("Loading %s\n", modelPath);

	printf("\t Model...");
	b = loadOBJ(modelPath, vertices, uvs, normals);

	if (b == true)	printf(" done!\n");
	else			printf(" ERROR!\n");
	
	setupVBO();
	setupVAO(shaderProgram);

	angle = 10.0f;
}

Model::~Model(void) {}

void Model::draw(ShaderProgram *shaderProgram) {
	modelMatrix = rotate(mat4(1.0f), angle, vec3(0.5f, 1.0f, 0.0f)); 
	//modelMatrix = translate(modelMatrix, vec3(0.0f, 2.0f, -13.0f)); 

	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, value_ptr(modelMatrix));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glBindVertexArray(0);
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

void Model::setupVAO(ShaderProgram *shaderProgram) {
	//Procedura tworz�ca VAO - "obiekt" OpenGL wi���cy numery slot�w atrybut�w z buforami VBO

	//Pobierz numery slot�w poszczeg�lnych atrybut�w
	GLuint locVertex = shaderProgram->getAttribLocation("vertex");		// "vertex" odnosi si� do deklaracji "in vec4 vertex;" w vertex shaderze
	GLuint locColor	 = shaderProgram->getAttribLocation("color");		// "color" odnosi si� do deklaracji "in vec4 color;" w vertex shaderze
	GLuint locNormal = shaderProgram->getAttribLocation("normal");		// "normal" odnosi si� do deklaracji "in vec4 normal;" w vertex shaderze

	//Wygeneruj uchwyt na VAO i zapisz go do zmiennej globalnej
	glGenVertexArrays(1,&vao);

	//Uaktywnij nowo utworzony VAO
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(locVertex); //W��cz u�ywanie atrybutu o numerze slotu zapisanym w zmiennej locVertex (atrybut "vertex")
	glVertexAttribPointer(locVertex, 3, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu locVertex maj� by� brane z aktywnego VBO
	
	glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(locColor); //W��cz u�ywanie atrybutu o numerze slotu zapisanym w zmiennej locColor (atrybut "color")
	glVertexAttribPointer(locColor, 2, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu locColor maj� by� brane z aktywnego VBO

	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glEnableVertexAttribArray(locNormal); //W��cz u�ywanie atrybutu o numerze slotu zapisanym w zmiennej locNormal (atrybut "normal")
	glVertexAttribPointer(locNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu locNormal maj� by� brane z aktywnego VBO

	glBindVertexArray(0);
}

void Model::setupVBO(void) {
	//Procedura tworz�ca bufory VBO zawieraj�ce dane z tablic opisuj�cych rysowany obiekt.
	//Wyb�r rysowanego modelu (poprzez zakomentowanie/odkomentowanie fragmentu kodu)
	/*vertices = cubeVertices;
	colors = cubeColors;
	normals = cubeNormals;
	vertexCount = cubeVertexCount;*/

	//Wsp�rz�dne wierzcho�k�w
	glGenBuffers(1, &verticesBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), kt�ry b�dzie zawiera� tablic� wierzcho�k�w
	glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW); //wgraj tablic� wierzcho�k�w do VBO
	
	//Kolory wierzcho�k�w
	glGenBuffers(1, &uvsBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), kt�ry b�dzie zawiera� tablic� kolor�w
	glBindBuffer(GL_ARRAY_BUFFER, uvsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW); //wgraj tablic� kolor�w do VBO
	
	//Wektory normalne wierzcho�k�w
	glGenBuffers(1, &normalsBuffer);//Wygeneruj uchwyt na Vertex Buffer Object (VBO), kt�ry b�dzie zawiera� tablic� wektor�w normalnych
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW); //wgraj tablic� wektor�w normalnych do VBO
	
}

bool Model::loadBMP(const char *imagepath){

	//printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
    FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("Image could not be opened.\n"); return 0;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){ 
		fprintf(stderr, "Not a correct BMP file.\n");
		return false;
	}
	// A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
		fprintf(stderr, "Not a correct BMP file.\n");
		return false;
	}
	// Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {fprintf(stderr, "Not a correct BMP file.\n");    return false;}
    if ( *(int*)&(header[0x1C])!=24 )         {fprintf(stderr, "Not a correct BMP file.\n");    return false;}

	// Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
    data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
    fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
    this->Texture = textureID;
    return true;
}
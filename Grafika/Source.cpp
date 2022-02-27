#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <cmath>
#define numVBO 4
#define numVAO 2
using namespace std;

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Beadando 1.";
GLFWwindow* window = nullptr;

unsigned int VAO[numVAO];
unsigned int VBO[numVBO];

unsigned int shaderLineProgram;
unsigned int shaderCircleProgram;

GLuint		renderingProgram;
GLuint		XoffsetLocation;
GLuint		YoffsetLocation;

float		x = 0.00f;
float		y = 0.00f;
float		increment = 0.01f;

bool		xDir = true;
bool		yDir = false;


float verticesLine[] = {
	-0.33,0.0,0.0,
	0.33,0.0,0.0
};

float LineColor[] = {
	0,0,1,0,
	0,0,1,0
};

std::vector<glm::vec3> verticesCircle;
std::vector<glm::vec3> CircleColor;

void DrawCircle()
{
	for (int i = 0; i <= 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		float x = 0.16 * cosf(theta);
		float y = 0.16 * sinf(theta);
		float z = 0.0f;
		if (i % 2 == 0) {
			verticesCircle.push_back(glm::vec3(x, y, z));
			CircleColor.push_back(glm::vec3(0.0, 1.0, 0.0));
		}
		else {
			verticesCircle.push_back(glm::vec3(0, 0, 0));
			CircleColor.push_back(glm::vec3(1.0, 0.0, 0.0));
		}
		
	}
}


std::string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

static unsigned int CompileShader(const string& sourceShader, unsigned int type) {
	unsigned int id = glCreateShader(type);
	const char* src = &sourceShader[0];
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	// Error check
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		glDeleteShader(id);
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const string& vertexShader, const string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vertex = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fragment = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}



/** Az alkalmazáshoz kapcsolódó elõkészítõ lépések, pl. a shader objektumok létrehozása. */
/** The first initialization steps of the program, e.g.: creating the shader objects. */
void init() {

	glGenBuffers(numVBO, VBO);
	glGenVertexArrays(numVAO, VAO);


	// Line

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STATIC_DRAW);
	glBindVertexArray(VAO[1]);
	

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LineColor), LineColor, GL_STATIC_DRAW);
	glBindVertexArray(VAO[1]);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Circle
	DrawCircle();


	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, verticesCircle.size() * sizeof(glm::vec3), verticesCircle.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(glm::vec3), CircleColor.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */


	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	string vertexShaderSourceLine = readShaderSource("vertexShaderLine.glsl");
	string fragmentShaderSourceLine = readShaderSource("fragmentShaderLine.glsl");

	string vertexShaderSourceCircle = readShaderSource("vertexShaderCircle.glsl");
	string fragmentShaderSourceCircle = readShaderSource("fragmentShaderCircle.glsl");

	shaderCircleProgram = CreateShader(vertexShaderSourceCircle,fragmentShaderSourceCircle);
	shaderLineProgram = CreateShader(vertexShaderSourceLine, fragmentShaderSourceLine);
	glUseProgram(shaderLineProgram);
	glUseProgram(shaderCircleProgram);
	
}

/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
/** Call display function which will draw into the GLFWwindow object. */
void display() {
	/** Töröljük le a színbuffert! */
	/** Let's clear the color buffer! */
	glClearColor(1, 1, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);



	// Line
	glBindVertexArray(VAO[1]);
	glLineWidth(3);
	glDrawArrays(GL_LINE_LOOP, 0, sizeof(verticesLine) / sizeof(verticesLine[0]));

	// Leválasztom
	glBindVertexArray(0);

	if (xDir) {
		x += increment;
		if (x > 0.84f) increment = -0.01f;
		if (x < -0.84f) increment = 0.01f;
		unsigned int offsetLoc = glGetUniformLocation(shaderCircleProgram, "offsetX");
		glProgramUniform1f(shaderCircleProgram, offsetLoc, x);
	}
	if (yDir) {
		y += increment;
		if (y > 0.84f) increment = -0.01f;
		if (y < -0.84f) increment = 0.01f;
		GLuint offsetLoc = glGetUniformLocation(shaderCircleProgram, "offsetY");
		glProgramUniform1f(shaderCircleProgram, offsetLoc, y);
	}


	// Circle
	glBindVertexArray(VAO[0]);
	//glDrawArrays(GL_LINE_LOOP, 0, verticesCircle.size());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCircle.size());
	glBindVertexArray(0);

}

/** Felesleges objektumok törlése. */
/** Clenup the unnecessary objects. */
void cleanUpScene() {

	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteProgram(shaderCircleProgram);

	glDeleteVertexArrays(1, &VAO[1]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteProgram(shaderLineProgram);

	/** Töröljük a GLFW ablakot. */
	/** Destroy the GLFW window. */
	glfwDestroyWindow(window);
	/** Leállítjuk a GLFW-t. */
	/** Stop the GLFW system. */
	glfwTerminate();
	/** Kilépés EXIT_SUCCESS kóddal. */
	/** Stop the software and exit with EXIT_SUCCESS code. */
	exit(EXIT_SUCCESS);
}

int main(void) {
	/** Próbáljuk meg inicializálni a GLFW-t! */
	/** Try to initialize GLFW! */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/** A használni kívánt OpenGL verzió: 4.3. */
	/** The needed OpenGL version: 4.3. */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed.

		/** Próbáljuk meg létrehozni az ablakunkat. */
		/** Let's try to create a window for drawing. */
		/** GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor * monitor, GLFWwindow * share) */
	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);

	/** Válasszuk ki az ablakunk OpenGL kontextusát, hogy használhassuk. */
	/** Select the OpenGL context (window) for drawing. */
	glfwMakeContextCurrent(window);

	/** Incializáljuk a GLEW-t, hogy elérhetõvé váljanak az OpenGL függvények, probléma esetén kilépés EXIT_FAILURE értékkel. */
	/** Initalize GLEW, so the OpenGL functions will be available, on problem exit with EXIT_FAILURE code. */
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	/** 0 = v-sync kikapcsolva, 1 = v-sync bekapcsolva, n = n db képkockányi idõt várakozunk */
	/** 0 = v-sync off, 1 = v-sync on, n = n pieces frame time waiting */
	glfwSwapInterval(1);
	/** Az alkalmazáshoz kapcsolódó elõkészítõ lépések, pl. a shader objektumok létrehozása. */
	/** The first initialization steps of the program, e.g.: creating the shader objects. */
	init();

	/** A megadott window struktúra "close flag" vizsgálata. */
	/** Checks the "close flag" of the specified window. */
	while (!glfwWindowShouldClose(window)) {
		/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
		/** Call display function which will draw into the GLFWwindow object. */
		display();
		/** Double buffered mûködés. */
		/** Double buffered working = swap the front and back buffer here. */
		glfwSwapBuffers(window);
		/** Események kezelése az ablakunkkal kapcsolatban, pl. gombnyomás. */
		/** Handle events related to our window, e.g.: pressing a key or moving the mouse. */
		glfwPollEvents();
	}

	/** Felesleges objektumok törlése. */
	/** Clenup the unnecessary objects. */
	cleanUpScene();

	/** Kilépés EXIT_SUCCESS kóddal. */
	/** Stop the software and exit with EXIT_SUCCESS code. */
	return EXIT_SUCCESS;
}
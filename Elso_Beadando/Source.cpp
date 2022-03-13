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

using namespace std;


static GLfloat line_vertices[] =
{
	-0.33f, 0.0f, 0.00f,
	 0.33f, 0.0f, 0.00f
};

std::vector<glm::vec3> verticesCircle;
std::vector<glm::vec3> CircleColor;
std::vector<glm::vec3> CircleColor2;


// Create the lines for the vertices and also the colors (with glm)
void CreateCircleLinesandColors()
{
	for (int i = 0; i <= 360; i++)
	{
		float theta = (i * (atan(1) * 4) / 180);
		float x = 0.16 * cosf(theta); // 0-1 300px 1/6 0,16 -> 50px
		float y = 0.16 * sinf(theta);
		if (i % 2 == 0) {
			verticesCircle.push_back(glm::vec3(x, y, 0));
			CircleColor2.push_back(glm::vec3(0.0, 1.0, 0.0));
			CircleColor.push_back(glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			verticesCircle.push_back(glm::vec3(0, 0, 0));
			CircleColor2.push_back(glm::vec3(1.0, 0.0, 0.0));
			CircleColor.push_back(glm::vec3(0.0, 1.0, 0.0));
		}

	}
}

#define		numVBOs			3
#define		numVAOs			2
GLuint		VBO[numVBOs]; // kör -> kell pont meg kell szín -> 2 VBO   / Vonal -> 1 pontok
GLuint		VAO[numVAOs]; // Hány objektumom van?

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Beadando1";

GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;
GLuint		LineProgram;
GLuint		CircleProgram;
GLuint		XoffsetLocation;
GLuint		YoffsetLocation;
GLdouble lastUpdate, updateFrequency = 0.01;


float		x = 0.00f;
float		y = 0.00f;
float		linex = 0;
float		liney = 0;
float		increment = 0.01f;
float		incrementx = 0.01f;
float		incrementy = 0.01f;

bool		xDir = true;
bool		yDir = false;
bool dvdmode = false;

bool checkOpenGLError() {
	bool	foundError = false;
	int		glErr = glGetError();


	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;

		foundError = true;
		glErr = glGetError();
	}


	return foundError;
}

void printShaderLog(GLuint shader) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;


	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);


		glGetShaderInfoLog(shader, length, &charsWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;


	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);


		glGetProgramInfoLog(prog, length, &charsWritten, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

string readShaderSource(const char* filePath) {

	ifstream	fileStream(filePath, ios::in);
	string		content;
	string		line;


	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}


	fileStream.close();


	return content;
}

GLuint createShaderProgramforLine() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;

	string		vertShaderStr = readShaderSource("vertexShader.glsl");
	string		fragShaderStr = readShaderSource("fragmentShader.glsl");

	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();


	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);


	glCompileShader(vShader);

	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}


	glCompileShader(fShader);

	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}


	GLuint		vfProgram = glCreateProgram();

	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);


	glLinkProgram(vfProgram);

	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}


	glDeleteShader(vShader);
	glDeleteShader(fShader);


	return vfProgram;
}

GLuint createShaderProgramforCircle() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;

	string		vertShaderStr = readShaderSource("vertexShaderCircle.glsl");
	string		fragShaderStr = readShaderSource("fragmentShaderCircle.glsl");


	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);


	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();


	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);


	glCompileShader(vShader);

	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}


	glCompileShader(fShader);

	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}


	GLuint		vfProgram = glCreateProgram();

	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);


	glLinkProgram(vfProgram);

	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}


	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return vfProgram;
}

void init(GLFWwindow* window) {

	LineProgram = createShaderProgramforLine();
	CircleProgram = createShaderProgramforCircle();


	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);




	// Circle
	CreateCircleLinesandColors();

	glBindVertexArray(VAO[0]); // kör objektum

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // kör pontok

	glBufferData(GL_ARRAY_BUFFER, verticesCircle.size() * sizeof(glm::vec3), verticesCircle.data(), GL_STATIC_DRAW); // Feltöltjüka  videókártya membe

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Hova,mennyit,micsodát,normalizált,pontosan milyen adat és mennyi,honnan kezdve


	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(glm::vec3), CircleColor.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Lecsatolom
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Line

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Lecsatolom
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(1, 1, 0.0, 1.0);
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_COLOR_BUFFER_BIT); // fontos lehet minden egyes alkalommal törölni!



	// Biztonsági lépés
	glUseProgram(0);
	glUseProgram(LineProgram);
	glBindVertexArray(VAO[1]);
	GLuint offsetLoc = glGetUniformLocation(LineProgram, "offsetY");
	glProgramUniform1f(LineProgram, offsetLoc, liney);
	GLuint offsetLoc2 = glGetUniformLocation(LineProgram, "offsetY");
	glProgramUniform1f(LineProgram, offsetLoc2, liney);
	glLineWidth(3); // vonal vastagság 3px
	glDrawArrays(GL_LINES, 0, sizeof(line_vertices) / sizeof(line_vertices[0]));
	glBindVertexArray(0);


	glUseProgram(0);
	glUseProgram(CircleProgram);
	glBindVertexArray(VAO[0]);
	GLuint offsetLoc3 = glGetUniformLocation(CircleProgram, "offsetX");
	glProgramUniform1f(CircleProgram, offsetLoc3, x);
	GLuint offsetLoc4 = glGetUniformLocation(CircleProgram, "offsetY");
	glProgramUniform1f(CircleProgram, offsetLoc4, y);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, verticesCircle.size());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCircle.size());

	if (dvdmode) {
		if (glfwGetTime() - lastUpdate >= updateFrequency)
		{
			y += incrementy;
			x += incrementx;
			if (x > 0.84f) incrementx = -0.01;
			if (x < -0.84f) incrementx = 0.01;
			if (y > 0.84f) incrementy = -0.01;
			if (y < -0.84f) incrementy = 0.01;
		}
	}
	else {
		if (xDir) {
			x += increment;
			if (x > 0.84f) {
				increment = -0.01f;
			}
			if (x < -0.84f) {
				increment = 0.01f;
			}

		}
		if (yDir) {
			y += increment;
			if (y > 0.84f) increment = -0.01f; // 0.84 fogja a körvonal elérni a szélét
			if (y < -0.84f) increment = 0.01f;

		}

	}
	if ((y >= -0.12 + liney && y <= 0.12 + liney) && (x >= -0.52 + linex && x <= 0.52 + linex)) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, CircleColor2.size() * sizeof(glm::vec3), CircleColor2.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

		glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(glm::vec3), CircleColor.data(), GL_STATIC_DRAW);


		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	cout << "X: ";
	cout << x << endl;
	cout << "Y: ";
	cout << y << endl;
	cout << "lineX: ";
	cout << linex << endl;
	cout << "lineY: ";
	cout << liney << endl;
	glBindVertexArray(0);
}



void cleanUpScene() {

	glDeleteVertexArrays(numVAOs, VAO);
	glDeleteBuffers(numVBOs, VBO);


	glDeleteProgram(LineProgram);
	glDeleteProgram(CircleProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

float RandomFloat(float left_side, float right_side) {

	return right_side + ((float)rand()) / (float)RAND_MAX * (left_side - right_side);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();


	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;


	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if(liney < 1.0f) liney += 0.1;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (liney > -1.0f) liney -= 0.1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		dvdmode = !dvdmode;
	}

}


int main(void) {

	if (!glfwInit())
		exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);


	glfwMakeContextCurrent(window);


	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetKeyCallback(window, keyCallback);



	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);


	glfwSwapInterval(1);


	glfwSetWindowSizeLimits(window, 600, 600, 600, 600);

	glfwSetWindowAspectRatio(window, 1, 1);


	init(window);


	while (!glfwWindowShouldClose(window)) {

		display(window, glfwGetTime());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}


	cleanUpScene();


	return EXIT_SUCCESS;
}
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


GLint isDragged = -1;

static vector<glm::vec3> myControlPoints =
{
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f, 0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
};

 vector<glm::vec3> drawPoints;

#define		numVBOs			2
#define		numVAOs			2
GLuint		VBO[numVBOs];
GLuint		VAO[numVAOs];
GLuint		bezierProgram;


GLint			window_width = 600;
GLint			window_height = 600;
char		window_title[] = "Beadando2";

GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;

int NCR(int n, int r) {
	if (r == 0) return 1;

	if (r > n / 2) return NCR(n, n - r);

	long res = 1;

	for (int k = 1; k <= r; ++k) {
		res *= n - k + 1;
		res /= k;
	}

	return res;
}

// Calculate the distance of two points
GLfloat dist2(glm::vec3 P1, glm::vec3 P2)
{
	GLfloat t1 = P1.x - P2.x;
	GLfloat t2 = P1.y - P2.y;

	return t1 * t1 + t2 * t2;
}

// Get the active points
GLint getActivePoint(vector<glm::vec3> p, GLint size, GLfloat sens, GLfloat x, GLfloat y)
{

	GLint i;
	GLfloat s = sens * sens;

	GLfloat xNorm = x / (window_width / 2) - 1.0f;
	GLfloat yNorm = y / (window_height / 2) - 1.0f;
	glm::vec3 P = glm::vec3(xNorm, yNorm, 0.0f);

	for (i = 0; i < size; i++) {
		if (dist2(p[i], P) < s) {
			return i;
		}
	}
	return -1;

}

GLdouble blending(GLint i, GLfloat t)
{
	return NCR(myControlPoints.size() - 1, i) * pow(t, i) * pow(1.0f - t, myControlPoints.size() - 1 - i);
}

void drawBezierCurve()
{
	drawPoints.clear();
	glm::vec3	nextPoint;
	GLfloat		t = 0.0f, B;
	GLfloat		increment = 1.0f / 100.0f; 

	while (t <= 1.0f) {
		nextPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < myControlPoints.size(); i++) {
			B = blending(i, t);
			nextPoint.x += B * myControlPoints.at(i).x;
			nextPoint.y += B * myControlPoints.at(i).y;
			nextPoint.z += B * myControlPoints.at(i).z;
		}

		drawPoints.push_back(nextPoint);
		t += increment;
	}
}

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

GLuint createShaderProgramforBezierCurve() {
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



void init(GLFWwindow* window) {

	bezierProgram = createShaderProgramforBezierCurve();

	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);

	drawBezierCurve();
	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, drawPoints.size() * sizeof(glm::vec3), drawPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0, 0, 0, 1.0);
}

void display(GLFWwindow* window, double currentTime) {

	glClear(GL_COLOR_BUFFER_BIT); // fontos lehet minden egyes alkalommal törölni!


	// Get the uniform value to determine the color
	GLuint color = glGetUniformLocation(bezierProgram, "colorchanger");
	


	// Bezier Curve
	glUseProgram(0);
	glUseProgram(bezierProgram);
	glBindVertexArray(VAO[0]);
	glLineWidth(3.5);
	glProgramUniform1f(bezierProgram, color, 0); // Green
	glDrawArrays(GL_LINE_STRIP, 0, drawPoints.size());
	glBindVertexArray(0);
	

	glBindVertexArray(VAO[1]);

	// Lines
	glLineWidth(1.5);
	glProgramUniform1f(bezierProgram, color, 1); // Purple
	glDrawArrays(GL_LINE_LOOP, 0, myControlPoints.size());

	// Control points
	glPointSize(15);
	glProgramUniform1f(bezierProgram, color, 2); // Red
	glDrawArrays(GL_POINTS, 0, myControlPoints.size());
	glBindVertexArray(0);
}



void cleanUpScene() {

	glDeleteVertexArrays(numVAOs, VAO);
	glDeleteBuffers(numVBOs, VBO);
	glDeleteProgram(bezierProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}

void updateBezier() {
	// Draw the points again
	drawBezierCurve();

	glBindVertexArray(VAO[0]);

	// Write to the VBOs the data that we want / write to the vbo the new bezier curve
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, drawPoints.size() * sizeof(glm::vec3), drawPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Also calculate the points again
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, myControlPoints.size() * sizeof(glm::vec3), myControlPoints.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}


void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	// If our mouse is already in dragging mode
	if (isDragged >= 0)
	{
		// Positions
		myControlPoints[isDragged].x = xPos / (window_width / 2) - 1.0f;
		myControlPoints[isDragged].y = (window_height - yPos) / (window_height / 2) - 1.0f;
	}
	updateBezier();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GLint i;
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		
		if ((i = getActivePoint(myControlPoints, myControlPoints.size(), 0.1f, x, window_height - y)) != -1)
		{
			isDragged = i;
		}
		else {
			myControlPoints.push_back(glm::vec3(x / (window_width / 2) - 1.0f, (window_height - y) / (window_height / 2) - 1.0f,0));
			updateBezier();
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if ((i = getActivePoint(myControlPoints, myControlPoints.size(), 0.1f, x, window_height - y)) != -1)
		{
			myControlPoints.erase(myControlPoints.begin() + i);
			updateBezier();
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		isDragged = -1;
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();


	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;

}


int main(void) {

	if (!glfwInit())
		exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);


	glfwMakeContextCurrent(window);


	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
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
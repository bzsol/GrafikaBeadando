#include <array>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

GLfloat cube1[] = {
      
	-0.5f, -0.5f, -0.5f,0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f, 0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,  0.0f,  0.0f, 1.0f, 

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,

	-0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f

};

GLfloat cube2[] = {

	-0.5f, -0.5f, 1.5f,0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, 1.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, 1.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, 1.5f,0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, 1.5f,0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, 1.5f,0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,2.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,2.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,2.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,2.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,2.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,2.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, 2.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, 1.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, 1.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, 1.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, 2.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, 2.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f, 2.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, 1.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, 1.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, 1.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, 2.5f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, 2.5f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, 1.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, 1.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, 2.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, 2.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, 2.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, 1.5f, 0.0f, -1.0f,  0.0f,

	-0.5f, 0.5f, 1.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, 1.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, 2.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, 2.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f, 2.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f, 1.5f, 0.0f,  1.0f,  0.0f

};

GLfloat cube3[] = {

	-0.5f, -0.5f, -2.5f,0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -2.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -2.5f,0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -2.5f,0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -2.5f,0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -2.5f,0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,-1.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,-1.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,-1.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,-1.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,-1.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,-1.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -1.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -2.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -2.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -2.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -1.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -1.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f, -1.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -2.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -2.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -2.5f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -1.5f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -1.5f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -1.5f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -1.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -1.5f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -2.5f, 0.0f, -1.0f,  0.0f,

	-0.5f, 0.5f, -2.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, -2.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, -1.5f, 0.0f,  1.0f,  0.0f,
	 0.5f, 0.5f, -1.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f, -1.5f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f,  -2.5f, 0.0f,  1.0f,  0.0f

};

#define		numVBOs			3
#define		numVAOs			3
GLuint		VBO[numVBOs];
GLuint		VAO[numVAOs];
GLuint		cubeProgram;
GLfloat		angle = 0.0;
GLfloat		radius = 8.0; // Radius!! 8 <= radius <= 10


glm::mat4 view;
glm::mat4 projection;
glm::vec3 cameraPos = glm::vec3(radius, 0.0f, 0.0f); // start pos
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); //  x = 0 y = 0 z = 0 position to the camera
glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f); // UP vector 0,0,1 value added.
glm::vec3 cameraMoving = glm::vec3(0.0, -1.0, 0.0);
glm::vec3 cameraAscending = glm::vec3(0.0, 0.0, 1.0);
GLdouble	currentTime, deltaTime, lastTime = 0.0f;
glm::vec3	lightPos;
GLuint		lightPosLoc;
GLuint		invTMatrixLoc;
glm::mat4	invTmatrix;

GLint			window_width = 600;
GLint			window_height = 600;
char			window_title[] = "Beadando3";

GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;


// Perspective 55 as the task said
void computeCameraMatrices()
{

	view = glm::lookAt(cameraPos, cameraTarget, up);
	projection = glm::perspective(glm::radians(55.0f), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 100.0f);
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

GLuint createShaderProgram() {
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

	cubeProgram = createShaderProgram();

	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube1), cube1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube2), cube2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[2]);


	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube3), cube3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glClearColor(0, 0, 0, 1.0);
	glEnable(GL_DEPTH_TEST);
}

void display(GLFWwindow* window, double currentTime) {
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(cubeProgram);


	// Cube 1
	glUseProgram(0);
	glUseProgram(cubeProgram);
	glBindVertexArray(VAO[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube1));
	glBindVertexArray(0);

	//Cube 2
	glUseProgram(0);
	glUseProgram(cubeProgram);
	glBindVertexArray(VAO[1]);
	glTranslatef(0.0f, 0.0f, 2.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube2));
	glBindVertexArray(0);

	// Cube 3
	glUseProgram(0);
	glUseProgram(cubeProgram);
	glBindVertexArray(VAO[2]);
	glTranslatef(0.0f, 0.0f, -2.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube3));
	glBindVertexArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	GLuint modelLoc = glGetUniformLocation(cubeProgram, "model");

	computeCameraMatrices();
	GLuint viewLoc = glGetUniformLocation(cubeProgram, "view");
	GLuint projectionLoc = glGetUniformLocation(cubeProgram, "projection");
	invTMatrixLoc = glGetUniformLocation(cubeProgram, "invTMatrix");
	lightPosLoc = glGetUniformLocation(cubeProgram, "lightPos");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	invTmatrix = glm::inverseTranspose(view * model);
	glUniformMatrix4fv(invTMatrixLoc, 1, GL_FALSE, glm::value_ptr(invTmatrix));

	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
}



void cleanUpScene() {

	glDeleteVertexArrays(numVAOs, VAO);
	glDeleteBuffers(numVBOs, VBO);
	glDeleteProgram(cubeProgram);

	glfwDestroyWindow(window);

	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, width, height);
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	GLfloat cameraSpeed = 10.0f * deltaTime;

	if ((keyboard[GLFW_KEY_W]) || (keyboard[GLFW_KEY_UP]))
	{
		cameraPos += cameraSpeed * cameraAscending;
	}

	if ((keyboard[GLFW_KEY_S]) || (keyboard[GLFW_KEY_DOWN]))
	{
		cameraPos -= cameraSpeed * cameraAscending;
	}

	if ((keyboard[GLFW_KEY_A]) || (keyboard[GLFW_KEY_LEFT]))
	{
		angle -= cameraSpeed;
		cameraPos.x = radius * cos(angle);
		cameraPos.y = radius * sin(angle);
	}

	if ((keyboard[GLFW_KEY_D]) || (keyboard[GLFW_KEY_RIGHT]))
	{
		angle += cameraSpeed;
		cameraPos.x = radius * cos(angle);
		cameraPos.y = radius * sin(angle);
	}

	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();


	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;

}


int main(void) {

	if (!glfwInit()) exit(EXIT_FAILURE);


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);


	glfwMakeContextCurrent(window);


	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetKeyCallback(window, keyCallback);


	if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);


	glfwSwapInterval(1);


	glfwSetWindowSizeLimits(window, 800, 800, 800, 800);

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
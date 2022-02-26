#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Beadando 1.";
GLFWwindow* window = nullptr;

float verticesLine[] = {
	-1.0,0.0,
	1.0,0.0
};

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
	/** Törlési szín beállítása pirosra 1.0 áttetszõség mellett. (red, green, blue, alpha) [0.0, 1.0] */
	/** Set the clearing color for red at 1.0 transparency. (red, green, blue, alpha) [0.0, 1.0] */
	glClearColor(1, 1, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create the VAO Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // Generáljuk le a hogy vertex array amit kitűzűnk
	glBindVertexArray(VAO); // Le kell bindolni

	// VBO Vertex Buffer Object Mem foglalás
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	string vertexShaderSource = readShaderSource("vertexShaderLine.glsl");
	string fragmentShaderSource = readShaderSource("fragmentShaderLine.glsl");


	unsigned int shader = CreateShader(vertexShaderSource, fragmentShaderSource);
	glUseProgram(shader);
}

/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
/** Call display function which will draw into the GLFWwindow object. */
void display() {
	/** Töröljük le a színbuffert! */
	/** Let's clear the color buffer! */
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(3);
	glDrawArrays(GL_LINE_LOOP, 0, 2);
}

/** Felesleges objektumok törlése. */
/** Clenup the unnecessary objects. */
void cleanUpScene() {
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
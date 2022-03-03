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

/** Hagyományos tömb is használható a csúcspontok tárolásához. */
/** C array can be used to store vertices also. */
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
		GLdouble theta = (i * (atan(1) * 4) / 180);
		GLdouble x = 0.16 * cosf(theta); // 0-1 300px 1/6 0,16 -> 50px
		GLdouble y = 0.16 * sinf(theta);
		if (i % 2 == 0) {
			verticesCircle.push_back(glm::vec3(x, y, 0));
			CircleColor.push_back(glm::vec3(0.0, 0.5, 0.0));
			CircleColor2.push_back(glm::vec3(1.0, 0.0, 0.0));
		}
		else {
			verticesCircle.push_back(glm::vec3(0, 0, 0));
			CircleColor.push_back(glm::vec3(1.0, 0.0, 0.0));
			CircleColor2.push_back(glm::vec3(0.0, 0.5, 0.0));
		}

	}
}

/* Vertex buffer objektum és vertex array objektum az adattároláshoz. */
/* Vertex buffer object ID and vertex array object ID for data storing.*/
#define		numVBOs			3
#define		numVAOs			2
GLuint		VBO[numVBOs];
GLuint		VAO[numVAOs];

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Beadando1";
/** A normál billentyûk a [0..255] tartományban vannak, a nyilak és a speciális billentyûk pedig a [256..511] tartományban helyezkednek el. */
/** Normal keys are fom [0..255], arrow and special keys are from [256..511]. */
GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;
GLuint		LineProgram;
GLuint		CircleProgram;
GLuint		XoffsetLocation;
GLuint		YoffsetLocation;

float		x = 0.00f;
float		y = 0.00f;
float		increment = 0.01f;

bool		xDir = true;
bool		yDir = false;

bool checkOpenGLError() {
	bool	foundError = false;
	int		glErr = glGetError();

	/** Vizsgáljuk meg, hogy van-e aktuálisan OpenGL hiba, és amennyiben igen, írassuk ki azokat a konzolra egyenként. */
	/** Check for OpenGL errors, and send them to the console ony by one. */
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;

		foundError = true;
		glErr = glGetError();
	}

	/** Ha van aktuálisan OpenGL hiba, a visszatérési érték true. */
	/** If there are OpenGL errors, the return value is true. */
	return foundError;
}

void printShaderLog(GLuint shader) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;

	/** Vizsgáljuk meg, hogy van-e valami a Shader Info Logban, és amennyiben igen, írassuk ki azt a konzolra soronként. */
	/** Check for Shader Info Log, and send it to the console by lines if it is created for the last compile. */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);

		/** Olvassuk és írassuk ki a következõ sort. */
		/** Read out and and send to the console the next line. */
		glGetShaderInfoLog(shader, length, &charsWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;

	/** Vizsgáljuk meg, hogy van-e valami a Program Info Logban, és amennyiben igen, írassuk ki azt a konzolra soronként. */
	/** Check for Program Info Log, and send it to the console by lines if it is created for the last compile. */
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);

		/** Olvassuk és írassuk ki a következõ sort. */
		/** Read out and and send to the console the next line. */
		glGetProgramInfoLog(prog, length, &charsWritten, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

string readShaderSource(const char* filePath) {
	/** A file stream inicializálása olvasásra. */
	/** Let's initialize the file stream for reading. */
	ifstream	fileStream(filePath, ios::in);
	string		content;
	string		line;

	/** A shader fájl sorainak beolvasása EOF jelzésig. (EOF = End Of File) */
	/** Read in the lines of the shader file until EOF. (EOF = End Of File) */
	while (!fileStream.eof()) {
		getline(fileStream, line);
		/** A shader fájl sorainak összefûzése. */
		/** Append the lines of the shader file. */
		content.append(line + "\n");
	}

	/** A file stream lezárása. */
	/** Let's close the file stream. */
	fileStream.close();

	/** Visszatérés a shader fájl tartalmával. */
	/** Return the content of the shader file. */
	return content;
}

GLuint createShaderProgramforLine() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	/** Beolvassuk a shader fájlok tartalmát. */
	/** Read in both shader files. */
	string		vertShaderStr = readShaderSource("vertexShaderLine.glsl");
	string		fragShaderStr = readShaderSource("fragmentShaderLine.glsl");
	/** Létrehozzuk a shader objektumokat és eltároljuk az ID-ket. */
	/** Let's create the shader objects and store the IDs. */
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	/** A shader fájlok tartalmát eltároló string objektum szöveggé konvertálásás is elvégezzük. */
	/** The contents of the shader string objects shall be converted to text of characters. */
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	/** Ekkor a betöltött kódot hozzárendelhetjük a shader objektumainkhoz. */
	/** The loaded source codes are assigned to the shader objects. */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	/** Fordítsuk le a vertex shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the vertex shader object. */
	glCompileShader(vShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}

	/** Fordítsuk le a fragment shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the fragment shader object. */
	glCompileShader(fShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}

	/** Shader program objektum létrehozása: ez fogja össze a shadereket. Eltároljuk az ID értéket. */
	/** Shader program object initialization: holds together the shaders. vfProgram stores the ID. */
	GLuint		vfProgram = glCreateProgram();
	/** Csatoljuk a shadereket az elõzõ lépésben létrehozott objektumhoz. */
	/** The shaders are attached to the program object. */
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	/** Végül a GLSL szerkesztõ ellenõrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** GLSL linker checks the shaders for compatibility. */
	glLinkProgram(vfProgram);
	/** Hibakeresési lépések. Például sikeres volt-e az összeszerkesztés? Ha nem, mi volt az oka? */
	/** Error checking. Was the link step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}

	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetõek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	/** Az elkészült program azonosítója a visszatérési értékünk. */
	/** The program ID will be the return value. */
	return vfProgram;
}

GLuint createShaderProgramforCircle() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	/** Beolvassuk a shader fájlok tartalmát. */
	/** Read in both shader files. */
	string		vertShaderStr = readShaderSource("vertexShaderCircle.glsl");
	string		fragShaderStr = readShaderSource("fragmentShaderCircle.glsl");
	/** Létrehozzuk a shader objektumokat és eltároljuk az ID-ket. */
	/** Let's create the shader objects and store the IDs. */
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	/** A shader fájlok tartalmát eltároló string objektum szöveggé konvertálásás is elvégezzük. */
	/** The contents of the shader string objects shall be converted to text of characters. */
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	/** Ekkor a betöltött kódot hozzárendelhetjük a shader objektumainkhoz. */
	/** The loaded source codes are assigned to the shader objects. */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	/** Fordítsuk le a vertex shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the vertex shader object. */
	glCompileShader(vShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}

	/** Fordítsuk le a fragment shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the fragment shader object. */
	glCompileShader(fShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}

	/** Shader program objektum létrehozása: ez fogja össze a shadereket. Eltároljuk az ID értéket. */
	/** Shader program object initialization: holds together the shaders. vfProgram stores the ID. */
	GLuint		vfProgram = glCreateProgram();
	/** Csatoljuk a shadereket az elõzõ lépésben létrehozott objektumhoz. */
	/** The shaders are attached to the program object. */
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	/** Végül a GLSL szerkesztõ ellenõrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** GLSL linker checks the shaders for compatibility. */
	glLinkProgram(vfProgram);
	/** Hibakeresési lépések. Például sikeres volt-e az összeszerkesztés? Ha nem, mi volt az oka? */
	/** Error checking. Was the link step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}

	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetõek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	/** Az elkészült program azonosítója a visszatérési értékünk. */
	/** The program ID will be the return value. */
	return vfProgram;
}

void init(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	LineProgram = createShaderProgramforLine();
	CircleProgram = createShaderProgramforCircle();

	/* Létrehozzuk a szükséges vertex buffer és vertex array objektumokat. */
	/* Create the vertex buffer and vertex array objects. */
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);


	// Circle
	CreateCircleLinesandColors();

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	glBufferData(GL_ARRAY_BUFFER, verticesCircle.size() * sizeof(glm::vec3), verticesCircle.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	glGenBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	glBufferData(GL_ARRAY_BUFFER, CircleColor.size() * sizeof(glm::vec3), CircleColor.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Line

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glClearColor(0.5, 0.5, 0.0, 1.0);
}

void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! */
	/* Let's clear the selected buffers! */
	glClear(GL_COLOR_BUFFER_BIT); // fontos lehet minden egyes alkalommal törölni!


	/** A megadott adatok segítségével háromszöget rajzolunk. */
	/** We draw triangle with the defined arrays. */

	glUseProgram(0);
	glUseProgram(LineProgram);
	glBindVertexArray(VAO[1]);
	glLineWidth(3); // vonal vastagság 3px
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);


	glUseProgram(0);
	glUseProgram(CircleProgram);
	glBindVertexArray(VAO[0]);
	//glDrawArrays(GL_LINE_LOOP, 0, verticesCircle.size());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCircle.size());
	if (xDir) {
		x += increment;
		if (x > 0.84f) increment = -0.01f;
		if (x < -0.84f) increment = 0.01f;
		GLuint offsetLoc = glGetUniformLocation(CircleProgram, "offsetX");
		glProgramUniform1f(CircleProgram, offsetLoc, x);
	}
	if (yDir) {
		y += increment;
		if (y > 0.84f) increment = -0.01f; // 0.84 fogja a körvonal elérni a szélét
		if (y < -0.84f) increment = 0.01f;
		GLuint offsetLoc = glGetUniformLocation(CircleProgram, "offsetY");
		glProgramUniform1f(CircleProgram, offsetLoc, y);
	}
	if ((y >= -0.1 && y <= 0.1) && (x >= -0.33 && x <= 0.33)) {
		cout << "X: ";
		cout << x << endl;
		cout << "Y: ";
		cout << y << endl;
		//glPro
	}


	glBindVertexArray(0);
}

/** Felesleges objektumok törlése. */
/** Clenup the unnecessary objects. */
void cleanUpScene() {

	glDeleteVertexArrays(numVAOs, VAO);
	glDeleteBuffers(numVBOs, VBO);

	/** Töröljük a shader programot. */
	/** Let's delete the shader program. */
	glDeleteProgram(LineProgram);
	glDeleteProgram(CircleProgram);
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

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;

	/** A kezelt képernyõ beállítása a teljes (0, 0, width, height) területre. */
	/** Set the viewport for the full (0, 0, width, height) area. */
	glViewport(0, 0, width, height);
}

float RandomFloat(float left_side, float right_side) {

	return right_side + ((float)rand()) / (float)RAND_MAX * (left_side - right_side);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/** ESC billentyûre kilépés. */
	/** Exit on ESC key. */
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();

	/** A billentyûk lenyomásának és felengedésének regisztrálása. Lehetõvé teszi gombkombinációk használatát. */
	/** Let's register press and release events for keys. Enables the usage of key combinations. */
	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;

	/** A V billentyû lenyomása vertikális mozgást indukál. */
	/** Pressing V induces vertical movement. */
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		xDir = false;
		yDir = true;
	}
	/** A H billentyû lenyomása horizontális mozgást indukál. */
	/** Pressing H induces horizontal movement. */
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		xDir = true;
		yDir = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		xDir = true;
		yDir = true;
		x = RandomFloat(-1.0f, 1.0f);
		y = RandomFloat(-1.0f, 1.0f);
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	/** Az egér mutató helyét kezelõ függvény. */
	/** Callback function for mouse position change. */
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	/** Az egér gombjaihoz köthetõ események kezelése. */
	/** Callback function for mouse button events. */
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

	/** A képernyõ átméretezés kezelése. */
	/** Callback function for window size change. */
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	/** Billentyûzethez köthetõ események kezelése. */
	/** Callback function for keyboard events. */
	glfwSetKeyCallback(window, keyCallback);
	/** Az egér mutató helyét kezelõ függvény megadása. */
	/** Callback function for mouse position change. */
	glfwSetCursorPosCallback(window, cursorPosCallback);
	/** Az egér gombjaihoz köthetõ események kezelése. */
	/** Callback function for mouse button events. */
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	/** Incializáljuk a GLEW-t, hogy elérhetõvé váljanak az OpenGL függvények, probléma esetén kilépés EXIT_FAILURE értékkel. */
	/** Initalize GLEW, so the OpenGL functions will be available, on problem exit with EXIT_FAILURE code. */
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	/** 0 = v-sync kikapcsolva, 1 = v-sync bekapcsolva, n = n db képkockányi idõt várakozunk */
	/** 0 = v-sync off, 1 = v-sync on, n = n pieces frame time waiting */
	glfwSwapInterval(1);

	/** A window ablak minimum és maximum szélességének és magasságának beállítása. */
	/** The minimum and maximum width and height values of the window object. */
	glfwSetWindowSizeLimits(window, 400, 400, 800, 800);
	/** A window oldalarányának megadása a számláló és az osztó segítségével. */
	/** Setting the aspect ratio using the numerator and the denominator values. */
	glfwSetWindowAspectRatio(window, 1, 1);

	/** Az alkalmazáshoz kapcsolódó elõkészítõ lépések, pl. a shader objektumok létrehozása. */
	/** The first initialization steps of the program, e.g.: creating the shader objects. */
	init(window);

	/** A megadott window struktúra "close flag" vizsgálata. */
	/** Checks the "close flag" of the specified window. */
	while (!glfwWindowShouldClose(window)) {
		/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
		/** Call display function which will draw into the GLFWwindow object. */
		display(window, glfwGetTime());
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
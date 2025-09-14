/*
 * Un petit récapitulatif des étapes effectuées pour dessiner le triangle:
 *
 *  1) Initialisation:
 *     - Création du VBO
 *     - Binding du VBO
 *     - Envoie des données de vertex
 *     - Débinding du VBO
 *     - Création du VAO
 *     - Binding du VAO
 *     - Activation de l'attribut de sommet 0 (la position)
 *     - Spécification de l'attribut de sommet 0
 *     - Débinding du VAO
 *
 *  2) Dessin:
 *     - Binding du VAO
 *     - Appel à la fonction de dessin
 *     - Débinding du VAO
 *
 *  3) Libération des resources
 *
 */


#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <vector>

int window_width  = 800;
int window_height = 800;

struct Vertex2DUV {

	glm::vec2 position;
	glm::vec2 texture;

	Vertex2DUV() : position(0, 0), texture(0, 0) {};
	Vertex2DUV(glm::vec2 position) : position(position), texture(0, 0) {};
	Vertex2DUV(glm::vec2 position, glm::vec2 texture) : position(position), texture(texture) {};
};

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	// Si on appuie sur la touche "Q" (qwerty -> "A" en azerty)
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

		// Fermer la fenêtre
		glfwSetWindowShouldClose (window, GLFW_TRUE);
	}
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double /*xpos*/, double /*ypos*/)
{
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
	window_width  = width;
	window_height = height;
}

int main(int /*argc*/, char** argv)
{
	/* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a window and its OpenGL context */
#ifdef __APPLE__
	/* We need to explicitly ask for a 3.3 context on Mac */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP2", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Intialize glad (loads the OpenGL functions) */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	GLint uTimeLocation;
	float uTimeValue = 0;

	// Chargement des shaders, compilation et d'indiquer à OpenGL de les utiliser.
	// ⚠️ On doit mettre ces variables dans un nouveau scope car sinon elle ne sont pas bien détruite à la fin du programme ce qui cause une erreur de segmentation
	{
		// On récupère le chemin d'ou est executé le programme
		glimac::FilePath applicationPath(argv[0]);

		// On charge les shaders
		glimac::Program program = loadProgram(applicationPath.dirPath() + "TP2/shaders/text2D.vs.glsl",
											  applicationPath.dirPath() + "TP2/shaders/text2D.fs.glsl");
		// On dit à OpenGL de les utiliser
		program.use();

		uTimeLocation = glGetUniformLocation(program.getGLId(), "uTime");

		glUniform1f(uTimeLocation, uTimeValue);
	}

	/* Hook input callbacks */
	glfwSetKeyCallback(window, &key_callback);
	glfwSetMouseButtonCallback(window, &mouse_button_callback);
	glfwSetScrollCallback(window, &scroll_callback);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
	glfwSetWindowSizeCallback(window, &size_callback);

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	// Création du VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Binding du VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Création des données des sommets
	std::vector<Vertex2DUV> vertices = {
		Vertex2DUV(glm::vec2(-1, -1)),
		Vertex2DUV(glm::vec2(1, -1)),
		Vertex2DUV(glm::vec2(0, 1))
	};

	// On rempli les données du VBO
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex2DUV),
		vertices.data(),
		GL_STATIC_DRAW);

	// On débind le VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// Création du VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// On bind le VAO
	glBindVertexArray(vao);

	// On créer des nouveax attribut de vertex
	const GLuint VERTEX_ATTR_POSITION = 0;
	const GLuint VERTEX_ATTR_TEXTURE = 1;

	// On active les nouveaux attribut vertex
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

	// On rebind le VBO pour définir les données qu'on veut lire
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// On fait la spéfication de nos attributs de vertex

	glVertexAttribPointer(
		VERTEX_ATTR_POSITION,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2DUV),
		(const GLvoid*) (offsetof(Vertex2DUV, position))
	);

	glVertexAttribPointer(
		VERTEX_ATTR_TEXTURE,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex2DUV),
		(const GLvoid*) (offsetof(Vertex2DUV, texture))
	);

	// On débind le VAO
	glBindVertexArray(0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		/*********************************
		* HERE SHOULD COME THE RENDERING CODE
		*********************************/

		uTimeValue += 0.01;
		glUniform1f(uTimeLocation, uTimeValue);

		// On bind le VAO pour récupérer les données
		glBindVertexArray(vao);

		// On déssiner les données dans le VAO
		glDrawArrays(
			GL_TRIANGLES,
			0,
			vertices.size()
		);

		// On débind le VAO
		glBindVertexArray(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	// Libération des ressources :
	// On libère les données du vbo & vao
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
	return 0;
}

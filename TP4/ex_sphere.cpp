#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>

int window_width  = 800;
int window_height = 800;

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

int main(int /*argc*/, char** /*argv*/)
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

	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "TP4", nullptr, nullptr);

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

	/* Hook input callbacks */
	glfwSetKeyCallback(window, &key_callback);
	glfwSetMouseButtonCallback(window, &mouse_button_callback);
	glfwSetScrollCallback(window, &scroll_callback);
	glfwSetCursorPosCallback(window, &cursor_position_callback);
	glfwSetWindowSizeCallback(window, &size_callback);

	/*********************************
	 * HERE SHOULD COME THE INITIALIZATION CODE
	 *********************************/

	// Mise en place du VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glimac::Sphere sphere(1, 32, 16);
	
	const glimac::ShapeVertex* vertices = sphere.getDataPointer();
	const GLsizei vertices_size = sphere.getVertexCount();

	// Remplissage du VBO, on envoie les données des vertices
	glBufferData(
		GL_ARRAY_BUFFER, // La cible ou le VBO est bindé
		vertices_size * sizeof(glimac::ShapeVertex), // Ici la taille du tableau est celle du nombre de vertices + la taille des données d'un vertexe
		vertices, // Il faut donner seulement les données de l'objet
		GL_STATIC_DRAW
	); // Un flag qui dit à OpenGL ce qu'on va faire du buffer (ici on va juste faire un dessin satatique)

	// On débind le buffer quand on as fini pour évitez de le modifer par erreur
	glBindBuffer (GL_ARRAY_BUFFER, 0);


	// Création du VAO (Vertex Array Object) -> Il vont nous servir à aider OpenGL à interprétez les données qu'on lui as envoyé dans le VBO
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// On bind le VAO avant de l'utiliser
	glBindVertexArray(vao);

	// Activation des attribut de vertex
	
	// Pour rendre le code plus clair ou utilise souvents des constantes pour définir les indexs des attribut.
	const GLuint VERTEX_ATTR_POSITION = 3;
	const GLuint VERTEX_ATTR_NORMAL = 5;
	const GLuint VERTEX_ATTR_TEXCOORDS = 8;

	// On active les deux attributs !
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
	glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

	// On rebind le VBO afin de le défnir comme les données qu'on veut lire
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(
		VERTEX_ATTR_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		(const GLvoid*) (offsetof(glimac::ShapeVertex, position))
	);

	glVertexAttribPointer(
		VERTEX_ATTR_NORMAL,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		(const GLvoid*) (offsetof(glimac::ShapeVertex, normal))
	);

	// On débind le VAO pour pas le remodifier par erreur
	glBindVertexArray(vao);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*********************************
		* HERE SHOULD COME THE RENDERING CODE
		*********************************/
	
		// on nettoie la fenêtre pour ne pas avoir le résidu du rendu précédent,
		// Dans glClear on met le flag pour définir ce qu'on veut nettoyer
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

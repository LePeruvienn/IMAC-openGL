/*
 * Un petit récapitulatif des étapes effectuées pour dessiner le disque:
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

#include <cstdint>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>
#include <vector>

int window_width  = 800;
int window_height = 800;

// Structure exo03
struct Vertex2DColor {

	glm::vec2 position;
	glm::vec3 color;
	Vertex2DColor () : position(0,0), color(0,0,0) {};
	Vertex2DColor (glm::vec2 position, glm::vec3 color) : position(position), color(color) {};
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


	// Chargement des shaders, compilation et d'indiquer à OpenGL de les utiliser.
	// ⚠️ On doit mettre ces variables dans un nouveau scope car sinon elle ne sont pas bien détruite à la fin du programme ce qui cause une erreur de segmentation
	{
		// On récupère le chemin d'ou est executé le programme
		glimac::FilePath applicationPath(argv[0]);

		// On charge les shaders
		glimac::Program program = loadProgram(applicationPath.dirPath() + "TP1/shaders/triangle.vs.glsl",
											  applicationPath.dirPath() + "TP1/shaders/triangle.fs.glsl");
		// On dit à OpenGL de les utiliser
		program.use();
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

	// Création d'un seul VBO (Vertex Buffer Object)
	GLuint vbo;
	glGenBuffers(1, &vbo);
	// A partir de ce point, la variable vbo contient l'identifiant d'un VBO

	// Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER
	
	// Créations des vertices du disque
	// Pour cela il faut définit d'abord notre tableau de triangles à déssiner
	std::vector<Vertex2DColor> vertices;

	// Création de la liste des indices
	// Ici on va stocker tiout les indices
	std::vector<uint32_t> indices;

	const int N = 10; // Nombre de trinagle qu'on veut déssiner
	const int R = 1; // Rayon de notre disque
	const float PI = glm::pi<float>(); // Nombre PI
	
	// Ajout du point au centre
	vertices.emplace_back (glm::vec2(0, 0), glm::vec3(0, 0 ,1));

	// On un nombre de point égale au nombre de triangle
	for (int i = 1; i <= N; i++) {

		// Angle du premier et du deuxième point sur le cercle
		float theta = (2 * PI * i) / N;

		// On calcule les positions des deux différents points
		float x = R * glm::cos(theta);
		float y = R * glm::sin(theta);

		// On ajoute le vertexe à la liste
		vertices.emplace_back (glm::vec2(x, y), glm::vec3(1, 1 ,0));

		// On ajoute la liste les index du trinagle
		indices.emplace_back (0); // Centre
		indices.emplace_back (i); // Point actuellement ajouter

		// Si on est à la fin de la boucle, on ferme la boucle en ajoutant le premier point
		if (i == N)
			indices.emplace_back (1);
		// Sinon, le on ajoute la prochain indice comme dernier point du triangle
		else
			indices.emplace_back (i + 1); // Le prochain point qu'on va ajouter
	}

	// Remplissage du VBO, on envoie les données des vertices
	glBufferData(
		GL_ARRAY_BUFFER, // La cible ou le VBO est bindé
		vertices.size() * sizeof(Vertex2DColor), // Ici la taille du tableau est celle du nombre de vertices + la taille des données d'un vertexe
		vertices.data(), // Il faut donner seulement les données de l'objet
		GL_STATIC_DRAW); // Un flag qui dit à OpenGL ce qu'on va faire du buffer (ici on va juste faire un dessin satatique)
	
	// On débind le buffer quand on as fini pour évitez de le modifer par erreur
	glBindBuffer (GL_ARRAY_BUFFER, 0);

	// => Creation du IBO
	GLuint ibo;
	glGenBuffers(1, &ibo);

	// => On bind sur GL_ELEMENT_ARRAY_BUFFER, cible reservée pour les IBOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// => On remplit l'IBO avec les indices:
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(uint32_t),
		indices.data(),
		GL_STATIC_DRAW
	);

	// => Comme d'habitude on debind avant de passer à autre chose
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Création du VAO (Vertex Array Object) -> Il vont nous servir à aider OpenGL à interprétez les données qu'on lui as envoyé dans le VBO
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// On bind le VAO avant de l'utiliser
	glBindVertexArray(vao);

	// => On bind l'IBO sur GL_ELEMENT_ARRAY_BUFFER; puisqu'un VAO est actuellement bindé,
	// cela a pour effet d'enregistrer l'IBO dans le VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Activation des attribut de vertex
	
	// Pour rendre le code plus clair ou utilise souvents des constantes pour définir les indexs des attribut.
	// Ici on voit que dans notre shader (triangle.vs.glsl), les attributs sont 3 et 8.
	const GLuint VERTEX_ATTR_POSITION = 3;
	const GLuint VERTEX_ATTR_COLOR = 8;

	// On active les deux attributs !
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

	// ⚠️ les index des attribut et ceux de la liste ne sont pas du tout la meme chose il ne faut pas se confondre
	

	// On rebind le VBO afin de le défnir comme les données qu'on veut lire
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Spécifcation des données de position
	glVertexAttribPointer(
		VERTEX_ATTR_POSITION, // Index de l'attribut
		2, // Nombre de composante de l'attribut
		GL_FLOAT, // Type de l'attribut
		GL_FALSE, // GLNormalized : pas besoin de s'occuper de ça pour le moment
		sizeof(Vertex2DColor), // Combien d'octet mesure chaque attribut (ici notre position est 2 GLfloat donc 2 fois leurs taille)
		(const GLvoid*) (offsetof(Vertex2DColor, position))); // Offset par rapport au début de l'attribut, on veut l'offset de la position

	// Spécifcation des données de couleurs
	glVertexAttribPointer(
		VERTEX_ATTR_COLOR, // Index de l'attribut
		3, // Nombre de composante de l'attribut
		GL_FLOAT, // Type de l'attribut
		GL_FALSE, // GLNormalized : pas besoin de s'occuper de ça pour le moment
		sizeof(Vertex2DColor), // Combien d'octet mesure chaque attribut (ici tout les données sont dans un seul struc Vertex2DColor
		(const GLvoid*) (offsetof(Vertex2DColor, color))); // Offset par rapport au début de l'attribut, on veut l'offset de la couleur
	

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

		// Désinner en utilisant le VAO :
	
		// d'abord on rebind le VAO
		glBindVertexArray(vao);

		// On fait un drawcall sur le VAO
		glDrawElements(
			GL_TRIANGLES,
			indices.size(),
			GL_UNSIGNED_INT,
			0
		);

		// On debind le VAO afin qu'on ne le modifie pas par erreur dans le reste du code
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

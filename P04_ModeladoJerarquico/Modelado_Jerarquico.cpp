// Practica 4: Modelado jerarquico
// Brazo articulado: brazo -> antebrazo -> mano -> 3 dedos (2 falanges cada uno)
// Rodrigo Jardon Marin - 321240693

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);

const GLint WIDTH = 800, HEIGHT = 600;

// Camara
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;
float rotX = 0.0f; // extra: girar la camara hacia arriba/abajo (Q y E)

// Articulaciones
float hombro = 0.0f;
float codo = 0.0f;
float mano = 0.0f;
float dedo1_base = 0.0f;
float dedo1_punta = 0.0f;
float dedo2_base = 0.0f;
float dedo2_punta = 0.0f;
float dedo3_base = 0.0f;
float dedo3_punta = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Cubo base (solo posiciones, el color se manda por uniform)
	float vertices[] = {
		-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f, // Front
		 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f, // Back
		 0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,

		 0.5f, -0.5f,  0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f, // Right
		 0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,   0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f, // Left
		-0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f, // Bottom
		 0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f, // Top
		 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Proyeccion y color
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);

	// Ubicaciones de los uniform (una sola vez)
	ourShader.Use();
	GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
	GLint uniformColor = ourShader.getColorLocation();

	glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f);
		glm::mat4 modelTemp2 = glm::mat4(1.0f);

		// Camara / Vista
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		// --- BRAZO ---
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- ANTEBRAZO ---
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); // Rojo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- MANO ---
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mano), glm::vec3(0.0f, 0.0f, 1.0f));

		// IMPORTANTE: Guardamos el pivote en "modelTempMano" para que los dedos partan de aqui
		glm::mat4 modelTempMano = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Mano Azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// --- DEDO 1 (Superior) ---
		// Falange 1 (Base): partimos de modelTempMano, avanzamos 0.25 en X (orilla) y 0.35 en Y (arriba)
		model = glm::translate(modelTempMano, glm::vec3(0.25f, 0.0f, 0.35f));
		model = glm::rotate(model, glm::radians(dedo1_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Empuje (mitad de 0.4)
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.2f)); // Escala de 0.4
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Falange 2 (Punta): partimos de modelTemp (centro de la falange 1)
		model = glm::translate(modelTemp, glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo1_punta), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f)); // Empuje (mitad de 0.3)
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f)); // Escala de 0.3
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- DEDO 2 (Medio) ---
		// Falange 1 (Base): partimos de modelTempMano, avanzamos 0.25 en X (orilla), sin movimiento en Y (centro)
		model = glm::translate(modelTempMano, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Empuje (mitad de 0.4)
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.2f)); // Escala de 0.4
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Falange 2 (Punta): partimos de modelTemp (centro de la falange 1)
		model = glm::translate(modelTemp, glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo2_punta), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f)); // Empuje (mitad de 0.3)
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f)); // Escala de 0.3
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// --- DEDO 3 (Inferior) ---
		// Falange 1 (Base): partimos de modelTempMano, avanzamos 0.25 en X (orilla) y -0.35 en Y (abajo)
		model = glm::translate(modelTempMano, glm::vec3(0.25f, 0.0f, -0.35f));
		model = glm::rotate(model, glm::radians(dedo3_base), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Empuje (mitad de 0.4)
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.2f)); // Escala de 0.4
		color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Falange 2 (Punta): partimos de modelTemp (centro de la falange 1)
		model = glm::translate(modelTemp, glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(dedo3_punta), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f)); // Empuje (mitad de 0.3)
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f)); // Escala de 0.3
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Camara
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rotX += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotX -= 0.4f;

	// Brazo Principal
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) hombro += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) hombro -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) codo += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) codo -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) mano += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) mano -= 0.18f;

	// --- Controles Dedo 1 ---
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) dedo1_base += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) dedo1_base -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) dedo1_punta += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) dedo1_punta -= 0.18f;

	// --- Controles Dedo 2 ---
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) dedo2_base += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) dedo2_base -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) dedo2_punta += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) dedo2_punta -= 0.18f;

	// --- Controles Dedo 3 ---
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) dedo3_base += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) dedo3_base -= 0.18f;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) dedo3_punta += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) dedo3_punta -= 0.18f;
}

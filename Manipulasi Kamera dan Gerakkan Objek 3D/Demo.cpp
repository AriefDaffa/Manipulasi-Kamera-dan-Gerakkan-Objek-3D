#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		RenderEngine& app = Demo();
		app.Start("Transformation: Transform Cube", 800, 600, false, false, 20);

	}

	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		RenderEngine& app = Demo();
		app.Start("Transformation: Transform Cube", 800, 600, false, false, 10);

	}
}

void Demo::Update(double deltaTime) {
	angle += (float)((deltaTime * 1.5f) / 1000);
}

void Demo::Render(int x) {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, x, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//=============== Section Alas meja ===============
		// front
		-0.5, 0.3, 0.5, 0, 0,
		0.5, 0.3, 0.5, 1, 0,
		0.5,  0.5, 0.5, 1, 1,
		-0.5,  0.5, 0.5, 0, 1,

		// right
		0.5,  0.5,  0.5, 0, 0,
		0.5,  0.5, -0.5, 1, 0,
		0.5, 0.3, -0.5, 1, 1,
		0.5, 0.3,  0.5, 0, 1,

		// back
		-0.5, 0.3, -0.5, 0, 0,
		0.5,  0.3, -0.5, 1, 0,
		0.5,   0.5, -0.5, 1, 1,
		-0.5,  0.5, -0.5, 0, 1,

		// left
		-0.5, 0.3, -0.5, 0, 0,
		-0.5, 0.3,  0.5, 1, 0,
		-0.5,  0.5,  0.5, 1, 1,
		-0.5,  0.5, -0.5, 0, 1,

		// upper
		0.5, 0.5,  0.5, 0, 0,
		-0.5, 0.5,  0.5, 1, 0,
		-0.5, 0.5, -0.5, 1, 1,
		0.5, 0.5, -0.5, 0, 1,

		// bottom
		-0.5, 0.3, -0.5, 0, 0,
		0.5, 0.3, -0.5, 1, 0,
		0.5, 0.3,  0.5, 1, 1,
		-0.5, 0.3,  0.5, 0, 1,

		// =============== Section Kaki kiri depan ===============
		// front
		-0.5, -0.5, 0.5, 0, 0,
		-0.4, -0.5, 0.5, 1, 0,
		-0.4,  0.4, 0.5, 1, 1,
		-0.5,  0.4, 0.5, 0, 1,

		// right
		-0.4,  0.4,  0.5, 0, 0,
		-0.4,  0.4, 0.4, 1, 0,
		-0.4, -0.5, 0.4, 1, 1,
		-0.4, -0.5,  0.5, 0, 1,

		// back
		-0.5, -0.5, 0.4, 0, 0,
		-0.4,  -0.5, 0.4, 1, 0,
		-0.4,   0.4, 0.4, 1, 1,
		-0.5,  0.4, 0.4, 0, 1,

		// left
		-0.5, -0.5, 0.4, 0, 0,
		-0.5, -0.5,  0.5, 1, 0,
		-0.5,  0.4,  0.5, 1, 1,
		-0.5,  0.4, 0.4, 0, 1,

		// upper
		-0.4, 0.4,  0.5, 0, 0,
		-0.5, 0.4,  0.5, 1, 0,
		-0.5, 0.4, 0.4, 1, 1,
		-0.4, 0.4, 0.4, 0, 1,

		// bottom
		-0.5, -0.5, 0.4, 0, 0,
		-0.4, -0.5, 0.4, 1, 0,
		-0.4, -0.5,  0.5, 1, 1,
		-0.5, -0.5,  0.5, 0, 1,

		// =============== Section kanan depan =============== 
		// front
		0.4, -0.5, 0.5, 0, 0,
		0.5, -0.5, 0.5, 1, 0,
		0.5,  0.4, 0.5, 1, 1,
		0.4,  0.4, 0.5, 0, 1,

		// right
		0.5,  0.4,  0.5, 0, 0,
		0.5,  0.4, 0.4, 1, 0,
		0.5, -0.5, 0.4, 1, 1,
		0.5, -0.5,  0.5, 0, 1,

		// back
		0.4, -0.5, 0.4, 0, 0,
		0.5,  -0.5, 0.4, 1, 0,
		0.5,   0.4, 0.4, 1, 1,
		0.4,  0.4, 0.4, 0, 1,

		// left
		0.4, -0.5, 0.4, 0, 0,
		0.4, -0.5,  0.5, 1, 0,
		0.4,  0.4,  0.5, 1, 1,
		0.4,  0.4, 0.4, 0, 1,

		// upper
		0.5, 0.4,  0.5, 0, 0,
		0.4, 0.4,  0.5, 1, 0,
		0.5, 0.4, 0.4, 1, 1,
		0.5, 0.4, 0.4, 0, 1,

		// bottom
		0.4, -0.5, 0.4, 0, 0,
		0.5, -0.5, 0.4, 1, 0,
		0.5, -0.5,  0.5, 1, 1,
		0.4, -0.5,  0.5, 0, 1,

		// =============== Section kanan belakang ===============
		// front
		0.4, -0.5, -0.4, 0, 0,
		0.5, -0.5, -0.4, 1, 0,
		0.5, 0.4, -0.4, 1, 1,
		0.4, 0.4, -0.4, 0, 1,

		// right
		0.5, 0.4, -0.4, 0, 0,
		0.5, 0.4, -0.5, 1, 0,
		0.5, -0.5, -0.5, 1, 1,
		0.5, -0.5, -0.4, 0, 1,

		// back
		0.4, -0.5, -0.5, 0, 0,
		0.5, -0.5, -0.5, 1, 0,
		0.5, 0.4, -0.5, 1, 1,
		0.4, 0.4, -0.5, 0, 1,

		// left
		0.4, -0.5, -0.5, 0, 0,
		0.4, -0.5, -0.4, 1, 0,
		0.4, 0.4, -0.4, 1, 1,
		0.4, 0.4, -0.5, 0, 1,

		// upper
		0.5, 0.4, -0.4, 0, 0,
		0.4, 0.4, -0.4, 1, 0,
		0.5, 0.4, -0.5, 1, 1,
		0.5, 0.4, -0.5, 0, 1,

		// bottom
		0.4, -0.5, -0.5, 0, 0,
		0.5, -0.5, -0.5, 1, 0,
		0.5, -0.5, -0.4, 1, 1,
		0.4, -0.5, -0.4, 0, 1,

		// =============== Section Kaki kiri belakang ===============
		// front
		-0.5, -0.5, -0.4, 0, 0,
		-0.4, -0.5, -0.4, 1, 0,
		-0.4, 0.4, -0.4, 1, 1,
		-0.5, 0.4, -0.4, 0, 1,

		// right
		-0.4, 0.4, -0.4, 0, 0,
		-0.4, 0.4, -0.5, 1, 0,
		-0.4, -0.5, -0.5, 1, 1,
		-0.4, -0.5, -0.4, 0, 1,

		// back
		-0.5, -0.5, -0.5, 0, 0,
		-0.4, -0.5, -0.5, 1, 0,
		-0.4, 0.4, -0.5, 1, 1,
		-0.5, 0.4, -0.5, 0, 1,

		// left
		-0.5, -0.5, -0.5, 0, 0,
		-0.5, -0.5, -0.4, 1, 0,
		-0.5, 0.4, -0.4, 1, 1,
		-0.5, 0.4, -0.5, 0, 1,

		// upper
		-0.4, 0.4, -0.4, 0, 0,
		-0.5, 0.4, -0.4, 1, 0,
		-0.5, 0.4, -0.5, 1, 1,
		-0.4, 0.4, -0.5, 0, 1,

		// bottom
		-0.5, -0.5, -0.5, 0, 0,
		-0.4, -0.5, -0.5, 1, 0,
		-0.4, -0.5, -0.4, 1, 1,
		-0.5, -0.5, -0.4, 0, 1,

		//============= section sekat depan ===============
		// front
		-0.4, 0.1, 0.5, 0, 0,
		0.4, 0.1, 0.5, 1, 0,
		0.4, 0.2, 0.5, 1, 1,
		-0.4, 0.2, 0.5, 0, 1,

		// right
		0.4, 0.2, 0.5, 0, 0,
		0.4, 0.2, 0.4, 1, 0,
		0.4, 0.1, 0.4, 1, 1,
		0.4, 0.1, 0.5, 0, 1,

		// back
		-0.4, 0.1, 0.4, 0, 0,
		0.4, 0.1, 0.4, 1, 0,
		0.4, 0.2, 0.4, 1, 1,
		-0.4, 0.2, 0.4, 0, 1,

		// left
		-0.4, 0.1, 0.4, 0, 0,
		-0.4, 0.1, 0.5, 1, 0,
		-0.4, 0.2, 0.5, 1, 1,
		-0.4, 0.2, 0.4, 0, 1,

		// upper
		0.4, 0.2, 0.5, 0, 0,
		-0.4, 0.2, 0.5, 1, 0,
		-0.4, 0.2, 0.4, 1, 1,
		0.4, 0.2, 0.4, 0, 1,

		// bottom
		-0.4, 0.1, 0.4, 0, 0,
		0.4, 0.1, 0.4, 1, 0,
		0.4, 0.1, 0.5, 1, 1,
		-0.4, 0.1, 0.5, 0, 1,


		//============= section sekat belakang ===============
		// front
		-0.4, 0.1, -0.5, 0, 0,
		0.4, 0.1, -0.5, 1, 0,
		0.4, 0.2, -0.5, 1, 1,
		-0.4, 0.2, -0.5, 0, 1,

		// right
		0.4, 0.2, -0.5, 0, 0,
		0.4, 0.2, -0.4, 1, 0,
		0.4, 0.1, -0.4, 1, 1,
		0.4, 0.1, -0.5, 0, 1,

		// back
		-0.4, 0.1, -0.4, 0, 0,
		0.4, 0.1, -0.4, 1, 0,
		0.4, 0.2, -0.4, 1, 1,
		-0.4, 0.2, -0.4, 0, 1,

		// left
		-0.4, 0.1, -0.4, 0, 0,
		-0.4, 0.1, -0.5, 1, 0,
		-0.4, 0.2, -0.5, 1, 1,
		-0.4, 0.2, -0.4, 0, 1,

		// upper
		0.4, 0.2, -0.5, 0, 0,
		-0.4, 0.2, -0.5, 1, 0,
		-0.4, 0.2, -0.4, 1, 1,
		0.4, 0.2, -0.4, 0, 1,

		// bottom
		-0.4, 0.1, -0.4, 0, 0,
		0.4, 0.1, -0.4, 1, 0,
		0.4, 0.1, -0.5, 1, 1,
		-0.4, 0.1, -0.5, 0, 1,

		//============= section sekat kiri ===============
		// front
		-0.5, 0.1, 0.4, 0, 0,
		-0.4, 0.1, 0.4, 1, 0,
		-0.4, 0.2, 0.4, 1, 1,
		-0.5, 0.2, 0.4, 0, 1,

		// right
		-0.4, 0.2, 0.4, 0, 0,
		-0.4, 0.2, -0.4, 1, 0,
		-0.4, 0.1, -0.4, 1, 1,
		-0.4, 0.1, 0.4, 0, 1,

		// back
		-0.5, 0.1, -0.4, 0, 0,
		-0.4, 0.1, -0.4, 1, 0,
		-0.4, 0.2, -0.4, 1, 1,
		-0.5, 0.2, -0.4, 0, 1,

		// left
		-0.5, 0.1, -0.4, 0, 0,
		-0.5, 0.1, 0.4, 1, 0,
		-0.5, 0.2, 0.4, 1, 1,
		-0.5, 0.2, -0.4, 0, 1,

		// upper
		-0.4, 0.2, 0.4, 0, 0,
		-0.5, 0.2, 0.4, 1, 0,
		-0.5, 0.2, -0.4, 1, 1,
		-0.4, 0.2, -0.4, 0, 1,

		// bottom
		-0.5, 0.1, -0.4, 0, 0,
		-0.4, 0.1, -0.4, 1, 0,
		-0.4, 0.1, 0.4, 1, 1,
		-0.5, 0.1, 0.4, 0, 1,

		//============= section sekat kanan ===============
		// front
		0.5, 0.1, 0.4, 0, 0,
		0.4, 0.1, 0.4, 1, 0,
		0.4, 0.2, 0.4, 1, 1,
		0.5, 0.2, 0.4, 0, 1,

		// right
		0.4, 0.2, 0.4, 0, 0,
		0.4, 0.2, -0.4, 1, 0,
		0.4, 0.1, -0.4, 1, 1,
		0.4, 0.1, 0.4, 0, 1,

		// back
		0.5, 0.1, -0.4, 0, 0,
		0.4, 0.1, -0.4, 1, 0,
		0.4, 0.2, -0.4, 1, 1,
		0.5, 0.2, -0.4, 0, 1,

		// left
		0.5, 0.1, -0.4, 0, 0,
		0.5, 0.1, 0.4, 1, 0,
		0.5, 0.2, 0.4, 1, 1,
		0.5, 0.2, -0.4, 0, 1,

		// upper
		0.4, 0.2, 0.4, 0, 0,
		0.5, 0.2, 0.4, 1, 0,
		0.5, 0.2, -0.4, 1, 1,
		0.4, 0.2, -0.4, 0, 1,

		// bottom
		0.5, 0.1, -0.4, 0, 0,
		0.4, 0.1, -0.4, 1, 0,
		0.4, 0.1, 0.4, 1, 1,
		0.5, 0.1, 0.4, 0, 1,

	};

	unsigned int indices[] = {
		//Alas Meja
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		//Kaki kiri depan
		24, 25, 26, 24, 26, 27,
		28, 29, 30, 28, 30, 31,
		32, 33, 34, 32, 34, 35,
		36, 38, 37, 36, 39, 38,
		40, 42, 41, 40, 43, 42,
		44, 46, 45, 44, 47, 46,

		//Kaki kanan depan
		48, 49, 50, 48, 50, 51,
		52, 53, 54, 52, 54, 55,
		56, 57, 58, 56, 58, 59,
		60, 62, 61, 60, 63, 62,
		64, 66, 65, 64, 67, 66,
		68, 70, 69, 68, 71, 70,

		//Kaki kanan belakang
		72, 73, 74, 72, 74, 75,
		76, 77, 78, 76, 78, 79,
		80, 81, 82, 80, 82, 83,
		84, 86, 85, 84, 87, 86,
		88, 90, 89, 88, 91, 90,
		92, 94, 93, 92, 95, 94,

		//Kaki kiri belakang
		96, 97, 98, 96, 98, 99,
		100, 101, 102, 100, 102,
		103, 104, 105, 106, 104,
		106, 107, 108, 110, 109,
		108, 111, 110, 112, 114,
		113, 112, 115, 114, 116,
		118, 117, 116, 119, 118,

		//sekat depan
		120, 121, 122, 120, 122, 123,
		124, 125, 126, 124, 126, 127,
		128, 129, 130, 128, 130, 131,
		132, 134, 133, 132, 135, 134,
		136, 138, 137, 136, 139, 138,
		140, 142, 141, 140, 143, 142,

		//sekat belakang
		144, 145, 146, 144, 146, 147,
		148, 149, 150, 148, 150, 151,
		152, 153, 154, 152, 154, 155,
		156, 158, 157, 156, 159, 158,
		160, 162, 161, 160, 163, 162,
		164, 166, 165, 164, 167, 166,

		//sekat kiri
		168, 169, 170, 168, 170, 171,
		172, 173, 174, 172, 174, 175,
		176, 177, 178, 176, 178, 179,
		180, 182, 181, 180, 183, 182,
		184, 186, 185, 184, 187, 186,
		188, 190, 189, 188, 191, 190,

		//sekat kanan
		192, 193, 194, 192, 194, 195,
		196, 197, 198, 196, 198, 199,
		200, 201, 202, 200, 202, 203,
		204, 206, 205, 204, 207, 206,
		208, 210, 209, 208, 211, 210,
		212, 214, 213, 212, 215, 214,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized



	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 3, 0));

	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	model = glm::scale(model, glm::vec3(3, 3, 3));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0,  0,
		 50.0, -0.5, -50.0, 50,  0,
		 50.0, -0.5,  50.0, 50, 50,
		-50.0, -0.5,  50.0,  0, 50,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Transformation: Transform Cube", 800, 600, false, false, 0);
}
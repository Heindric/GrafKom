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
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(0, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
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
		// format position, tex coords
		// bagian dudukan
		// front
		-0.5, 1.3, 0.5, 0, 0,  // left bottom front 0
		0.5,  1.3, 0.5, 1, 0,   // right bottom front 1
		0.5,  1.5, 0.5, 1, 1,   // right top front 2
		-0.5, 1.5, 0.5, 0, 1,  // left top front 3

		// right
		0.5,  1.5,  0.5, 0, 0,  // right top front 4
		0.5,  1.5, -0.5, 1, 0,  // right top back 5
		0.5, 1.3, -0.5, 1, 1,  // right bottom back 6
		0.5, 1.3,  0.5, 0, 1,  // right bottom front 7

		// back
		-0.5, 1.3, -0.5, 0, 0, // left bottom  back 8 
		0.5,  1.3, -0.5, 1, 0, // right bottom back 9
		0.5,   1.5, -0.5, 1, 1, // right top back 10
		-0.5,  1.5, -0.5, 0, 1, // left top back 11

		// left
		-0.5, 1.3, -0.5, 0, 0, // left bottom back 12
		-0.5, 1.3,  0.5, 1, 0, // left bottom front 13
		-0.5,  1.5,  0.5, 1, 1, // left top front 14
		-0.5,  1.5, -0.5, 0, 1, // left top back 15

		// top
		0.5,  1.5,  0.5, 0, 0,   // right top front 16
		-0.5, 1.5,  0.5, 1, 0,  // left top front 17
		-0.5, 1.5, -0.5, 1, 1,  // left top back 18
		0.5,  1.5, -0.5, 0, 1,   // right top back 19

		// bottom
		-0.5, 1.3, -0.5, 0, 0, // left bottom back 20
		0.5,  1.3, -0.5, 1, 0,  // right bottom back 21
		0.5,  1.3,  0.5, 1, 1,  // right bottom front 22
		-0.5, 1.3,  0.5, 0, 1, // left bottom front 23

		// format position, tex coords
		// bagian kaki kiri depan
		// front
		-0.5,  0, 0.5, 0, 0,  // left bottom front 24
		-0.3,  0, 0.5, 1, 0,   // right bottom front 25
		-0.3,  1.3, 0.5, 1, 1,   // right top front 26
		-0.5,  1.3, 0.5, 0, 1,  // left top front 27

		// right
		-0.3,  1.3,  0.5, 0, 0,  // right top front 28
		-0.3,  1.3,  0.3, 1, 0,  // right top back 29
		-0.3,   0,   0.3, 1, 1,  // right bottom back 30
		-0.3,   0,   0.5, 0, 1,  // right bottom front 31

		// back
		-0.5, 0,  0.3, 0, 0, // left bottom  back 32
		-0.3,  0,  0.3, 1, 0, // right bottom back 33
		-0.3,   1.3, 0.3, 1, 1, // right top back 34
		-0.5,  1.3, 0.3, 0, 1, // left top back 35

		// left
		-0.5, 0, 0.3, 0, 0, // left bottom back 36
		-0.5, 0,  0.5, 1, 0, // left bottom front 37
		-0.5, 1.3,  0.5, 1, 1, // left top front 38
		-0.5, 1.3, 0.3, 0, 1, // left top back 39

		//// top
		//-0.3,  2.0,  0.5, 0, 0,   // right top front 40
		//-0.5, 2.0,  0.5, 1, 0,  // left top front 41
		//-0.5, 2.0, -0.3, 1, 1,  // left top back 42
		//-0.3,  2.0, -0.3, 0, 1,   // right top back 43

		// bottom
		-0.5,  0, -0.3, 0, 0, // left bottom back 44
		-0.3,  0, -0.3, 1, 0,  // right bottom back 45
		-0.3,  0,  0.5, 1, 1,  // right bottom front 46
		-0.5,  0,  0.5, 0, 1, // left bottom front 47
			   
		// format position, tex coords
		// bagian kaki kanan depan
		// front
		0.3,  0, 0.5, 0, 0,  // left bottom front 48
		0.5,  0, 0.5, 1, 0,   // right bottom front 49
		0.5,  1.3, 0.5, 1, 1,   // right top front 50
		0.3,  1.3, 0.5, 0, 1,  // left top front 51

		// right
		0.5,  1.3,  0.5, 0, 0,  // right top front 52
		0.5,  1.3, 0.3, 1, 0,  // right top back 53
		0.5,    0, 0.3, 1, 1,  // right bottom back 54
		0.5,    0,  0.5, 0, 1,  // right bottom front 55

		// back
		0.3,     0, 0.3, 0, 0, // left bottom  back 56 
		0.5,     0, 0.3, 1, 0, // right bottom back 57
		0.5,   1.3, 0.3, 1, 1, // right top back 58
		0.3,   1.3, 0.3, 0, 1, // left top back 59

		// left
		0.3, 0, 0.3, 0, 0, // left bottom back 60
		0.3, 0,  0.5, 1, 0, // left bottom front 61
		0.3,  1.3,  0.5, 1, 1, // left top front 62
		0.3,  1.3, 0.3, 0, 1, // left top back 63

		//// top
		//0.5,  2.0,  0.5, 0, 0,   // right top front 64
		//0.3, 2.0,  0.5, 1, 0,  // left top front 65
		//0.3, 2.0, -0.5, 1, 1,  // left top back 66
		//0.5,  2.0, -0.5, 0, 1,   // right top back 67

		// bottom
		0.3,  0, 0.3, 0, 0, // left bottom back 68
		0.5,  0, 0.3, 1, 0,  // right bottom back 69
		0.5,  0,  0.5, 1, 1,  // right bottom front 70
		0.3,  0,  0.5, 0, 1, // left bottom front 71

			

		// format position, tex coords
		// bagian kaki belakang kiri
		// front
			-0.5, 0,  -0.3, 0, 0,  // left bottom front 72
			-0.3, 0,  -0.3, 1, 0,   // right bottom front 73
			-0.3, 1.3,-0.3, 1, 1,   // right top front 74
			-0.5, 1.3,-0.3, 0, 1,  // left top front 75

			// right
			-0.3, 1.3, -0.3, 0, 0,  // right top front 76
			-0.3, 1.3, -0.5, 1, 0,  // right top back 77
			-0.3, 0, -0.5, 1, 1,  // right bottom back 78
			-0.3, 0, -0.3, 0, 1,  // right bottom front 79

			// back
			-0.5, 0, -0.5, 0, 0, // left bottom  back 80 
			-0.3, 0, -0.5, 1, 0, // right bottom back 81
			-0.3, 1.3, -0.5, 1, 1, // right top back 82
			-0.5, 1.3, -0.5, 0, 1, // left top back 83

			// left
			-0.5, 0, -0.5, 0, 0, // left bottom back 84
			-0.5, 0, -0.3, 1, 0, // left bottom front 85
			-0.5, 1.3, -0.3, 1, 1, // left top front 86
			-0.5, 1.3, -0.5, 0, 1, // left top back 87

			//// top
			//-0.3, 1.8, 0.5, 0, 0,   // right top front 88
			//-0.5, 1.8, 0.5, 1, 0,  // left top front 89
			//-0.5, 1.8, -0.5, 1, 1,  // left top back 90
			//-0.3, 1.8, -0.5, 0, 1,   // right top back 91

			// bottom
			-0.5, 0, -0.5, 0, 0, // left bottom back 92
			-0.3, 0, -0.5, 1, 0,  // right bottom back 93
			-0.3, 0, -0.3, 1, 1,  // right bottom front 94
			-0.5, 0, -0.3, 0, 1, // left bottom front 95

		// format position, tex coords
		// bagian kaki belakang kanan
		// front
			0.3, 0, -0.3, 0, 0,  // left bottom front 96
			 0.5, 0, -0.3, 1, 0,   // right bottom front 97
			 0.5, 1.3, -0.3, 1, 1,   // right top front 98
			0.3, 1.3, -0.3, 0, 1,  // left top front 99

			// right
			0.5, 1.3, -0.3, 0, 0,  // right top front 100
			0.5, 1.3, -0.5, 1, 0,  // right top back 101
			0.5, 0, -0.5, 1, 1,  // right bottom back 102
			0.5, 0, -0.3, 0, 1,  // right bottom front 103

			// back
			0.3, 0, -0.5, 0, 0, // left bottom  back 104
			0.5,  0, -0.5, 1, 0, // right bottom back 105
			0.5,  1.3, -0.5, 1, 1, // right top back 106
			0.3, 1.3, -0.5, 0, 1, // left top back 107

			// left
			0.3, 0, -0.5, 0, 0, // left bottom back 108
			0.3, 0, -0.3, 1, 0, // left bottom front 109
			0.3, 1.3, -0.3, 1, 1, // left top front 110
			0.3, 1.3, -0.5, 0, 1, // left top back 111

			//// top
			//0.5, 2.0, -0.3, 0, 0,   // right top front 112
			//-0.5, 2.0,-0.3, 1, 0,  // left top front 113
			//-0.5, 2.0, -0.5, 1, 1,  // left top back 114
			//0.5, 2.0, -0.5, 0, 1,   // right top back 115

			// bottom
			0.3, 0, -0.5, 0, 0, // left bottom back 116
			0.5,  0, -0.5, 1, 0,  // right bottom back 117
			 0.5, 0, -0.3, 1, 1,  // right bottom front 118
			-0.5, 0, -0.3, 0, 1, // left bottom front 119

			// format position, tex coords
		// bagian sandaran
		// front
			-0.5, 1.5, -0.3, 0, 0,  // left bottom front 120
			0.5,  1.5, -0.3, 1, 0,   // right bottom front 121
			0.5,  2.5, -0.3, 1, 1,   // right top front 122
			-0.5, 2.5, -0.3, 0, 1,  // left top front 123

			// right
			0.5, 2.5, -0.3, 0, 0,  // right top front 124
			0.5, 2.5, -0.5, 1, 0,  // right top back 125
			0.5, 1.5, -0.5, 1, 1,  // right bottom back 126
			0.5, 1.5, -0.3, 0, 1,  // right bottom front 127

			// back
			-0.5, 1.5, -0.5, 0, 0, // left bottom  back 128
			0.5,  1.5, -0.5, 1, 0, // right bottom back 129
			0.5,  2.5, -0.5, 1, 1, // right top back 130
			-0.5, 2.5, -0.5, 0, 1, // left top back 131

			// left
			-0.5, 1.5, -0.5, 0, 0, // left bottom back 132
			-0.5, 1.5, -0.3, 1, 0, // left bottom front 133
			-0.5, 2.5, -0.3, 1, 1, // left top front 134
			-0.5, 2.5, -0.5, 0, 1, // left top back 135

			// top
			0.5,  2.5, -0.3, 0, 0,   // right top front 136
			-0.5, 2.5, -0.3, 1, 0,  // left top front 137
			-0.5, 2.5, -0.5, 1, 1,  // left top back 138
			0.5,  2.5, -0.5, 0, 1,   // right top back 139

			//// bottom
			//-0.5, 1.8, -0.5, 0, 0, // left bottom back 140
			//0.5, 1.8, -0.5, 1, 0,  // right bottom back 141
			//0.5, 1.8, 0.5, 1, 1,  // right bottom front 142
			//-0.5, 1.8, 0.5, 0, 1, // left bottom front 143
	};

	unsigned int indices[] = {
		0,     1,       2,      0,      2,       3,
		4,     5,       6,      4,      6,       7,
		8,     9,       10,     8,     10,       11,
		12,    14,      13,     12,    15,       14,
		16,    18,      17,     16,    19,       18,
		20,    22,      21,     20,    23,       22,

		24,    25,    26,    24,    26,    27,
		28,    29,    30,    28,    30,    31,
		32,    33,    34,    32,    34,    35,
		36,    38,    37,    36,    39,    38,
		40,    42,    41,    40,    43,    42,
		44,    46,    45,    44,    47,    46,

		48,    49,    50,    48,    50,    51,
		52,    53,    54,    52,    54,    55,
		56,    57,    58,    56,    58,    59,
		60,    62,    61,    60,    63,    62,
		64,    66,    65,    64,    67,    66,
		68,    70,    69,    68,    71,    70,

		72,    73,      74,     72,    74,       75,
		76,    77,      78,     76,    78,       79,
		80,    81,      82,     80,    82,       83,
		84,    86,      85,     84,    87,       86,
		88,    90,      89,     88,    91,       90,
		92,    94,      93,     92,    95,       94,

		96,    97,      98,     96,    98,       99,
	   100,   101,     102,    100,   102,      103,
	   104,   105,     106,    104,   106,      107,
	   108,   110,     109,    108,   111,      110,
	   112,   114,     113,    112,   115,      114,
	   116,   118,     117,    116,   119,      118,


	   120,      121,     122,   120,   122,      123,
	   124,      125,     126,   124,   126,      127,
	   128,      129,     130,   128,   130,      131,
	   132,      134,     133,   132,   135,      134,
	   136,      138,     137,   136,   139,      138,
	   140,      142,     141,   140,   143,      142

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

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Texture Mapping: Draw textured cube and plane", 800, 600, false, false);
}
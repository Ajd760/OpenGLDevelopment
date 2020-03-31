/// Next TODO:
// Implement GLFW Window wrapper class


/*
* Include glad before GLFW, otherwise glad will bark about already including opengl headers
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Global settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// Ad hoc shaders - temporary
const char* vertShaderSrc = "#version 330 core\n"
						"layout (location = 0) in vec3 aPos;\n"
						"void main()\n"
						"{\n"
						"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
						"}\n\0";

// Orange-ish frag shader
const char* fragShaderSrc_orange = "#version 330 core\n"
						"out vec4 FragColor;\n"
						"void main()\n"
						"{\n"
						"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
						"}\n\0";

// Yellow frag shader
const char* fragShaderSrc_yellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";


int main()
{
#pragma region initstuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGLDevelopment", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set callback function to be called each time window is resized
#pragma endregion initstuff


	// Compile and Link Shaders
	// ----------------------------------------------------
	// Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);
	// Check vertex shader compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED!\n" << infoLog << std::endl;
	}

	// Compile fragment shader orange
	unsigned int fragmentShader_orange;
	fragmentShader_orange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_orange, 1, &fragShaderSrc_orange, NULL);
	glCompileShader(fragmentShader_orange);
	// Check fragment shader compilation
	glGetShaderiv(fragmentShader_orange, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_orange, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED!\n" << infoLog << std::endl;
	}

	// Compile fragment shader yellow
	unsigned int fragmentShader_yellow;
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_yellow, 1, &fragShaderSrc_yellow, NULL);
	glCompileShader(fragmentShader_yellow);
	// Check fragment shader compilation
	glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED!\n" << infoLog << std::endl;
	}

	//Link Shader program 1
	unsigned int shaderProgram_1;
	shaderProgram_1 = glCreateProgram();
	glAttachShader(shaderProgram_1, vertexShader);
	glAttachShader(shaderProgram_1, fragmentShader_orange);
	glLinkProgram(shaderProgram_1);
	// Check shader program linking for errors
	glGetProgramiv(shaderProgram_1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED!\n" << infoLog << std::endl;
	}

	//Link Shader program 2
	unsigned int shaderProgram_2;
	shaderProgram_2 = glCreateProgram();
	glAttachShader(shaderProgram_2, vertexShader);
	glAttachShader(shaderProgram_2, fragmentShader_yellow);
	glLinkProgram(shaderProgram_2);
	// Check shader program linking for errors
	glGetProgramiv(shaderProgram_2, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram_2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED!\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader_orange);
	glDeleteShader(fragmentShader_yellow);
	// ----------------------------------------------------


	// Create vertex and buffer data, configure vertex attributes
	// -----------------------------------------------------------
	// Create 2 triangles
	float vertices1[] = {
		0.75f, 0.75f, 0.0f, // 1st triangle in top right of screen
		0.75f, 0.25f, 0.0f, 
		0.0f, 0.25f, 0.0f, 
	};

	float vertices2[] = {
		-0.75f, -0.75f, 0.0f, //2nd triangle in bottom left of screen (just mirrors first triangle)
		-0.75f, -0.25f, 0.0f,
		0.0f, -0.25f, 0.0f
	};
	
	// Indices to create 2 triangles forming the rectangle (for EBO)
	unsigned int indices[] = {
		0, 1, 3, //first triangle
		1, 2, 3  //second triangle
	};

	// Create and bind a Vertex Buffer Object and Vertex Array Object, and give them the data in the vertices[] array
	unsigned int VBO1, VBO2, VAO1, VAO2, EBO; //EBO not used in this exercise
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	//glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind/set the Vertex buffer object, then the element buffer object
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// EBO NOT USED IN THIS EXERCISE - but left in for reference
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// glVertexAttribPointer tells openGL how to process the vertex array data (starting at psition 0 in the array, stride of 3 (x,y,z coords), data values are floats, etc)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //This last param is the offset of where the position data begins in the buffer, and requires the cast (void*)
	glEnableVertexAttribArray(0);

	// Unbind the VBO since the call to glVertexAttribPointer registered the VBO as the vertex attribute's bound VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ** Can unbind the VAO so that other VAO calls won't accidentally modify this VAO, however this rarely happens since 
	//		modifying other VAOs require a call to glBindVertexArray anyways
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment to draw in WIREFRAME mode

	//Bind and get data for 2nd VBO/VAO
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// -----------------------------------------------------------

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Check inputs
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	//Clear screen with a grey/green color
		glClear(GL_COLOR_BUFFER_BIT);			// Actually clear the screen

		// Draw a triangle
		glUseProgram(shaderProgram_1);
		glBindVertexArray(VAO1);			
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Drawing a triangle, starting at index 0 of the bound array, drawing 3 vertices
		//glBindVertexArray(0);

		// Draw 2nd triangle, with a different shader program
		glUseProgram(shaderProgram_2);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);	//Drawing a triangle, starting at index 0 of the bound array, drawing 3 vertices
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Drawing 2 triangles to form a rectangle with an EBO
		glBindVertexArray(0);  //don't need to unbind every time

		// Check/call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Deallocate everything before program end
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}


// Callback function to be registered to the window that should get called 
//	each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Processes user input each frame
void processInput(GLFWwindow* window)
{
	// Check if user pressed escape, if so close the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}